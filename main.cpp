#include <iostream>

#include "UILayer/IOHandler.h"
#include "LogicLayer/SyncWrapper.h"

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
        FileModel *remote_file = remote_files[local_file->get_relative_path()];
        auto remote_it = remote_files.find(local_file->get_relative_path());
        if (remote_it == remote_files.end()) {
            continue; //TODO TESTING MEMORY LEAK 2
        }
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
        try{
            string message = syncWrapper->conflictLogic->conflict_handler(opt, dataModel, local_file, remote_file);
            cout << message << endl;
        } 
        catch (const runtime_error &e) {
            cerr << "!Runtime error occurred: " << e.what() << endl; 
            
        }
        
        catch(...){
            cout << "We fucked up in syncWrapper->conflictLogic->conflict_handler(opt, dataModel, local_file, remote_file);..." << endl;
        }        
    }
    
}


int main(int argc, const char * argv[]) {

    string ret_msg;
    string box_color = "orange";

    SyncWrapper *syncWrapper;

    CommandModel *networkCommandModel = IOHandler::is_network_command(argc, argv);


    bool is_help_command = IOHandler::is_help_command(argc, argv);
    
    if(networkCommandModel) {
        // This might break since the NetworkLogic may or may not be initiated
        try{
            syncWrapper = new SyncWrapper(networkCommandModel);
            // if(syncWrapper->networkLogic->sftpSession == nullptr){
            if (!syncWrapper->networkLogic) {
                return 0;
            }
        } 
        catch (const runtime_error &e) {
            cerr << "!Runtime error occurred: " << e.what() << endl; 
            return 0;
        } 
        catch (const exception &e) {
            cerr << "Exception occurred: " << e.what() << endl;
            return 0;
        } 
        catch (...) {
            cerr << "Unknown error occurred." << endl;
            return 0;
        }
    
        
        DataModel * dataModel= syncWrapper->initialize_files();

        // If the merge flag is on, then we resolve conflicts
        if(networkCommandModel->get_merge()) conflict_resolver(syncWrapper,dataModel);

        // Do all sync actions
        ret_msg = syncWrapper->sync_with_remote(dataModel);

        // Change color of the text box if there is ! at the start of the string
        if (!ret_msg.empty() && ret_msg[0] == '!') {
            ret_msg.erase(0, 1);
            box_color = "pink";
        }    
        delete dataModel;
    } 
    else if(is_help_command) {
        ret_msg = "The help page should be displayed here.";
        IOHandler::display_help_page();
        return 0;
    } 
    else {
        ret_msg = "Invalid Command";
        box_color = "pink";
    }
    
    IOHandler::output_title(ret_msg, box_color);


    delete syncWrapper;
    delete networkCommandModel;

    return 0;
}
