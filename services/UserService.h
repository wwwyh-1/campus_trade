#pragma once

#include <string>

class UserService
{
public:
    bool registerUser(const std::string &username,
                      const std::string &password,
                      const std::string &phone,
                      std::string &message);
};