#pragma once

#include <json/json.h>

class OrderDao
{
public:
    bool getProductPriceAndStock(int productId, double &price, int &stock);

    bool decreaseStock(int productId);

    bool createOrder(int userId, int productId, double amount);

    Json::Value getOrdersByUserId(int userId);

    bool cancelOrder(int orderId);
};