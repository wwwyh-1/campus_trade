#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class UserController : public HttpController<UserController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::registerUser, "/api/user/register", Post);
    ADD_METHOD_TO(UserController::loginUser, "/api/user/login", Post);
    METHOD_LIST_END

    void registerUser(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback);

    void loginUser(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback);
};