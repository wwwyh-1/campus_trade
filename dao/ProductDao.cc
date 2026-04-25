#include "ProductDao.h"
#include <drogon/drogon.h>
#include <iostream>

bool ProductDao::insertProduct(int sellerId,
                               const std::string &name,
                               const std::string &description,
                               double price,
                               int stock)
{
    try
    {
        auto client = drogon::app().getDbClient();

        client->execSqlSync(
            "INSERT INTO products(seller_id, name, description, price, stock) "
            "VALUES(?, ?, ?, ?, ?)",
            sellerId,
            name,
            description,
            price,
            stock);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "insert product failed: " << e.what() << std::endl;
        return false;
    }
}

Json::Value ProductDao::getProductList()
{
    Json::Value arr(Json::arrayValue);

    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "SELECT id, seller_id, name, description, price, stock, status, created_at "
            "FROM products WHERE status = 1 ORDER BY id DESC");

        for (auto row : result)
        {
            Json::Value item;
            item["id"] = row["id"].as<int>();
            item["seller_id"] = row["seller_id"].as<int>();
            item["name"] = row["name"].as<std::string>();
            item["description"] = row["description"].as<std::string>();
            item["price"] = row["price"].as<double>();
            item["stock"] = row["stock"].as<int>();
            item["status"] = row["status"].as<int>();
            item["created_at"] = row["created_at"].as<std::string>();

            arr.append(item);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "get product list failed: " << e.what() << std::endl;
    }

    return arr;
}

bool ProductDao::getProductById(int productId, Json::Value &product)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "SELECT id, seller_id, name, description, price, stock, status, created_at "
            "FROM products WHERE id = ? AND status = 1",
            productId);

        if (result.empty())
        {
            return false;
        }

        auto row = result[0];

        product["id"] = row["id"].as<int>();
        product["seller_id"] = row["seller_id"].as<int>();
        product["name"] = row["name"].as<std::string>();
        product["description"] = row["description"].as<std::string>();
        product["price"] = row["price"].as<double>();
        product["stock"] = row["stock"].as<int>();
        product["status"] = row["status"].as<int>();
        product["created_at"] = row["created_at"].as<std::string>();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "get product by id failed: " << e.what() << std::endl;
        return false;
    }
}

bool ProductDao::updateProduct(int productId,
                               const std::string &name,
                               const std::string &description,
                               double price,
                               int stock)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "UPDATE products SET name = ?, description = ?, price = ?, stock = ? "
            "WHERE id = ? AND status = 1",
            name,
            description,
            price,
            stock,
            productId);

        return result.affectedRows() > 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "update product failed: " << e.what() << std::endl;
        return false;
    }
}

bool ProductDao::deleteProduct(int productId)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "UPDATE products SET status = 0 WHERE id = ? AND status = 1",
            productId);

        return result.affectedRows() > 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "delete product failed: " << e.what() << std::endl;
        return false;
    }
}