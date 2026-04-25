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

    double price = 0.0;
    int stock = 0;

    if (!orderDao.getProductPriceAndStock(productId, price, stock))
    {
        message = "product not found";
        return false;
    }

    if (stock <= 0)
    {
        message = "product stock not enough";
        return false;
    }

    if (!orderDao.decreaseStock(productId))
    {
        message = "decrease stock failed";
        return false;
    }

    if (!orderDao.createOrder(userId, productId, price))
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