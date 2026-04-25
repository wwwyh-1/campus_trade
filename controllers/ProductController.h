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
    ADD_METHOD_TO(ProductController::updateProduct, "/api/product/update/{1}", Put);
    ADD_METHOD_TO(ProductController::deleteProduct, "/api/product/delete/{1}", Delete);
    METHOD_LIST_END

    void createProduct(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback);

    void listProducts(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback);

    void getProductDetail(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback,
                          int productId);

    void updateProduct(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback,
                       int productId);

    void deleteProduct(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback,
                       int productId);
};