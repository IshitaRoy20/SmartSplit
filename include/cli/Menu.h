#pragma once

#include "../services/GroupService.h"
#include "../services/MemberService.h"
#include "../services/ExpenseService.h"
#include "../models/Group.h"
#include "../models/Member.h"
#include <vector>

class Menu
{
private:

    GroupService groupService;
    MemberService memberService;
    ExpenseService expenseService;

    int getMainMenuChoice();

    int getGroupMenuChoice();

    bool isValidGroup(int groupId, const std::vector<Group>& groups);

    bool isValidGroupMember(
        int memberId,
        const std::vector<Member>& groupMembers
    );

    bool isValidExpense(
        int expenseId,
        const std::vector<Expense>& groupExpenses
    );

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

    void viewSettlements(int groupId);

    void viewDashboard(int groupId);
    
    void deleteExpense(int groupId);
};