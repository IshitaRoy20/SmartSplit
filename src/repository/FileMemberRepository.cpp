#include "FileMemberRepository.h"

#include <fstream>
#include <sstream>

void FileMemberRepository::saveMember(
    const Member& member
)
{
    std::ofstream file(
        "../data/members.txt",
        std::ios::app
    );

    file
        << member.getId()
        << ","
        << member.getGroupId()
        << ","
        << member.getName()
        << "\n";
}

std::vector<Member>
FileMemberRepository::getAllMembers()
{
    std::vector<Member> members;

    std::ifstream file(
        "../data/members.txt"
    );

    std::string line;

    while(std::getline(file,line))
    {
        std::stringstream ss(line);

        std::string idStr;
        std::string groupIdStr;
        std::string name;

        std::getline(ss,idStr,',');
        std::getline(ss,groupIdStr,',');
        std::getline(ss,name);

        members.emplace_back(
            std::stoi(idStr),
            std::stoi(groupIdStr),
            name
        );
    }

    return members;
}

void FileMemberRepository::deleteMember(
    int memberId
)
{
    auto members =
        getAllMembers();

    std::ofstream file(
        "../data/members.txt"
    );

    for(auto& member : members)
    {
        if(member.getId() == memberId)
        {
            continue;
        }

        file
            << member.getId()
            << ","
            << member.getGroupId()
            << ","
            << member.getName()
            << "\n";
    }
}