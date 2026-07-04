#pragma once

#include "../repository/SQLiteMemberRepository.h"

#include <iostream>

class MemberService
{
private:

    SQLiteMemberRepository repository;

public:

    void addMember(
        int groupId,
        const std::string& name
    )
    {
        repository.addMember(groupId, name);
    }

    void viewMembers(
        int groupId
    )
    {
        auto members =
            repository.getMembersByGroup(groupId);

        if(members.empty())
        {
            std::cout
                << "\nNo Members Found\n";

            return;
        }

        for(auto& member : members)
        {
            std::cout
                << member.getId()
                << ". "
                << member.getName()
                << "\n";
        }
    }

    void removeMember(
        int groupId,
        int memberId
    )
    {
        repository.deleteMember(
            groupId,
            memberId
        );
    }

    std::vector<Member> getMembersByGroup(
        int groupId
    )
    {
        return repository.getMembersByGroup(groupId);
    }

    bool memberExists(
        int groupId,
        const std::string& name
    )
    {
        auto members =
            repository.getMembersByGroup(groupId);

        for(const auto& member : members)
        {
            if(member.getName() == name)
            {
                return true;
            }
        }

        return false;
    }
};