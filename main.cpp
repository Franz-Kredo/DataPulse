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



    bool is_valid = IOHandler::is_network_command(argc, argv);

    if(is_valid){
        cout << "Is network command!" << endl;
    } else {
        cout << "Is not network command foe shoe!" << endl;
    }


    return 0;
}
