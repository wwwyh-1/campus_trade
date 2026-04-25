#include "UserController.h"
#include "../services/UserService.h"
#include <json/json.h>
#include <string>

void UserController::registerUser(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value resp;
    auto json = req->getJsonObject();

    if (!json)
    {
        resp["code"] = -1;
        resp["message"] = "invalid json";

        auto response = HttpResponse::newHttpJsonResponse(resp);
        callback(response);
        return;
    }

    std::string username = (*json)["username"].asString();
    std::string password = (*json)["password"].asString();
    std::string phone = (*json)["phone"].asString();

    UserService userService;
    std::string message;

    bool ok = userService.registerUser(username, password, phone, message);

    if (ok)
    {
        resp["code"] = 0;
        resp["message"] = message;
        resp["data"]["username"] = username;
        resp["data"]["phone"] = phone;
    }
    else
    {
        resp["code"] = -1;
        resp["message"] = message;
    }

    auto response = HttpResponse::newHttpJsonResponse(resp);
    callback(response);
}
