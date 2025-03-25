#include "SyncWrapper.h"


SyncWrapper::SyncWrapper(){
    this->dataLogic = new DataLogic;
}

string SyncWrapper::sync_with_remote(string fucking_change_me_to_command_model){
    return "This should return a descriptive message if anything went wrong, or hopefully a success message";
}
