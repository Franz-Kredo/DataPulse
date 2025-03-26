#include "SyncWrapper.h"


SyncWrapper::SyncWrapper(){
    this->dataLogic = new DataLogic;
    
}

string SyncWrapper::sync_with_remote(CommandModel *commandModel){
    // try {
    //     this->networkLogic = new NetworkLogic(commandModel);
    //     this->networkLogic->list_remote_directory(commandModel);
    // } catch (const exception &e) {
    //     cerr << "Exception: " << e.what() << endl;
    //     return "There was either ssh or sftp error within the constructor of the NetworkLogic.";
    // }

    try {
        // Existing network logic initialization
        this->networkLogic = new NetworkLogic(commandModel);
        // this->networkLogic->list_remote_directory(commandModel);

        // Try to read data locally and remotely
        DataModel *dataModel = this->dataLogic->read_data(commandModel);
        
    } 
    catch (const runtime_error &e) {
        cerr << "!Runtime error occurred: " << e.what() << endl;
        return e.what(); // Return or handle the error message as needed
    } 
    catch (const exception &e) {
        cerr << "Exception occurred: " << e.what() << endl;
        return "!An unexpected error occurred.";
    } 
    catch (...) {
        cerr << "Unknown error occurred." << endl;
        return "!Unknown error.";
    }


    return "Sync succeeded";
}
