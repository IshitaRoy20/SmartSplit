#pragma once

#include "IMemberRepository.h"

class FileMemberRepository
    : public IMemberRepository
{
public:

    void saveMember(
        const Member& member
    ) override;

    std::vector<Member>
    getAllMembers() override;

    void deleteMember(
        int memberId
    ) override;
};