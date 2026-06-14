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
            << "1. Create Group\n"
            << "2. Manage Group\n"
            << "3. List Groups\n"
            << "4. Exit\n"
            << "=================================\n"
            << "Enter Choice: ";

        std::cin >> choice;

        switch(choice)
        {
            case 1:
                createGroup();
                break;

            case 2:
                manageGroup();
                break;

            case 3:
                listGroups();
                break;

            case 4:
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

void Menu::manageGroup()
{
    int groupId;

    std::cout
        << "\n===== AVAILABLE GROUPS =====\n";

    groupService.listGroups();

    std::cout
        << "\nEnter Group ID: ";

    std::cin >> groupId;

    int choice;

    while(true)
    {
        std::cout
            << "\n=================================\n"
            << "       GROUP WORKSPACE\n"
            << "=================================\n"
            << "1. Add Member\n"
            << "2. View Members\n"
            << "3. Remove Member\n"
            << "4. Back\n"
            << "=================================\n"
            << "Choice: ";

        std::cin >> choice;

        switch(choice)
        {
            case 1:
                addMember(groupId);
                break;

            case 2:
                viewMembers(groupId);
                break;

            case 3:
                removeMember(groupId);
                break;

            case 4:
                return;

            default:
                std::cout
                    << "\nInvalid Choice\n";
        }
    }
}

void Menu::addMember(int groupId)
{
    std::string name;

    std::cin.ignore();

    std::cout
        << "\nEnter Member Name: ";

    std::getline(
        std::cin,
        name
    );

    memberService.addMember(
        groupId,
        name
    );

    std::cout
        << "\nMember Added Successfully\n";
}

void Menu::viewMembers(int groupId)
{
    std::cout
        << "\n===== MEMBERS =====\n";

    memberService.viewMembers(
        groupId
    );
}

void Menu::removeMember(int groupId)
{
    std::cout
        << "\n===== MEMBERS =====\n";

    memberService.viewMembers(
        groupId
    );

    int memberId;

    std::cout
        << "\nEnter Member ID: ";

    std::cin >> memberId;

    memberService.removeMember(
        memberId
    );

    std::cout
        << "\nMember Removed Successfully\n";
}