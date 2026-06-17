#pragma once

#include "../services/GroupService.h"
#include "../services/MemberService.h"
#include "../services/ExpenseService.h"

class Menu
{
private:

    GroupService groupService;

    MemberService memberService;

    ExpenseService expenseService;

public:

    void run();

    void createGroup();

    void listGroups();

    void manageGroup();

    void addMember(int groupId);

    void viewMembers(int groupId);

    void removeMember(int groupId);

    void addExpense(int groupId);

    void viewExpenses(int groupId);

    void viewExpenseDetails(int groupId);

    void viewBalances(int groupId);

    void viewSettlements(int groupId);

    void viewDashboard(int groupId);
};