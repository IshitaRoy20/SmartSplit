#include "Menu.h"

#include <iostream>
#include <limits>

void Menu::run()
{
    int choice;

    while(true)
    {
        std::cout
            << "\n=================================\n"
            << "          SMARTSPLIT\n"
            << "=================================\n"
            << " GROUP MANAGEMENT\n"
            << "---------------------------------\n"
            << "1. Create Group\n"
            << "2. List Groups\n\n"
            << " USER MANAGEMENT\n"
            << "---------------------------------\n"
            << "3. Create User\n"
            << "4. List Users\n\n"
            << " SYSTEM\n"
            << "---------------------------------\n"
            << "5. Exit\n"
            << "=================================\n"
            << "Enter Choice: ";

        std::cin >> choice;

        switch(choice)
        {
            case 1:
                createGroup();
                break;

            case 2:
                listGroups();
                break;

            case 3:
                createUser();
                break;

            case 4:
                listUsers();
                break;

            case 5:
                std::cout
                    << "\nThank you for using SmartSplit!\n";
                return;

            default:
                std::cout
                    << "\nInvalid Choice.\n";
        }
    }
}

void Menu::createGroup()
{
    std::string name;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::cout
        << "\nEnter Group Name: ";

    std::getline(
        std::cin,
        name
    );

    groupService.createGroup(name);

    std::cout
        << "\nGroup Created Successfully.\n";
}

void Menu::listGroups()
{
    std::cout
        << "\n===== GROUPS =====\n";

    groupService.listGroups();
}

void Menu::createUser()
{
    std::string name;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::cout
        << "\nEnter User Name: ";

    std::getline(
        std::cin,
        name
    );

    userService.createUser(name);

    std::cout
        << "\nUser Created Successfully.\n";
}

void Menu::listUsers()
{
    std::cout
        << "\n===== USERS =====\n";

    userService.listUsers();
}