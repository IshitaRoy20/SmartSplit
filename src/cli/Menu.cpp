#include "../include/cli/Menu.h"
#include "../include/utilities/ANSIColor.h"
#include "../include/utilities/InputValidator.h"
#include <set>
#include <iostream>

int Menu::getMainMenuChoice()
{
    Colors::separator();
    std::cout << Colors::format("SMARTSPLIT - Expense Manager", Colors::BRIGHT_CYAN) << std::endl;
    Colors::separator();
    std::cout << "\n  1. Create Group\n";
    std::cout << "  2. Manage Group\n";
    std::cout << "  3. View Groups\n";
    std::cout << "  4. Exit\n\n";

    return InputValidator::getIntInRange("Enter choice (1-4): ", 1, 4);
}

int Menu::getGroupMenuChoice()
{
    Colors::separator();
    std::cout << Colors::format("GROUP WORKSPACE", Colors::BRIGHT_CYAN) << std::endl;
    Colors::separator();
    
    std::cout << "\n  MEMBERS\n";
    std::cout << "  1. Add Member\n";
    std::cout << "  2. View Members\n";
    std::cout << "  3. Remove Member\n";
    
    std::cout << "\n  EXPENSES\n";
    std::cout << "  4. Add Expense\n";
    std::cout << "  5. View Expenses\n";
    std::cout << "  6. View Details\n";
    
    std::cout << "\n  REPORTS\n";
    std::cout << "  7. View Settlements\n";
    std::cout << "  8. Dashboard\n";
    
    std::cout << "\n  ACTIONS\n";
    std::cout << "  9. Delete Expense\n";
    std::cout << "  10. Exit\n\n";

    return InputValidator::getIntInRange("Enter choice (1-10): ", 1, 10);
}

bool Menu::isValidGroup(int groupId, const std::vector<Group>& groups)
{
    for(const auto& group : groups)
    {
        if(group.getId() == groupId)
            return true;
    }
    return false;
}

bool Menu::isValidGroupMember(int memberId, const std::vector<Member>& groupMembers)
{
    for(const auto& member : groupMembers)
    {
        if(member.getId() == memberId)
            return true;
    }
    return false;
}

