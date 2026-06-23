#pragma once

#include <vector>
#include "../models/Group.h"

class IGroupRepository
{
public:

    virtual void saveGroup(
        const Group& group
    ) = 0;

    virtual std::vector<Group>
    getAllGroups() = 0;

    virtual ~IGroupRepository() = default;
};