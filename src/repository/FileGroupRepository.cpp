#include "../include/repository/FileGroupRepository.h"
#include "../include/utilities/ConfigPath.h"
#include <fstream>
#include <sstream>
#include <iostream>

void FileGroupRepository::saveGroup(const Group& group)
{
    std::ofstream file(Config::Path::getGroupsFile(), std::ios::app);

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open groups file for writing.\n";
        return;
    }

    file << group.getId() << "," << group.getName() << "\n";
    file.close();
}

std::vector<Group> FileGroupRepository::getAllGroups()
{
    std::vector<Group> groups;
    std::ifstream file(Config::Path::getGroupsFile());

    if(!file.is_open())
        return groups;

    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty()) 
            continue;

        std::stringstream ss(line);
        std::string idStr, name;

        std::getline(ss, idStr, ',');
        std::getline(ss, name);

        try
        {
            groups.emplace_back(std::stoi(idStr), name);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error parsing group: " << e.what() << "\n";
            continue;
        }
    }

    file.close();
    return groups;
}