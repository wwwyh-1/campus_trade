#pragma once

#include <string>
#include <json/json.h>

class ProductDao
{
public:
    bool insertProduct(int sellerId,
                       const std::string &name,
                       const std::string &description,
                       double price,
                       int stock);

    Json::Value getProductList();

    bool getProductById(int productId, Json::Value &product);
};