#include "SyncWrapper.h"
#include "../UILayer/IOHandler.h"
#include "ConflictLogic.h"


SyncWrapper::SyncWrapper(CommandModel *commandModel){
    this->commandModel = commandModel;
    this->fileLogic = new FileLogic();
    
    try{ this->networkLogic = new NetworkLogic(commandModel); }
    catch (...) { cout << "An error occurred while creating networkLogic." << endl; } 


    this->conflictLogic = new ConflictLogic(this->fileLogic, this->networkLogic);
    this->dataLogic = new DataLogic(this->fileLogic, this->networkLogic, this->conflictLogic);
}
DataModel *SyncWrapper::sync_and_resolve_conflict(){
        // this->networkLogic = new NetworkLogic(commandModel);
        // this->networkLogic->list_remote_directory(this->commandModel);
        // Collect data locally and remotely
        return this->dataLogic->collect_files(this->commandModel);
}

string SyncWrapper::sync_with_remote(DataModel *dataModel){
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
        //this->networkLogic->list_remote_directory(this->commandModel);
        // Collect data locally and remotely
        //DataModel *dataModel = this->dataLogic->collect_files(this->commandModel);


        // Write the syncable data to local and remote
        this->dataLogic->write_data(dataModel, commandModel);
        

        // dataModel = dataModel;

        // IOHandler::wait(3);
        bool sync_completed = this->verify_sync(dataModel);
        if(sync_completed) 
            return "Sync was successful!";
        else
            return "Sync verification was unsuccessful, something went wrong in syncing or verification...";

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


    return "Sync unsuccessful...";
}

bool SyncWrapper::verify_sync(DataModel *oldDataModel){
    // Collect data locally and remotely
    DataModel *dataModel = this->dataLogic->collect_files(this->commandModel);
    
    bool is_synced = this->dataLogic->compare_synced_data(dataModel, this->commandModel);

    return is_synced;
    
}
