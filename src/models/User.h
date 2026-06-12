#pragma once
#include <string>
class User {
private:
    int userId;
    std::string name;

public:
    User(int id, std::string name)
        : userId(id), name(name) {}

    int getId() const {
        return userId;
    }

    std::string getName() const {
        return name;
    }
};