#include "DataLogic.h"


DataLogic::DataLogic(){
    
}


//================================================================//
//=============== PUBLIC READING & WRITING METHODS ===============//
//================================================================//
PlaceholderModel *DataLogic::read_data(CommandModel *commandModel){
    PlaceholderModel *read_locally = this->read_local(commandModel); 
    PlaceholderModel *read_remote = this->read_remote(commandModel); 
    
    // Throw messages to main to print more info for user
    if (read_locally == nullptr && read_remote == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading both local and remote files.");
    if (read_locally == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading local files.");
    if (read_remote == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading remote files.");

    //TODO Here we will generate the DataModel and return it
    return nullptr;
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
PlaceholderModel *DataLogic::read_local(CommandModel *commandModel){
    return nullptr;
}

PlaceholderModel *DataLogic::read_remote(CommandModel *commandModel){
    return nullptr;
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


