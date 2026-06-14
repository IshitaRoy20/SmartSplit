#pragma once

#include <string>

class User
{
private:

    int id;
    std::string name;

public:

    User() = default;

    User(
        int id,
        const std::string& name
    )
        : id(id),
          name(name)
    {}

    int getId() const
    {
        return id;
    }

    std::string getName() const
    {
        return name;
    }
};