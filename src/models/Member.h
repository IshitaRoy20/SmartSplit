#pragma once

#include <string>

class Member
{
private:

    int id;
    int groupId;
    std::string name;

public:

    Member() = default;

    Member(
        int id,
        int groupId,
        const std::string& name
    )
        : id(id),
          groupId(groupId),
          name(name)
    {}

    int getId() const
    {
        return id;
    }

    int getGroupId() const
    {
        return groupId;
    }

    std::string getName() const
    {
        return name;
    }
};