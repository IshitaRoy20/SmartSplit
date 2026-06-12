#include "cli/Menu.h"

int main() {

    Menu menu;

    while(true) {

        menu.showMainMenu();

        int choice;

        std::cin >> choice;

        switch(choice) {

            case 1:
                menu.createGroup();
                break;

            case 2:
                menu.listGroups();
                break;

            case 3:
                return 0;

            default:
                std::cout << "Invalid Choice\n";
        }
    }
}