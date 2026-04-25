#pragma once

#include <json/json.h>

class OrderDao
{
public:
    bool createOrderWithTransaction(int userId,
                                    int productId,
                                    double &amount);

    Json::Value getOrdersByUserId(int userId);

    bool cancelOrder(int orderId);
};