#pragma once

#include <sqlite3.h>
#include <string>

class Database
{
private:

    sqlite3* db;

    std::string databasePath;

public:

    Database();

    ~Database();

    bool open();

    void close();

    sqlite3* getConnection();

    void initialize();
};