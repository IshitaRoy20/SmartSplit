#pragma once

#include "../models/Member.h"

#include <vector>

class IMemberRepository
{
public:

    virtual Member addMember(
        int groupId,
        const std::string& name
    ) = 0;

    virtual std::vector<Member>
    getMembersByGroup(
        int groupId
    ) = 0;

    virtual void deleteMember(
        int groupId,
        int memberId
    ) = 0;

    virtual ~IMemberRepository() = default;
};
