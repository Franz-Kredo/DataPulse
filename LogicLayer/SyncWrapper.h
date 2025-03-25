

#ifndef SYNCWRAPPER_H
#define SYNCWRAPPER_H

#include "DataLogic.h"
#include <string>

using namespace std;


class SyncWrapper{
public:
    DataLogic *dataLogic;
    
    SyncWrapper();

    //---- Methods ----//
    string sync_with_remote(string fucking_change_me_to_command_model);
};

#endif // SYNCWRAPPER_H