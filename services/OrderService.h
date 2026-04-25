#pragma once

#include <json/json.h>
#include <string>

class OrderService
{
public:
    bool createOrder(int userId,
                     int productId,
                     std::string &message);

    Json::Value getOrdersByUserId(int userId);

    bool cancelOrder(int orderId,
                     std::string &message);
};