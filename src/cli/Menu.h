#pragma once

#include "../services/GroupService.h"
#include "../services/UserService.h"

class Menu
{
private:

    GroupService groupService;
    UserService userService;

public:

    void run();

    void createGroup();
    void listGroups();

    void createUser();
    void listUsers();
};