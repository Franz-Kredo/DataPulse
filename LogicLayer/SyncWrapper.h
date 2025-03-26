

#ifndef SYNCWRAPPER_H
#define SYNCWRAPPER_H

#include "../Models/CommandModel.h"

#include "DataLogic.h"
#include "NetworkLogic.h"

#include <string>

#include "../Models/PlaceholderModel.h" // Until FileModel and maybe DataModel is ready

using namespace std;


class SyncWrapper{
public:
    DataLogic *dataLogic;
    NetworkLogic *networkLogic;
    
    SyncWrapper();

    //---- Methods ----//
    string sync_with_remote(CommandModel *commandModel);
};

#endif // SYNCWRAPPER_H