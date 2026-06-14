#pragma once

#include <string>

class Group {
private:
    int id;
    std::string name;

public:

    Group() = default;

    Group(
        int id,
        const std::string& name
    )
        : id(id),
          name(name)
    {}

    int getId() const {
        return id;
    }

    std::string getName() const {
        return name;
    }
};