bool Menu::isValidExpense(int expenseId, const std::vector<Expense>& groupExpenses)
{
    for(const auto& expense : groupExpenses)
    {
        if(expense.getId() == expenseId)
            return true;
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
    Colors::header("\nCreate Group");

    std::string name = InputValidator::getValidString(
        "Group name: ",
        "Name cannot be empty."
    );

    groupService.createGroup(name);
    Colors::success("Group created successfully!");
    std::cout << "\n";
}

void Menu::listGroups()
{
    Colors::header("\nYour Groups");
    groupService.listGroups();
    std::cout << "\n";
}

void Menu::manageGroup()
{
    auto groups = groupService.getAllGroups();

    if(groups.empty())
    {
        Colors::warning("No groups exist. Create one first.");
        std::cout << "\n";
        return;
    }

    Colors::header("\nSelect Group");
    groupService.listGroups();

    int groupId = InputValidator::getPositiveInt("Group ID: ");

    if(!isValidGroup(groupId, groups))
    {
        Colors::error("Invalid group ID.");
        std::cout << "\n";
        return;
    }

    while(true)
    {
        int choice = getGroupMenuChoice();

        switch(choice)
        {
            case 1: addMember(groupId); break;
            case 2: viewMembers(groupId); break;
            case 3: removeMember(groupId); break;
            case 4: addExpense(groupId); break;
            case 5: viewExpenses(groupId); break;
            case 6: viewExpenseDetails(groupId); break;
            case 7: viewSettlements(groupId); break;
            case 8: viewDashboard(groupId); break;
            case 9: deleteExpense(groupId); break;
            case 10: return;
        }
    }
}

void Menu::addMember(int groupId)
{
    Colors::header("\nAdd Member");

    std::string name = InputValidator::getValidString(
        "Member name: ",
        "Name cannot be empty."
    );

    if(memberService.memberExists(groupId, name))
    {
        Colors::warning("Member already exists.");
        std::cout << "\n";
        return;
    }

    memberService.addMember(groupId, name);
    Colors::success("Member added!");
    std::cout << "\n";
}

void Menu::viewMembers(int groupId)
{
    Colors::header("\nGroup Members");
    memberService.viewMembers(groupId);
    std::cout << "\n";
}

void Menu::removeMember(int groupId)
{
    auto groupMembers = memberService.getMembersByGroup(groupId);

    if(groupMembers.empty())
    {
        Colors::warning("No members in this group.");
        std::cout << "\n";
        return;
    }

    Colors::header("\nRemove Member");
    memberService.viewMembers(groupId);

    int memberId = InputValidator::getPositiveInt("Member ID: ");

    if(!isValidGroupMember(memberId, groupMembers))
    {
        Colors::error("Invalid member ID.");
        std::cout << "\n";
        return;
    }

    double balance = expenseService.getMemberBalance(groupId, memberId, groupMembers);

    if(InputValidator::hasDebt(balance))
    {
        Colors::error("Cannot remove member with unsettled balance.");
        Colors::info("Settle expenses first.");
        std::cout << "\n";
        return;
    }

    memberService.removeMember(groupId, memberId);
    Colors::success("Member removed!");
    std::cout << "\n";
}

void Menu::viewExpenses(int groupId)
{
    Colors::header("\nExpenses");
    expenseService.viewExpenses(groupId);
    std::cout << "\n";
}

void Menu::viewExpenseDetails(int groupId)
{
    auto members = memberService.getMembersByGroup(groupId);
    Colors::header("\nExpense Details");
    expenseService.viewExpenseDetails(groupId, members);
    std::cout << "\n";
}

void Menu::viewSettlements(int groupId)
{
    auto members = memberService.getMembersByGroup(groupId);
    Colors::header("\nSettlements");
    expenseService.viewSettlements(groupId, members);
    std::cout << "\n";
}

void Menu::viewDashboard(int groupId)
{
    auto members = memberService.getMembersByGroup(groupId);
    Colors::header("\nDashboard");
    expenseService.viewDashboard(groupId, members);
    std::cout << "\n";
}

void Menu::addExpense(int groupId)
{
    Colors::header("\nAdd Expense");

    auto groupMembers = memberService.getMembersByGroup(groupId);

    if(groupMembers.empty())
    {
        Colors::warning("Add members first.");
        std::cout << "\n";
        return;
    }

    std::string title = InputValidator::getValidString(
        "Description: ",
        "Description cannot be empty."
    );

    double totalAmount = InputValidator::getPositiveDouble(
        "Total amount (₹): ",
        0.0
    );

    Colors::header("Who paid?");
    memberService.viewMembers(groupId);

    int payerCount = InputValidator::getIntInRange(
        "Number of payers: ",
        1,
        static_cast<int>(groupMembers.size())
    );

    std::cout << "\n";

    double paidSum = 0;
    std::set<int> usedPayers;
    std::vector<std::pair<int, double>> pendingPayments;

    for(int i = 0; i < payerCount; i++)
    {
        Colors::info("Payer " + std::to_string(i + 1) + "/" + std::to_string(payerCount));
        
        int memberId = InputValidator::getPositiveInt("Member ID: ");

        if(!isValidGroupMember(memberId, groupMembers))
        {
            Colors::warning("Invalid ID.");
            i--;
            continue;
        }

        if(usedPayers.count(memberId))
        {
            Colors::warning("Already added.");
            i--;
            continue;
        }

        usedPayers.insert(memberId);

        double paidAmount = InputValidator::getPositiveDouble(
            "Amount paid (₹): ",
            0.0
        );

        paidSum += paidAmount;
        pendingPayments.push_back({memberId, paidAmount});

        std::cout << "Total: " << paidSum << " / " << totalAmount << "\n\n";
    }

    if(!InputValidator::isApproximatelyEqual(paidSum, totalAmount))
    {
        Colors::error("Amounts don't match.");
        Colors::info("Paid: ₹" + std::to_string(paidSum) + " | Expense: ₹" + std::to_string(totalAmount));
        std::cout << "\n";
        return;
    }

    int expenseId = expenseService.createExpense(groupId, title, totalAmount);
    
    for(const auto& payment : pendingPayments)
    {
        expenseService.addPayment(expenseId, groupId, payment.first, payment.second);
    }

    Colors::success("Expense added: ₹" + std::to_string(totalAmount) + " (" + title + ")");
    std::cout << "\n";
}

void Menu::deleteExpense(int groupId)
{
    Colors::header("\nDelete Expense");

    auto groupExpenses = expenseService.getExpensesByGroup(groupId);

    if(groupExpenses.empty())
    {
        Colors::warning("No expenses to delete.");
        std::cout << "\n";
        return;
    }

    Colors::info("Select expense:");
    std::cout << "";
    expenseService.viewExpenses(groupId);

    int expenseId = InputValidator::getPositiveInt("Expense ID: ");

    if(!isValidExpense(expenseId, groupExpenses))
    {
        Colors::error("Invalid expense ID.");
        std::cout << "\n";
        return;
    }

    expenseService.deleteExpense(groupId, expenseId);
    Colors::success("Expense deleted!");
    std::cout << "\n";
}