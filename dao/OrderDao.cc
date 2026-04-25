#include "OrderDao.h"
#include <drogon/drogon.h>
#include <iostream>

bool OrderDao::getProductPriceAndStock(int productId, double &price, int &stock)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "SELECT price, stock FROM products WHERE id = ? AND status = 1",
            productId);

        if (result.empty())
        {
            return false;
        }

        price = result[0]["price"].as<double>();
        stock = result[0]["stock"].as<int>();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "get product price and stock failed: " << e.what() << std::endl;
        return false;
    }
}

bool OrderDao::decreaseStock(int productId)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "UPDATE products SET stock = stock - 1 WHERE id = ? AND stock > 0",
            productId);

        return result.affectedRows() > 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "decrease stock failed: " << e.what() << std::endl;
        return false;
    }
}

bool OrderDao::createOrder(int userId, int productId, double amount)
{
    try
    {
        auto client = drogon::app().getDbClient();

        client->execSqlSync(
            "INSERT INTO orders(user_id, product_id, amount, status) VALUES(?, ?, ?, 0)",
            userId,
            productId,
            amount);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "create order failed: " << e.what() << std::endl;
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