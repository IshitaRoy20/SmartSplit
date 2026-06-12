#pragma once
#include <iostream>
#include <vector>
#include "../models/Group.h"

class GroupService {
private:
    std::vector<Group> groups;

public:
    void createGroup(int id, const std::string& name) {
        groups.emplace_back(id, name);
    }

    void listGroups() {

        if(groups.empty()) {
            std::cout << "\nNo Groups Found\n";
            return;
        }

        std::cout << "\nGroups:\n";

        for(auto& g : groups) {
            std::cout
                << g.getId()
                << ". "
                << g.getName()
                << "\n";
        }
    }
};