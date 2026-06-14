#pragma once
#include <iostream>
#include "../repository/FileUserRepository.h"

class UserService
{
private:

    FileUserRepository repository;

    int nextId = 1;

public:

    UserService()
    {
        auto users =
            repository.getAllUsers();

        if(!users.empty())
        {
            nextId =
                users.back().getId()
                + 1;
        }
    }

    void createUser(
        const std::string& name
    )
    {
        User user(
            nextId++,
            name
        );

        repository.saveUser(user);
    }

    void listUsers()
    {
        auto users =
            repository.getAllUsers();

        if(users.empty())
        {
            std::cout
                << "\nNo Users Found\n";

            return;
        }

        for(auto& u : users)
        {
            std::cout
                << u.getId()
                << ". "
                << u.getName()
                << "\n";
        }
    }
};