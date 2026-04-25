#pragma once

#include <string>

class SeckillService
{
public:
    bool createSeckillOrder(int userId,
                            int productId,
                            std::string &message);
};