#include "OrderService.h"
#include "../dao/OrderDao.h"

bool OrderService::createOrder(int userId,
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

    OrderDao orderDao;

    double amount = 0.0;

    if (!orderDao.createOrderWithTransaction(userId, productId, amount))
    {
        message = "create order failed";
        return false;
    }

    message = "create order success";
    return true;
}

Json::Value OrderService::getOrdersByUserId(int userId)
{
    OrderDao orderDao;
    return orderDao.getOrdersByUserId(userId);
}

bool OrderService::cancelOrder(int orderId,
                               std::string &message)
{
    if (orderId <= 0)
    {
        message = "invalid order id";
        return false;
    }

    OrderDao orderDao;

    if (!orderDao.cancelOrder(orderId))
    {
        message = "cancel order failed";
        return false;
    }

    message = "cancel order success";
    return true;
}