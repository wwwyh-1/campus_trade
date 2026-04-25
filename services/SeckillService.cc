#include "SeckillService.h"
#include "../dao/OrderDao.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <string>

bool SeckillService::createSeckillOrder(int userId,
                                        int productId,
                                        std::string &message)
{
    if (userId <= 0)
    {
        message = "invalid user id";
        return false;
    }

    if (productId <= 0)
    {
        message = "invalid product id";
        return false;
    }

    std::string stockKey = "seckill:stock:" + std::to_string(productId);

    redisContext *context = redisConnect("127.0.0.1", 6379);

    if (context == nullptr || context->err)
    {
        message = "redis connect failed";

        if (context != nullptr)
        {
            redisFree(context);
        }

        return false;
    }

    /*
        Lua 脚本作用：
        1. 从 Redis 读取 seckill:stock:{productId}
        2. 如果库存 key 不存在，返回 -2
        3. 如果库存 <= 0，返回 -1
        4. 如果库存充足，执行 DECR 扣减库存
        5. 返回扣减后的库存

        这样“判断库存 + 扣减库存”在 Redis 中原子执行，
        可以避免并发秒杀时多个请求同时扣减导致超卖。
    */
    const char *luaScript =
        "local stock = tonumber(redis.call('GET', KEYS[1]));"
        "if stock == nil then "
        "   return -2;"
        "end;"
        "if stock <= 0 then "
        "   return -1;"
        "end;"
        "redis.call('DECR', KEYS[1]);"
        "return stock - 1;";

    redisReply *reply = (redisReply *)redisCommand(
        context,
        "EVAL %s 1 %s",
        luaScript,
        stockKey.c_str());

    if (reply == nullptr)
    {
        message = "redis lua execute failed";
        redisFree(context);
        return false;
    }

    long long result = reply->integer;
    freeReplyObject(reply);

    if (result == -2)
    {
        message = "seckill stock not initialized";
        redisFree(context);
        return false;
    }

    if (result == -1)
    {
        message = "seckill stock not enough";
        redisFree(context);
        return false;
    }

    /*
        Redis 秒杀库存预减成功后，再进入 MySQL 创建订单流程。
        createOrderWithTransaction 内部负责：
        1. 查询商品库存
        2. 扣减 MySQL 商品库存
        3. 创建订单
        4. 失败时回滚事务
    */
    OrderDao orderDao;
    double amount = 0.0;

    bool ok = orderDao.createOrderWithTransaction(userId, productId, amount);

    if (!ok)
    {
        /*
            如果 Redis 库存已经预减成功，
            但是 MySQL 事务创建订单失败，
            需要把 Redis 秒杀库存加回来。
        */
        redisReply *rollbackReply = (redisReply *)redisCommand(
            context,
            "INCR %s",
            stockKey.c_str());

        if (rollbackReply != nullptr)
        {
            freeReplyObject(rollbackReply);
        }

        message = "create seckill order failed";
        redisFree(context);
        return false;
    }

    /*
        秒杀订单创建成功后，商品库存已经变化。
        这里主动删除商品详情缓存 product:{id}，
        避免商品详情接口继续返回旧库存。
    */
    std::string productCacheKey = "product:" + std::to_string(productId);

    redisReply *delReply = (redisReply *)redisCommand(
        context,
        "DEL %s",
        productCacheKey.c_str());

    if (delReply != nullptr)
    {
        freeReplyObject(delReply);
    }

    message = "create seckill order success";
    redisFree(context);
    return true;
}