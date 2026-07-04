#pragma once

#include "IGroupRepository.h"
#include "../cache/LRUCache.h"

class SQLiteGroupRepository
    : public IGroupRepository
{
private:

    LRUCache<int, std::vector<Group>> cache;

public:

    SQLiteGroupRepository();

    Group addGroup(
        const std::string& name
    ) override;

    std::vector<Group>
    getAllGroups() override;
};
