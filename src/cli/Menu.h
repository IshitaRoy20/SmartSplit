#pragma once

#include "../services/GroupService.h"

class Menu {
private:
    GroupService groupService;

public:
    void showMainMenu();

    void createGroup();

    void listGroups();
};