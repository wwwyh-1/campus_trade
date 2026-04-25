#include "UserDao.h"
#include <drogon/drogon.h>
#include <iostream>

bool UserDao::isUsernameExists(const std::string &username)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "SELECT id FROM users WHERE username = ?",
            username);

        return !result.empty();
    }
    catch (const std::exception &e)
    {
        std::cerr << "check username failed: " << e.what() << std::endl;
        return true;
    }
}

bool UserDao::insertUser(const std::string &username,
                         const std::string &password,
                         const std::string &phone)
{
    try
    {
        auto client = drogon::app().getDbClient();

        client->execSqlSync(
            "INSERT INTO users(username, password, phone) VALUES(?, ?, ?)",
            username,
            password,
            phone);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "insert user failed: " << e.what() << std::endl;
        return false;
    }
}
bool UserDao::checkLogin(const std::string &username,
                         const std::string &password,
                         int &userId)
{
    try
    {
        auto client = drogon::app().getDbClient();

        auto result = client->execSqlSync(
            "SELECT id, password FROM users WHERE username = ?",
            username);

        if (result.empty())
        {
            return false;
        }

        std::string dbPassword = result[0]["password"].as<std::string>();

        if (dbPassword != password)
        {
            return false;
        }

        userId = result[0]["id"].as<int>();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "check login failed: " << e.what() << std::endl;
        return false;
    }
}