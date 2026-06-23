#include "../include/cli/Menu.h"
#include "../include/utilities/ANSIColor.h"
#include "../include/utilities/InputValidator.h"
#include <set>
#include <iostream>

int Menu::getMainMenuChoice()
{
    Colors::separator();
    std::cout << Colors::format("          SMARTSPLIT", Colors::BRIGHT_CYAN) << std::endl;
    Colors::separator();
    std::cout << Colors::format("1. Create Group", Colors::GREEN) << std::endl;
    std::cout << Colors::format("2. Manage Group", Colors::CYAN) << std::endl;
    std::cout << Colors::format("3. List Groups", Colors::BLUE) << std::endl;
    std::cout << Colors::format("4. Exit", Colors::RED) << std::endl;
    Colors::separator();

    return InputValidator::getIntInRange("Enter Choice: ", 1, 4);
}

int Menu::getGroupMenuChoice()
{
    Colors::separator();
    std::cout << Colors::format("       GROUP WORKSPACE", Colors::BRIGHT_CYAN) << std::endl;
    Colors::separator();
    std::cout << Colors::format("1. Add Member", Colors::GREEN) << std::endl;
    std::cout << Colors::format("2. View Members", Colors::CYAN) << std::endl;
    std::cout << Colors::format("3. Remove Member", Colors::RED) << std::endl;
    std::cout << std::endl;
    std::cout << Colors::format("4. Add Expense", Colors::GREEN) << std::endl;
    std::cout << Colors::format("5. View Expenses", Colors::CYAN) << std::endl;
    std::cout << Colors::format("6. View Expense Details", Colors::CYAN) << std::endl;
    std::cout << std::endl;
    std::cout << Colors::format("7. View Settlements", Colors::YELLOW) << std::endl;
    std::cout << Colors::format("8. View Dashboard", Colors::YELLOW) << std::endl;
    std::cout << std::endl;
    std::cout << Colors::format("9. Delete Expense", Colors::RED) << std::endl;
    std::cout << Colors::format("10. Exit Group Workspace", Colors::RED) << std::endl;
    Colors::separator();

    return InputValidator::getIntInRange("Choice: ", 1, 10);
}

bool Menu::isValidGroup(int groupId, const std::vector<Group>& groups)
{
    for(const auto& group : groups)
    {
        if(group.getId() == groupId)
        {
            return true;
        }
    }
    return false;
}

bool Menu::isValidGroupMember(
    int groupId,
    int memberId,
    const std::vector<Member>& allMembers
)
{
    for(const auto& member : allMembers)
    {
        if(member.getGroupId() == groupId && member.getId() == memberId)
        {
            return true;
        }
    }
    return false;
}

std::vector<Member> Menu::getGroupMembers(
    int groupId,
    const std::vector<Member>& allMembers
)
{
    std::vector<Member> groupMembers;
    for(const auto& member : allMembers)
    {
        if(member.getGroupId() == groupId)
        {
            groupMembers.push_back(member);
        }
    }
    return groupMembers;
}

bool Menu::isValidExpense(
    int groupId,
    int expenseId,
    const std::vector<Expense>& allExpenses
)
{
    for(const auto& expense : allExpenses)
    {
        if(expense.getGroupId() == groupId && expense.getId() == expenseId)
        {
            return true;
        }
    }
    return false;
}

void Menu::run()
{
    while(true)
    {
        int choice = getMainMenuChoice();

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
                Colors::success("Thank you for using SmartSplit!");
                return;
        }
    }
}

void Menu::createGroup()
{
    Colors::header("\n📋 CREATE GROUP");

    std::string name = InputValidator::getValidString(
        "Enter Group Name: ",
        "Group Name Cannot Be Empty."
    );

    groupService.createGroup(name);
    Colors::success("Group Created Successfully!");
}

void Menu::listGroups()
{
    Colors::header("\n📋 GROUPS");
    groupService.listGroups();
}

void Menu::manageGroup()
{
    auto groups = groupService.getAllGroups();

    if(groups.empty())
    {
        Colors::warning("No Groups Exist. Create a Group First.");
        return;
    }

    Colors::header("\n📋 AVAILABLE GROUPS");
    groupService.listGroups();

    int groupId = InputValidator::getPositiveInt("Enter Group ID: ");

    if(!isValidGroup(groupId, groups))
    {
        Colors::error("Invalid Group ID.");
        return;
    }

   while(true)
    {
        int choice = getGroupMenuChoice();

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
                addExpense(groupId);
                break;

            case 5:
                viewExpenses(groupId);
                break;

            case 6:
                viewExpenseDetails(groupId);
                break;

            case 7:
                viewSettlements(groupId);
                break;

            case 8:
                viewDashboard(groupId);
                break;

            case 9:
                deleteExpense(groupId);
                break;

            case 10:
                return;
        }
    }
}

void Menu::addMember(int groupId)
{
    Colors::header("\n👤 ADD MEMBER");

    std::string name = InputValidator::getValidString(
        "Enter Member Name: ",
        "Member Name Cannot Be Empty."
    );

    if(memberService.memberExists(groupId, name))
    {
        Colors::warning("Member Already Exists.");
        return;
    }

    memberService.addMember(groupId, name);
    Colors::success("Member Added Successfully!");
}

