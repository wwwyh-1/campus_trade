#include "ProductService.h"
#include "../dao/ProductDao.h"
#include <hiredis/hiredis.h>
#include <json/json.h>
#include <sstream>
#include <iostream>

bool ProductService::createProduct(int sellerId,
                                   const std::string &name,
                                   const std::string &description,
                                   double price,
                                   int stock,
                                   std::string &message)
{
    if (sellerId <= 0)
    {
        message = "invalid seller id";
        return false;
    }

    if (name.empty())
    {
        message = "product name is empty";
        return false;
    }

    if (price <= 0)
    {
        message = "invalid price";
        return false;
    }

    if (stock < 0)
    {
        message = "invalid stock";
        return false;
    }

    ProductDao productDao;

    if (!productDao.insertProduct(sellerId, name, description, price, stock))
    {
        message = "create product failed";
        return false;
    }

    message = "create product success";
    return true;
}

Json::Value ProductService::getProductList()
{
    ProductDao productDao;
    return productDao.getProductList();
}

bool ProductService::getProductById(int productId,
                                    Json::Value &product,
                                    std::string &message)
{
    if (productId <= 0)
    {
        message = "invalid product id";
        return false;
    }

    std::string cacheKey = "product:" + std::to_string(productId);

    // 1. 先查 Redis
    redisContext *context = redisConnect("127.0.0.1", 6379);

    if (context != nullptr && context->err == 0)
    {
        redisReply *reply = (redisReply *)redisCommand(context, "GET %s", cacheKey.c_str());

        if (reply != nullptr && reply->type == REDIS_REPLY_STRING)
        {
            Json::CharReaderBuilder reader;
            Json::Value cachedProduct;
            std::string errs;
            std::istringstream iss(reply->str);

            if (Json::parseFromStream(reader, iss, &cachedProduct, &errs))
            {
                product = cachedProduct;
                message = "get product success from redis";

                freeReplyObject(reply);
                redisFree(context);
                return true;
            }
        }

        if (reply != nullptr)
        {
            freeReplyObject(reply);
        }
    }
    else
    {
        std::cerr << "redis connect failed" << std::endl;
    }

    if (context != nullptr)
    {
        redisFree(context);
    }

    // 2. Redis 没有，查 MySQL
    ProductDao productDao;

    if (!productDao.getProductById(productId, product))
    {
        message = "product not found";
        return false;
    }

    // 3. 把 MySQL 查到的数据写入 Redis，缓存 300 秒
    redisContext *setContext = redisConnect("127.0.0.1", 6379);

    if (setContext != nullptr && setContext->err == 0)
    {
        Json::StreamWriterBuilder writer;
        std::string jsonStr = Json::writeString(writer, product);

        redisReply *setReply = (redisReply *)redisCommand(
            setContext,
            "SETEX %s %d %s",
            cacheKey.c_str(),
            300,
            jsonStr.c_str());

        if (setReply != nullptr)
        {
            freeReplyObject(setReply);
        }
    }
    else
    {
        std::cerr << "redis set connect failed" << std::endl;
    }

    if (setContext != nullptr)
    {
        redisFree(setContext);
    }

    message = "get product success from mysql";
    return true;
}