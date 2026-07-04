#pragma once

#include <sqlite3.h>
#include <string>
#include <mutex>

class Database
{
private:

    sqlite3* db;

    std::string databasePath;

    std::mutex dbMutex;

    Database();

public:

    static Database& getInstance();

    Database(const Database&) = delete;

    Database& operator=(const Database&) = delete;

    ~Database();

    bool open();

    void close();

    sqlite3* getConnection();

    std::mutex& getMutex();

    void initialize();
};
