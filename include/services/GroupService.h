#pragma once

#include <iostream>

#include "../repository/SQLiteGroupRepository.h"

class GroupService
{
private:

    SQLiteGroupRepository repository;

public:

    void createGroup(
        const std::string& name
    )
    {
        repository.addGroup(name);
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

    std::vector<Group> getAllGroups()
    {
        return repository.getAllGroups();
    }
};
