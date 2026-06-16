#include "Menu.h"
#include <set>
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
        auto groups =
            groupService.getAllGroups();

        if(groups.empty())
        {
            std::cout
                << "\nNo Groups Exist.\n"
                << "Create a Group First.\n";

            return;
        }

        int groupId;

        std::cout
            << "\n===== AVAILABLE GROUPS =====\n";

        groupService.listGroups();

        std::cout
            << "\nEnter Group ID: ";

        std::cin >> groupId;

        bool validGroup = false;

        for(const auto& group : groups)
        {
            if(group.getId() == groupId)
            {
                validGroup = true;
                break;
            }
        }

        if(!validGroup)
        {
            std::cout
                << "\nInvalid Group ID.\n";

            return;
        }

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
            << "\n"
            << "4. Add Expense\n"
            << "5. View Expenses\n"
            << "6. View Balances\n"
            << "\n"
            << "7. Back\n"
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
                addExpense(groupId);
                break;

            case 5:
                viewExpenses(groupId);
                break;
            case 6:
                viewBalances(groupId);
                break;
            case 7:
                return;

            default:
                std::cout
                    << "\nInvalid Choice\n";
        }
    }
}
void Menu::viewExpenses(
    int groupId
)
{
    std::cout
        << "\n===== EXPENSES =====\n";

    expenseService.viewExpenses(
        groupId
    );
}
void Menu::viewBalances(
    int groupId
)
{
    expenseService.viewBalances(
        groupId,
        memberService.getAllMembers()
    );
}
void Menu::addExpense(
    int groupId
)
{
            auto allMembers =
            memberService.getAllMembers();

        int memberCount = 0;

        for(const auto& member : allMembers)
        {
            if(member.getGroupId() == groupId)
            {
                memberCount++;
            }
        }

        if(memberCount == 0)
        {
            std::cout
                << "\nNo Members Found In This Group.\n"
                << "Add Members First.\n";

            return;
        }
    std::string title;

    double totalAmount;

    int payerCount;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::cout
        << "\nExpense Title: ";

    std::getline(
        std::cin,
        title
    );

    std::cout
        << "Total Expense Amount: ";

    std::cin >> totalAmount;

    std::cout
        << "\n===== MEMBERS =====\n";

    memberService.viewMembers(
        groupId
    );

    std::cout
        << "\nNumber Of Payers: ";

    std::cin >> payerCount;

            if(payerCount > memberCount)
        {
            std::cout
                << "\nInvalid Payer Count.\n"
                << "Only "
                << memberCount
                << " Member(s) Exist.\n";

            return;
        }

        if(payerCount <= 0)
        {
            std::cout
                << "\nPayer Count Must Be Positive.\n";

            return;
        }

       double paidSum = 0;

std::set<int> usedPayers;

std::vector<std::pair<int,double>>
pendingPayments;

for(int i=0; i<payerCount; i++)
{
    int memberId;

    std::cout
        << "\nMember ID: ";

    std::cin >> memberId;

    bool validMember = false;

    for(const auto& member : allMembers)
    {
        if(member.getGroupId() == groupId &&
           member.getId() == memberId)
        {
            validMember = true;
            break;
        }
    }

    if(!validMember)
    {
        std::cout
            << "\nInvalid Member ID.\n";

        i--;
        continue;
    }

    if(usedPayers.count(memberId))
    {
        std::cout
            << "\nMember Already Added As Payer.\n";

        i--;
        continue;
    }

    usedPayers.insert(memberId);

    double paidAmount;

    std::cout
        << "Amount Paid: ";

    std::cin >> paidAmount;

    if(paidAmount <= 0)
    {
        std::cout
            << "\nAmount Must Be Greater Than Zero.\n";

        usedPayers.erase(memberId);

        i--;
        continue;
    }

    paidSum += paidAmount;

    pendingPayments.push_back(
    {
        memberId,
        paidAmount
    });
}

        if(paidSum != totalAmount)
        {
            std::cout
                << "\nTransaction Cancelled.\n"
                << "Total Paid Does Not Match Expense Amount.\n";

            return;
        }
        int expenseId =
    expenseService.createExpense(
        groupId,
        title,
        totalAmount
        );

        for(const auto& payment :
            pendingPayments)
        {
            expenseService.addPayment(
                expenseId,
                payment.first,
                payment.second
            );
        }

    std::cout
        << "\nExpense Added Successfully\n";
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
    auto allMembers =
        memberService.getAllMembers();

    bool groupHasMembers = false;

    for(const auto& member : allMembers)
    {
        if(member.getGroupId() == groupId)
        {
            groupHasMembers = true;
            break;
        }
    }

    if(!groupHasMembers)
    {
        std::cout
            << "\nNo Members Found In This Group.\n";

        return;
    }

    std::cout
        << "\n===== MEMBERS =====\n";

    memberService.viewMembers(
        groupId
    );

    int memberId;

    std::cout
        << "\nEnter Member ID: ";

    std::cin >> memberId;

    bool validMember = false;

    for(const auto& member : allMembers)
    {
        if(member.getGroupId() == groupId &&
           member.getId() == memberId)
        {
            validMember = true;
            break;
        }
    }

    if(!validMember)
    {
        std::cout
            << "\nInvalid Member ID.\n";

        return;
    }

    memberService.removeMember(
        memberId
    );

    std::cout
        << "\nMember Removed Successfully\n";
}