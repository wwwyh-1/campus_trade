#pragma once

#include <drogon/HttpController.h>
#include <functional>

using namespace drogon;

class OrderController : public HttpController<OrderController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(OrderController::createOrder, "/api/order/create", Post);
    ADD_METHOD_TO(OrderController::getUserOrders, "/api/order/user/{1}", Get);
    ADD_METHOD_TO(OrderController::cancelOrder, "/api/order/cancel/{1}", Put);
    METHOD_LIST_END

    void createOrder(const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&callback);

    void getUserOrders(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback,
                       int userId);

    void cancelOrder(const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&callback,
                     int orderId);
};