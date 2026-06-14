#pragma once

#include <iostream>

#include "../repository/FileGroupRepository.h"

class GroupService
{
private:

    FileGroupRepository repository;

    int nextId = 1;

public:

    GroupService()
    {
        auto groups =
            repository.getAllGroups();

        if(!groups.empty())
        {
            nextId =
                groups.back().getId()
                + 1;
        }
    }

    void createGroup(
        const std::string& name
    )
    {
        Group group(
            nextId++,
            name
        );

        repository.saveGroup(
            group
        );
    }

    void listGroups()
    {
        auto groups =
            repository.getAllGroups();

        if(groups.empty())
        {
            std::cout
            << "\nNo Groups Found\n";

            return;
        }

        std::cout
        << "\nGroups:\n";

        for(auto& g : groups)
        {
            std::cout
            << g.getId()
            << ". "
            << g.getName()
            << "\n";
        }
    }
};