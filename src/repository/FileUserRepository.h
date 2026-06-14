#pragma once

#include "IUserRepository.h"

class FileUserRepository
    : public IUserRepository
{
public:

    void saveUser(
        const User& user
    ) override;

    std::vector<User>
    getAllUsers() override;
};