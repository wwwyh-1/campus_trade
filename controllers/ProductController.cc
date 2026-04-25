#include "ProductController.h"
#include "../services/ProductService.h"
#include <json/json.h>
#include <string>

void ProductController::createProduct(
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

    int sellerId = (*json)["seller_id"].asInt();
    std::string name = (*json)["name"].asString();
    std::string description = (*json)["description"].asString();
    double price = (*json)["price"].asDouble();
    int stock = (*json)["stock"].asInt();

    ProductService productService;
    std::string message;

    bool ok = productService.createProduct(
        sellerId, name, description, price, stock, message);

    resp["code"] = ok ? 0 : -1;
    resp["message"] = message;

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void ProductController::listProducts(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value resp;

    ProductService productService;
    Json::Value products = productService.getProductList();

    resp["code"] = 0;
    resp["message"] = "get product list success";
    resp["data"] = products;

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void ProductController::getProductDetail(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    int productId)
{
    Json::Value resp;
    Json::Value product;
    std::string message;

    ProductService productService;
    bool ok = productService.getProductById(productId, product, message);

    resp["code"] = ok ? 0 : -1;
    resp["message"] = message;

    if (ok)
    {
        resp["data"] = product;
    }

    callback(HttpResponse::newHttpJsonResponse(resp));
}