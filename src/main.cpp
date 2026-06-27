#include "../include/cli/Menu.h"
#include "../include/utilities/ConfigPath.h"
int main()
{
    Config::Path::initialize("../data");
    
    Menu menu;
    menu.run();
    
    return 0;
}