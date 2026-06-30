#include "../../include/database/Database.h"

#include <iostream>

Database::Database()
{
    db = nullptr;

    databasePath =
        "../data/smartsplit.db";
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

void Database::initialize()
{

}