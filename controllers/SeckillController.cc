#include "SeckillController.h"
#include "../services/SeckillService.h"
#include <json/json.h>
#include <string>

void SeckillController::createSeckillOrder(
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

    SeckillService seckillService;
    std::string message;

    bool ok = seckillService.createSeckillOrder(userId, productId, message);

    resp["code"] = ok ? 0 : -1;
    resp["message"] = message;

    callback(HttpResponse::newHttpJsonResponse(resp));
}