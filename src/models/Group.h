#pragma once

#include <string>
#include <vector>

class Group {
private:
    int groupId;
    std::string groupName;

public:
    Group(int id, std::string name)
        : groupId(id), groupName(name) {}

    int getId() const {
        return groupId;
    }

    std::string getName() const {
        return groupName;
    }
};