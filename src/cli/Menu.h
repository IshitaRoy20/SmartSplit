#pragma once

#include "../services/GroupService.h"
#include "../services/MemberService.h"

class Menu
{
private:

    GroupService groupService;
    MemberService memberService;

public:

    void run();

    void createGroup();

    void listGroups();

    void manageGroup();

   void addMember(int groupId);

void viewMembers(int groupId);

void removeMember(int groupId);
};