#pragma once

#include "IGroupRepository.h"

class FileGroupRepository
    : public IGroupRepository
{
public:

    void saveGroup(
        const Group& group
    ) override;

    std::vector<Group>
    getAllGroups() override;
};