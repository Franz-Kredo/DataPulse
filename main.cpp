#include <iostream>
// using std::byte;



#include <libssh/libssh.h>
#include <string>
#include <vector>
#include "Models/DataModel.h"
#include "Models/FileModel.h"
#include "UILayer/IOHandler.h"
#include "LogicLayer/SyncWrapper.h"
// #include "LogicLayer/NetworkLogic.h"
#include "Tests/LexiTests/TestHandler.h"


void conflict_resolver(SyncWrapper *syncWrapper, DataModel *dataModel){
    // have this fucker return a vector of conflicting files
    // print them and call by dict to methods if he presses sume numba autoresolve
    // basically the cin in the loop disapear swapped by an else that store the default
    // could be class variable in conflictLogic?
    int opt;
    bool apply_to_all = false;
    unordered_map<string, FileModel*>&  remote_files = dataModel->get_remote_files();
    vector<FileModel*> local_conflicts = syncWrapper->conflictLogic->mark_conlicting_files(dataModel);
    for (FileModel* local_file : local_conflicts){
        FileModel *remote_file = remote_files[local_file->get_name()];
        if(!apply_to_all){
            cout << local_file->get_name() +" Conflicting" << endl;
            cout << "\n\nChoose_how_to_resolve" << endl;
            pair <int, bool> result = IOHandler::input_conflict_resolution_option();
            opt = result.first;
            bool has_a = result.second;
            cout << "Num option: " << opt << endl;
            if (has_a) apply_to_all = true;

        }else{

        }
            syncWrapper->conflictLogic->conflict_handler(opt, dataModel, local_file, remote_file);
        

        
    }
    
}



int main(int argc, const char * argv[]) {
    // TestHandler().test_local_file_ops();
    // return 0;
    // TestHandler().test_network_connection();
    // return 0;
    string ret_msg;
    string box_color = "orange";

    SyncWrapper *syncWrapper;

    // bool is_valid = IOHandler::is_network_command(argc, argv);
    CommandModel *networkCommandModel = IOHandler::is_network_command(argc, argv);
    bool is_help_command = IOHandler::is_help_command(argc, argv);

    // cout << "is_valid: " << is_valid << endl;
    // cout << "is_network_command: " << networkCommandModel << endl;
    // cout << "is_help_command: " << is_help_command << endl;
    
    if(networkCommandModel) {
        syncWrapper = new SyncWrapper(networkCommandModel);
        DataModel * dataModel= syncWrapper->sync_and_resolve_conflict();

        conflict_resolver(syncWrapper,dataModel);
        
        ret_msg = syncWrapper->sync_with_remote(dataModel);
        
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


