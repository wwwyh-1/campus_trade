#include "ProductService.h"
#include "../dao/ProductDao.h"

bool ProductService::createProduct(int sellerId,
                                   const std::string &name,
                                   const std::string &description,
                                   double price,
                                   int stock,
                                   std::string &message)
{
    if (sellerId <= 0)
    {
        message = "invalid seller id";
        return false;
    }

    if (name.empty())
    {
        message = "product name is empty";
        return false;
    }

    if (price <= 0)
    {
        message = "invalid price";
        return false;
    }

    if (stock < 0)
    {
        message = "invalid stock";
        return false;
    }

    ProductDao productDao;

    if (!productDao.insertProduct(sellerId, name, description, price, stock))
    {
        message = "create product failed";
        return false;
    }

    message = "create product success";
    return true;
}

Json::Value ProductService::getProductList()
{
    ProductDao productDao;
    return productDao.getProductList();
}

bool ProductService::getProductById(int productId,
                                    Json::Value &product,
                                    std::string &message)
{
    if (productId <= 0)
    {
        message = "invalid product id";
        return false;
    }

    ProductDao productDao;

    if (!productDao.getProductById(productId, product))
    {
        message = "product not found";
        return false;
    }

    message = "get product success";
    return true;
}