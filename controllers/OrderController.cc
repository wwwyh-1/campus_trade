#include "OrderController.h"
#include "../services/OrderService.h"
#include <json/json.h>
#include <string>

void OrderController::createOrder(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value resp;
    auto json = req->getJsonObject();

    if (!json)
    {
        resp["code"] = -1;
        resp["message"] = "invalid json";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    int userId = (*json)["user_id"].asInt();
    int productId = (*json)["product_id"].asInt();

    OrderService orderService;
    std::string message;

    bool ok = orderService.createOrder(userId, productId, message);

    resp["code"] = ok ? 0 : -1;
    resp["message"] = message;

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void OrderController::getUserOrders(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    int userId)
{
    Json::Value resp;

    OrderService orderService;
    Json::Value orders = orderService.getOrdersByUserId(userId);

    resp["code"] = 0;
    resp["message"] = "get user orders success";
    resp["data"] = orders;

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void OrderController::cancelOrder(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    int orderId)
{
    Json::Value resp;

    OrderService orderService;
    std::string message;

    bool ok = orderService.cancelOrder(orderId, message);

    resp["code"] = ok ? 0 : -1;
    resp["message"] = message;

    callback(HttpResponse::newHttpJsonResponse(resp));
}