void Menu::viewMembers(int groupId)
{
    Colors::header("\n👤 MEMBERS");
    memberService.viewMembers(groupId);
}

void Menu::removeMember(int groupId)
{
    auto allMembers = memberService.getAllMembers();

    std::vector<Member> groupMembers = getGroupMembers(groupId, allMembers);

    if(groupMembers.empty())
    {
        Colors::warning("No Members Found In This Group.");
        return;
    }

    Colors::header("\n👤 MEMBERS");
    memberService.viewMembers(groupId);

    int memberId = InputValidator::getPositiveInt("Enter Member ID: ");

    if(!isValidGroupMember(groupId, memberId, allMembers))
    {
        Colors::error("Invalid Member ID.");
        return;
    }

    double balance = expenseService.getMemberBalance(
        groupId,
        memberId,
        allMembers
    );

    if(InputValidator::hasDebt(balance))
    {
        Colors::error(
            "Cannot Remove Member. Member Has Pending Debt. "
            "Settle Balances First."
        );
        return;
    }

    memberService.removeMember(memberId);
    Colors::success("Member Removed Successfully!");
}

void Menu::viewExpenses(int groupId)
{
    Colors::header("\n💰 EXPENSES");
    expenseService.viewExpenses(groupId);
}

void Menu::viewExpenseDetails(int groupId)
{
    auto members = memberService.getAllMembers();
    Colors::header("\n💰 EXPENSE DETAILS");
    expenseService.viewExpenseDetails(groupId, members);
}

void Menu::viewSettlements(int groupId)
{
    auto members = memberService.getAllMembers();
    Colors::header("\n🔄 SETTLEMENTS");
    expenseService.viewSettlements(groupId, members);
}

void Menu::viewDashboard(int groupId)
{
    auto members = memberService.getAllMembers();
    Colors::header("\n📊 DASHBOARD");
    expenseService.viewDashboard(groupId, members);
}

void Menu::addExpense(int groupId)
{
    Colors::header("\n💰 ADD EXPENSE");

    auto allMembers = memberService.getAllMembers();
    std::vector<Member> groupMembers = getGroupMembers(groupId, allMembers);

    if(groupMembers.empty())
    {
        Colors::warning("No Members Found In This Group. Add Members First.");
        return;
    }

    // Get expense details
    std::string title = InputValidator::getValidString(
        "Expense Title: ",
        "Expense Title Cannot Be Empty."
    );

    double totalAmount = InputValidator::getPositiveDouble(
        "Total Expense Amount: ",
        0.0
    );

    Colors::header("\n👤 MEMBERS");
    memberService.viewMembers(groupId);

    int payerCount = InputValidator::getIntInRange(
        "Number Of Payers: ",
        1,
        groupMembers.size()
    );

    double paidSum = 0;
    std::set<int> usedPayers;
    std::vector<std::pair<int, double>> pendingPayments;

    for(int i = 0; i < payerCount; i++)
    {
        int memberId = InputValidator::getPositiveInt("Member ID: ");

        if(!isValidGroupMember(groupId, memberId, allMembers))
        {
            Colors::warning("Invalid Member ID. Try again.");
            i--;
            continue;
        }
        if(usedPayers.count(memberId))
        {
            Colors::warning("Member Already Added As Payer. Try again.");
            i--;
            continue;
        }

        usedPayers.insert(memberId);

        // Get amount
        double paidAmount = InputValidator::getPositiveDouble(
            "Amount Paid: ",
            0.0
        );

        paidSum += paidAmount;
        pendingPayments.push_back({memberId, paidAmount});

        Colors::info(
            "Running Total: " + std::to_string(paidSum) + " / " + 
            std::to_string(totalAmount)
        );
    }

    if(!InputValidator::isApproximatelyEqual(paidSum, totalAmount))
    {
        Colors::error(
            "Transaction Cancelled. Total Paid (" + std::to_string(paidSum) +
            ") Does Not Match Expense Amount (" + std::to_string(totalAmount) + ")."
        );
        return;
    }

    int expenseId = expenseService.createExpense(
        groupId,
        title,
        totalAmount
    );
    for(const auto& payment : pendingPayments)
    {
        expenseService.addPayment(
            expenseId,
            payment.first,
            payment.second
        );
    }

    Colors::success("Expense Added Successfully!");
}

void Menu::deleteExpense(int groupId)
{
    Colors::header("\n🗑️  DELETE EXPENSE");

    auto expenses = expenseService.getAllExpenses();
    std::vector<Expense> groupExpenses;
    for(const auto& expense : expenses)
    {
        if(expense.getGroupId() == groupId)
        {
            groupExpenses.push_back(expense);
        }
    }

    if(groupExpenses.empty())
    {
        Colors::warning("No Expenses Found In This Group.");
        return;
    }

    Colors::header("\n💰 EXPENSES");
    expenseService.viewExpenses(groupId);

    int expenseId = InputValidator::getPositiveInt("Enter Expense ID: ");

    if(!isValidExpense(groupId, expenseId, expenses))
    {
        Colors::error("Invalid Expense ID.");
        return;
    }

    expenseService.deleteExpense(expenseId);
    Colors::success("Expense Deleted Successfully!");
}