#pragma once

#include <drogon/HttpController.h>
#include <functional>

using namespace drogon;

class ProductController : public HttpController<ProductController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ProductController::createProduct, "/api/product/create", Post);
    ADD_METHOD_TO(ProductController::listProducts, "/api/product/list", Get);
    ADD_METHOD_TO(ProductController::getProductDetail, "/api/product/{1}", Get);
    METHOD_LIST_END

    void createProduct(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback);

    void listProducts(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback);

    void getProductDetail(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback,
                          int productId);
};