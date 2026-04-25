#pragma once

#include <string>
#include <json/json.h>

class ProductService
{
public:
    bool createProduct(int sellerId,
                       const std::string &name,
                       const std::string &description,
                       double price,
                       int stock,
                       std::string &message);

    Json::Value getProductList();

    bool getProductById(int productId,
                        Json::Value &product,
                        std::string &message);

    bool updateProduct(int productId,
                       const std::string &name,
                       const std::string &description,
                       double price,
                       int stock,
                       std::string &message);

    bool deleteProduct(int productId,
                       std::string &message);
};