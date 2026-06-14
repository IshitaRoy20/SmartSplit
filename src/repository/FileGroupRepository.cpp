#include "FileGroupRepository.h"
#include <iostream>
#include <fstream>
#include <sstream>

void FileGroupRepository::saveGroup(
    const Group& group
)
{
    std::ofstream file("../data/groups.txt", std::ios::app );



    file<< group.getId()
        << ","
        << group.getName()
        << "\n";
}
std::vector<Group>
FileGroupRepository::getAllGroups()
{
    std::vector<Group> groups;

    std::ifstream file(
        "../data/groups.txt"
    );

    std::string line;

    while(std::getline(file,line))
    {
        std::stringstream ss(line);

        std::string idStr;
        std::string name;

        std::getline(
            ss,
            idStr,
            ','
        );

        std::getline(
            ss,
            name
        );

        groups.emplace_back(
            std::stoi(idStr),
            name
        );
    }
    return groups;
}