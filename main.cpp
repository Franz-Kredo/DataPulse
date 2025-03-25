#include <libssh/libssh.h>
#include <iostream>
#include "UILayer/IOHandler.h"


int main(int argc, const char * argv[]) {

    // string str = "/home/franz/github_repos/DataPulse///SyncTester";
    // regex reg("(?:(?:[^\\0 !$`&*()+])|(?:\\\\(?:[ !\\$`&*\\(\\)\\+])))+");

    // if (std::regex_match(str, reg))
    //     std::cout << "The entire string matches the regex." << std::endl;
    // else
    //     std::cout << "The string does not match the regex." << std::endl;



    bool is_network_command = IOHandler::is_network_command(argc, argv);
    bool is_help_command = IOHandler::is_help_command(argc, argv);

    cout << "is_network_command: " << is_network_command << endl;
    cout << "is_help_command: " << is_help_command << endl;
    

    // if(is_help_command){
    //     cout << "Is a help command!" << endl;
    // } else {
    //     cout << "Is not a help command foe shoe!" << endl;
    // }


    return 0;
}
