#pragma once

#include <drogon/HttpController.h>
#include <functional>

using namespace drogon;

class SeckillController : public HttpController<SeckillController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SeckillController::createSeckillOrder, "/api/seckill/create", Post);
    METHOD_LIST_END

    void createSeckillOrder(const HttpRequestPtr &req,
                            std::function<void(const HttpResponsePtr &)> &&callback);
};