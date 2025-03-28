#include "SyncWrapper.h"
#include "../UILayer/IOHandler.h"

SyncWrapper::SyncWrapper(CommandModel *commandModel){
    this->commandModel = commandModel;
    this->fileLogic = new FileLogic();
    
    try{ this->networkLogic = new NetworkLogic(commandModel); }
    catch (...) { cout << "An error occurred while creating networkLogic." << endl; } 


    this->dataLogic = new DataLogic(this->fileLogic, this->networkLogic);
}

string SyncWrapper::sync_with_remote(){
    try {
        // Collect data locally and remotely
        DataModel *dataModel = this->dataLogic->collect_files(this->commandModel);

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
