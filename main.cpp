#include <libssh/libssh.h>
#include <iostream>
#include "UILayer/IOHandler.h"
#include "LogicLayer/SyncWrapper.h"
#include "LogicLayer/NetworkLogic.h"
#include "Tests/LexiTests/TestHandler.h"




int main(int argc, const char * argv[]) {
    // TestHandler().test_local_file_ops();
    // return 0;
    TestHandler().test_network_connection();
    return 0;
    string ret_msg;
    string box_color = "orange";

    SyncWrapper *syncWrapper = new SyncWrapper;

    bool is_valid = IOHandler::is_network_command(argc, argv);
    CommandModel *networkCommandModel = IOHandler::is_network_command(argc, argv);
    bool is_help_command = IOHandler::is_help_command(argc, argv);

    cout << "is_valid: " << is_valid << endl;
    cout << "is_network_command: " << networkCommandModel << endl;
    cout << "is_help_command: " << is_help_command << endl;
    
    if(networkCommandModel) {
        ret_msg = syncWrapper->sync_with_remote(networkCommandModel);
        
        // This might break since the NetworkLogic may or may not be initiated
        syncWrapper->networkLogic->list_remote_directory(networkCommandModel);

        if (!ret_msg.empty() && ret_msg[0] == '!') {
            ret_msg.erase(0, 1);
            box_color = "pink";
        }    
    } 
    else if(is_help_command) {
        ret_msg = "The help page should be displayed here.";
        IOHandler::display_help_page();
        return 0;
    } 
    else {
        ret_msg = "Hmm, this is neither help or network command, maybe try the help pages?";
        box_color = "purple";
    }
    
    IOHandler::output_title(ret_msg, box_color);

    return 0;
}

