#include <libssh/libssh.h>
#include <iostream>
#include "UILayer/IOHandler.h"
#include "LogicLayer/NetworkLogic.h"

void test_network_connection(){
           CommandModel command;
            command.set_host("sty.frostbyte.is");
            command.set_username("alexanderme22");
            command.set_local_path("/home/lexman/");
            command.set_remote_path("/home/alexanderme22");
            command.set_priv_key_path("/home/lexman/.ssh/sty1_server");

    try {
        NetworkLogic netLogic(&command);
        netLogic.list_remote_directory(&command);
    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }


}

int main(int argc, const char * argv[]) {

    // string str = "/home/franz/github_repos/DataPulse///SyncTester";
    // regex reg("(?:(?:[^\\0 !$`&*()+])|(?:\\\\(?:[ !\\$`&*\\(\\)\\+])))+");

    // if (std::regex_match(str, reg))
    //     std::cout << "The entire string matches the regex." << std::endl;
    // else
    //     std::cout << "The string does not match the regex." << std::endl;



    bool is_valid = IOHandler::is_network_command(argc, argv);
    /*  TESTING NETWORK
            HERE \/     */
    test_network_connection();
    if(is_valid){
        cout << "Is network command!" << endl;
     } else {
        cout << "Is not network command foe shoe!" << endl;
        std::cerr << "Usage: " << argv[0] << " <host> <username> <private_key_path> <local_path> <remote_dir>" << std::endl;
    }


    return 0;
}

