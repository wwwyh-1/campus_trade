#pragma once

#include <string>

class UserDao
{
public:
    bool isUsernameExists(const std::string &username);
    bool insertUser(const std::string &username,
                    const std::string &password,
                    const std::string &phone);
};