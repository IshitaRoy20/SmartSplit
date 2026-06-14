#pragma once

#include "../repository/FileMemberRepository.h"

#include <iostream>

class MemberService
{
private:

    FileMemberRepository repository;

    int nextId = 1;

public:

    MemberService()
    {
        auto members =
            repository.getAllMembers();

        if(!members.empty())
        {
            nextId =
                members.back().getId()
                + 1;
        }
    }

    void addMember(
        int groupId,
        const std::string& name
    )
    {
        Member member(
            nextId++,
            groupId,
            name
        );

        repository.saveMember(member);
    }

    void viewMembers(
        int groupId
    )
    {
        auto members =
            repository.getAllMembers();

        bool found = false;

        for(auto& member : members)
        {
            if(member.getGroupId()
               == groupId)
            {
                found = true;

                std::cout
                    << member.getId()
                    << ". "
                    << member.getName()
                    << "\n";
            }
        }

        if(!found)
        {
            std::cout
                << "\nNo Members Found\n";
        }
    }

    void removeMember(
        int memberId
    )
    {
        repository.deleteMember(
            memberId
        );
    }
};