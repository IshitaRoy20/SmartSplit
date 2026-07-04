#include "../../include/database/Database.h"

#include <iostream>

Database::Database()
{
    db = nullptr;

    databasePath =
        "../data/smartsplit.db";
}

Database& Database::getInstance()
{
    static Database instance;

    return instance;
}

Database::~Database()
{
    close();
}

bool Database::open()
{
    int result =
        sqlite3_open(
            databasePath.c_str(),
            &db
        );

    if(result != SQLITE_OK)
    {
        std::cout
            << "Unable to Open Database\n";

        return false;
    }

    std::cout
        << "SQLite Connected Successfully\n";

    return true;
}

void Database::close()
{
    if(db != nullptr)
    {
        sqlite3_close(db);

        db = nullptr;
    }
}

sqlite3*
Database::getConnection()
{
    return db;
}

std::mutex&
Database::getMutex()
{
    return dbMutex;
}

void Database::initialize()
{
    const char* sql = R"(

    CREATE TABLE IF NOT EXISTS groups
    (
        id INTEGER PRIMARY KEY,
        name TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS members
    (
        group_id INTEGER NOT NULL,
        id INTEGER NOT NULL,
        name TEXT NOT NULL,

        PRIMARY KEY(group_id, id),

        FOREIGN KEY(group_id)
        REFERENCES groups(id)
    );

    CREATE TABLE IF NOT EXISTS expenses
    (
        id INTEGER PRIMARY KEY,
        group_id INTEGER NOT NULL,
        title TEXT NOT NULL,
        amount REAL NOT NULL,

        FOREIGN KEY(group_id)
        REFERENCES groups(id)
    );

    CREATE TABLE IF NOT EXISTS expense_payments
    (
        expense_id INTEGER NOT NULL,
        group_id INTEGER NOT NULL,
        member_id INTEGER NOT NULL,
        amount_paid REAL NOT NULL,

        FOREIGN KEY(expense_id)
        REFERENCES expenses(id)
    );

    )";

    char* errorMessage = nullptr;

    int result =
        sqlite3_exec(
            db,
            sql,
            nullptr,
            nullptr,
            &errorMessage
        );

    if(result != SQLITE_OK)
    {
        std::cout
            << "Database Initialization Failed\n";

        std::cout
            << errorMessage
            << "\n";

        sqlite3_free(
            errorMessage
        );

        return;
    }

    std::cout
        << "Database Tables Ready\n";
}
