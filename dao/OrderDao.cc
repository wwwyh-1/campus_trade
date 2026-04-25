#include "OrderDao.h"
#include <drogon/drogon.h>
#include <iostream>

bool OrderDao::createOrderWithTransaction(int userId,
                                          int productId,
                                          double &amount)
{
    try
    {
        auto client = drogon::app().getDbClient();

        // 创建事务对象
        auto trans = client->newTransaction();

        // 1. 查询商品价格和库存
        auto productResult = trans->execSqlSync(
            "SELECT price, stock FROM products WHERE id = ? AND status = 1",
            productId);

        if (productResult.empty())
        {
            trans->rollback();
            return false;
        }

        amount = productResult[0]["price"].as<double>();
        int stock = productResult[0]["stock"].as<int>();

        if (stock <= 0)
        {
            trans->rollback();
            return false;
        }

        // 2. 扣减库存
        auto updateResult = trans->execSqlSync(
            "UPDATE products SET stock = stock - 1 WHERE id = ? AND stock > 0",
            productId);

        if (updateResult.affectedRows() <= 0)
        {
            trans->rollback();
            return false;
        }

        // 3. 创建订单
        trans->execSqlSync(
            "INSERT INTO orders(user_id, product_id, amount, status) VALUES(?, ?, ?, 0)",
            userId,
            productId,
            amount);

        // 4. 提交事务
        

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "create order transaction failed: " << e.what() << std::endl;
        return false;
    }
}

Json::Value OrderDao::getOrdersByUserId(int userId)
{
    Json::Value arr(Json::arrayValue);

    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "SELECT o.id, o.user_id, o.product_id, o.amount, o.status, o.created_at, "
            "p.name AS product_name "
            "FROM orders o "
            "JOIN products p ON o.product_id = p.id "
            "WHERE o.user_id = ? "
            "ORDER BY o.id DESC",
            userId);

        for (auto row : result)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["user_id"] = row["user_id"].as<int>();
            item["product_id"] = row["product_id"].as<int>();
            item["product_name"] = row["product_name"].as<std::string>();
            item["amount"] = row["amount"].as<double>();
            item["status"] = row["status"].as<int>();
            item["created_at"] = row["created_at"].as<std::string>();

            arr.append(item);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "get orders by user id failed: " << e.what() << std::endl;
    }

    return arr;
}

bool OrderDao::cancelOrder(int orderId)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "UPDATE orders SET status = 2 WHERE id = ? AND status = 0",
            orderId);

        return result.affectedRows() > 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "cancel order failed: " << e.what() << std::endl;
        return false;
    }
}