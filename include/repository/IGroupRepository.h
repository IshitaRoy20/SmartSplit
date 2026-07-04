#pragma once

#include <vector>
#include "../models/Group.h"

class IGroupRepository
{
public:

    virtual Group addGroup(
        const std::string& name
    ) = 0;

    virtual std::vector<Group>
    getAllGroups() = 0;

    virtual ~IGroupRepository() = default;
};
