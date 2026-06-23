#pragma once

#include "../models/Member.h"

#include <vector>

class IMemberRepository
{
public:

    virtual void saveMember(
        const Member& member
    ) = 0;

    virtual std::vector<Member>
    getAllMembers() = 0;

    virtual void deleteMember(
        int memberId
    ) = 0;

    virtual ~IMemberRepository() = default;
};