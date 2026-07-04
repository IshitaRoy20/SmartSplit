#include "../include/cli/Menu.h"
#include "../include/utilities/ConfigPath.h"
#include "../include/database/Database.h"
int main()
{
    Config::Path::initialize("../data");

    Database& database = Database::getInstance();

    if(!database.open())
    {
        return 1;
    }

    database.initialize();

    Menu menu;
    menu.run();

    return 0;
}