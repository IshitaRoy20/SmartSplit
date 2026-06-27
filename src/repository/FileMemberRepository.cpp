#include "../include/repository/FileMemberRepository.h"
#include "../include/utilities/ConfigPath.h"
#include <fstream>
#include <sstream>
#include <iostream>

void FileMemberRepository::saveMember(const Member& member)
{
    std::ofstream file(Config::Path::getMembersFile(), std::ios::app);

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open members file for writing.\n";
        return;
    }

    file << member.getId() << "," << member.getGroupId() << "," << member.getName() << "\n";
    file.close();
}

std::vector<Member> FileMemberRepository::getAllMembers()
{
    std::vector<Member> members;
    std::ifstream file(Config::Path::getMembersFile());

    if(!file.is_open())
        return members;

    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty()) 
            continue;

        std::stringstream ss(line);
        std::string idStr, groupIdStr, name;

        std::getline(ss, idStr, ',');
        std::getline(ss, groupIdStr, ',');
        std::getline(ss, name);

        try
        {
            members.emplace_back(std::stoi(idStr), std::stoi(groupIdStr), name);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error parsing member: " << e.what() << "\n";
            continue;
        }
    }

    file.close();
    return members;
}

void FileMemberRepository::deleteMember(int memberId)
{
    auto members = getAllMembers();
    std::ofstream file(Config::Path::getMembersFile());

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open members file for writing.\n";
        return;
    }

    for(const auto& member : members)
    {
        if(member.getId() != memberId)
        {
            file << member.getId() << "," << member.getGroupId() << "," << member.getName() << "\n";
        }
    }

    file.close();
}