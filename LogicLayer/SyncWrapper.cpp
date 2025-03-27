#include "SyncWrapper.h"


SyncWrapper::SyncWrapper(CommandModel *commandModel){
    this->commandModel = commandModel;
    this->fileLogic = new FileLogic();
    
    try{ this->networkLogic = new NetworkLogic(commandModel); }
    catch (...) { cout << "An error occurred while creating networkLogic." << endl; } 


    this->dataLogic = new DataLogic(this->fileLogic, this->networkLogic);
}

string SyncWrapper::sync_with_remote(){
    // try {
    //     this->networkLogic = new NetworkLogic(commandModel);
    //     this->networkLogic->list_remote_directory(commandModel);
    // } catch (const exception &e) {
    //     cerr << "Exception: " << e.what() << endl;
    //     return "There was either ssh or sftp error within the constructor of the NetworkLogic.";
    // }

    try {
        // Existing network logic initialization
        // this->networkLogic = new NetworkLogic(commandModel);
        this->networkLogic->list_remote_directory(this->commandModel);
        
        // Collect data locally and remotely
        DataModel *dataModel = this->dataLogic->collect_files(this->commandModel);

        // Write the syncable data to local and remote
        this->dataLogic->write_data(dataModel, commandModel);

        
        
        dataModel = dataModel;
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


    return "Sync finished [doesn't mean it's successful]";
}
