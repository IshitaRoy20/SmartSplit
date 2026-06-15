#include "Menu.h"

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
    int groupId;

    std::cout
        << "\n===== AVAILABLE GROUPS =====\n";

    groupService.listGroups();

    std::cout
        << "\nEnter Group ID: ";

    std::cin >> groupId;

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
            << "\n"
            << "6. Back\n"
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
void Menu::addExpense(
    int groupId
)
{
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

    int expenseId =
        expenseService.createExpense(
            groupId,
            title,
            totalAmount
        );

    std::cout
        << "\n===== MEMBERS =====\n";

    memberService.viewMembers(
        groupId
    );

    std::cout
        << "\nNumber Of Payers: ";

    std::cin >> payerCount;

    double paidSum = 0;

    for(int i=0; i<payerCount; i++)
    {
        int memberId;

        double paidAmount;

        std::cout
            << "\nMember ID: ";

        std::cin >> memberId;

        std::cout
            << "Amount Paid: ";

        std::cin >> paidAmount;

        paidSum += paidAmount;

        expenseService.addPayment(
            expenseId,
            memberId,
            paidAmount
        );
    }

    if(paidSum != totalAmount)
    {
        std::cout
            << "\nWARNING:\n"
            << "Total Paid = "
            << paidSum
            << "\nExpense Amount = "
            << totalAmount
            << "\n";
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
    std::cout
        << "\n===== MEMBERS =====\n";

    memberService.viewMembers(
        groupId
    );

    int memberId;

    std::cout
        << "\nEnter Member ID: ";

    std::cin >> memberId;

    memberService.removeMember(
        memberId
    );

    std::cout
        << "\nMember Removed Successfully\n";
}