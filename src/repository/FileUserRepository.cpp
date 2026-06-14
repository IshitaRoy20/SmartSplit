#include "FileUserRepository.h"

#include <fstream>
#include <sstream>

void FileUserRepository::saveUser(
    const User& user
)
{
    std::ofstream file(
        "../data/users.txt",
        std::ios::app
    );

    file
        << user.getId()
        << ","
        << user.getName()
        << "\n";
}

std::vector<User>
FileUserRepository::getAllUsers()
{
    std::vector<User> users;

    std::ifstream file(
        "../data/users.txt"
    );

    std::string line;

    while(std::getline(file,line))
    {
        std::stringstream ss(line);

        std::string idStr;
        std::string name;

        std::getline(ss,idStr,',');
        std::getline(ss,name);

        users.emplace_back(
            std::stoi(idStr),
            name
        );
    }

    return users;
}