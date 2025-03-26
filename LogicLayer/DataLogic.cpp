#include "DataLogic.h"


DataLogic::DataLogic(){
    
}


//================================================================//
//=============== PUBLIC READING & WRITING METHODS ===============//
//================================================================//
DataModel *DataLogic::read_data(CommandModel *commandModel){
    unordered_map<string, FileModel> *local_files = this->read_local(commandModel); 
    unordered_map<string, FileModel> *remote_files = this->read_remote(commandModel); 
    
    // Throw messages to main to print more info for user
    if (local_files == nullptr && remote_files == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading both local and remote files.");
    if (local_files == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading local files.");
    if (remote_files == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading remote files.");

    DataModel *dataModel = new DataModel();

    dataModel->add_local_files(*local_files);
    dataModel->add_remote_files(*remote_files);
    
    dataModel = this->mark_syncable_files(dataModel);
    
    return dataModel;
}

PlaceholderModel *DataLogic::write_data(PlaceholderModel *dataModel){
    PlaceholderModel *write_locally = this->write_local(dataModel); 
    PlaceholderModel *write_remote = this->write_remote(dataModel); 
    
    
    // Throw messages to main to print more info for user
    if (write_locally == nullptr && write_remote == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed writing both local and remote files.");
    if (write_locally == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed writing to local files.");
    if (write_remote == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed writing to remote files.");

    return nullptr;
}



//===============================================================//
//=================== PRIVATE READING METHODS ===================//
//===============================================================//
unordered_map<string, FileModel> *DataLogic::read_local(CommandModel *commandModel){
    return nullptr;
}

unordered_map<string, FileModel> *DataLogic::read_remote(CommandModel *commandModel){
    return nullptr;
}

DataModel *DataLogic::mark_syncable_files(DataModel *dataModel){
    return dataModel;
}



//===============================================================//
//=================== PRIVATE WRITING METHODS ===================//
//===============================================================//
PlaceholderModel *DataLogic::write_local(PlaceholderModel *dataModel){
    return nullptr;
}

PlaceholderModel *DataLogic::write_remote(PlaceholderModel *dataModel){
    return nullptr;
}



