#include "UserService.h"
#include "../dao/UserDao.h"

bool UserService::registerUser(const std::string &username,
                               const std::string &password,
                               const std::string &phone,
                               std::string &message)
{
    if (username.empty() || password.empty())
    {
        message = "username or password is empty";
        return false;
    }

    UserDao userDao;

    if (userDao.isUsernameExists(username))
    {
        message = "username already exists";
        return false;
    }

    if (!userDao.insertUser(username, password, phone))
    {
        message = "register failed";
        return false;
    }

    message = "register success";
    return true;
}
bool UserService::login(const std::string &username,
                        const std::string &password,
                        int &userId,
                        std::string &message)
{
    if (username.empty() || password.empty())
    {
        message = "username or password is empty";
        return false;
    }

    UserDao userDao;

    if (!userDao.checkLogin(username, password, userId))
    {
        message = "username or password error";
        return false;
    }

    message = "login success";
    return true;
}