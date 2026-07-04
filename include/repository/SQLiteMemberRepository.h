#pragma once

#include "IMemberRepository.h"
#include "../cache/LRUCache.h"

class SQLiteMemberRepository
    : public IMemberRepository
{
private:

    LRUCache<int, std::vector<Member>> cache;

public:

    SQLiteMemberRepository();

    Member addMember(
        int groupId,
        const std::string& name
    ) override;

    std::vector<Member>
    getMembersByGroup(
        int groupId
    ) override;

    void deleteMember(
        int groupId,
        int memberId
    ) override;
};
