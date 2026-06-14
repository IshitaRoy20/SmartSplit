#pragma once

#include "../models/User.h"
#include <vector>

class IUserRepository
{
public:

    virtual void saveUser(
        const User& user
    ) = 0;

    virtual std::vector<User>
    getAllUsers() = 0;

    virtual ~IUserRepository() = default;
};