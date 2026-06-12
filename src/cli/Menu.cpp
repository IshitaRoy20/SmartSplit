#include "Menu.h"

#include <iostream>

void Menu::showMainMenu() {

    std::cout << "\n=========================\n";
    std::cout << "      SMARTSPLIT\n";
    std::cout << "=========================\n";

    std::cout << "1. Create Group\n";
    std::cout << "2. List Groups\n";
    std::cout << "3. Exit\n";

    std::cout << "\nChoice: ";
}

void Menu::createGroup() {

    int id;
    std::string name;

    std::cout << "Enter Group ID: ";
    std::cin >> id;

    std::cin.ignore();

    std::cout << "Enter Group Name: ";
    std::getline(std::cin, name);

    groupService.createGroup(id, name);

    std::cout << "\nGroup Created Successfully\n";
}

void Menu::listGroups() {
    groupService.listGroups();
}