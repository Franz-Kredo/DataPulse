#include "SyncWrapper.h"
#include "../UILayer/IOHandler.h"
#include "ConflictLogic.h"


SyncWrapper::SyncWrapper(CommandModel *commandModel) {
    this->commandModel = commandModel;
    this->fileLogic = new FileLogic();
    this->networkLogic = nullptr;
    this->dataLogic = nullptr;
    this->conflictLogic = nullptr;
    
    try {
        this->networkLogic = new NetworkLogic(commandModel);
        
        if (this->networkLogic && this->networkLogic->sftpSession) {
            this->dataLogic = new DataLogic(this->fileLogic, this->networkLogic);
            this->conflictLogic = new ConflictLogic(this->fileLogic, this->networkLogic, this->dataLogic);
        } else {
            cout << "Failed to establish connection." << endl;
            // Cleanup
            delete this->fileLogic;
            this->fileLogic = nullptr;
            
            delete this->networkLogic;
            this->networkLogic = nullptr;
        }
    } catch (const exception &e) {
        cout << "An error occurred while creating networkLogic: " << e.what() << endl;
        
        // Clean up any resources that were allocated
        delete this->fileLogic;
        this->fileLogic = nullptr;
        
        // Re-throw the exception
        throw;
    }
}


DataModel *SyncWrapper::initialize_files(){
        // Collect data locally and remotely
        return this->dataLogic->collect_files(this->commandModel);
}


string SyncWrapper::sync_with_remote(DataModel *dataModel){
    try {
        // Write the syncable data to local and remote
        this->dataLogic->write_data(dataModel, commandModel);
        
        bool sync_completed = this->verify_sync(dataModel);
        if(sync_completed) 
            return "Sync concluded!";
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
    DataModel *dataModel = this->dataLogic->collect_files(this->commandModel, false);
    
    bool is_synced = this->dataLogic->compare_synced_data(dataModel, this->commandModel);

    delete dataModel;

    return is_synced;
    
}
