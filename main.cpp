#include <libssh/libssh.h>
#include <iostream>
#include "UILayer/IOHandler.h"
#include "LogicLayer/SyncWrapper.h"
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
    string ret_msg;
    SyncWrapper *syncWrapper = new SyncWrapper;

    bool is_valid = IOHandler::is_network_command(argc, argv);
    bool is_network_command = IOHandler::is_network_command(argc, argv);
    bool is_help_command = IOHandler::is_help_command(argc, argv);

    cout << "is_valid: " << is_valid << endl;
    cout << "is_network_command: " << is_network_command << endl;
    cout << "is_help_command: " << is_help_command << endl;
    
    if(is_network_command){
        ret_msg = syncWrapper->sync_with_remote("This should be an instance of the CommandModel, not a string!");
        IOHandler::output_title(ret_msg);
    }


    return 0;
}

