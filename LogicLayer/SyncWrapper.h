

#ifndef SYNCWRAPPER_H
#define SYNCWRAPPER_H

#include "../Models/CommandModel.h"

#include "DataLogic.h"
#include "NetworkLogic.h"
#include "FileLogic.h"

#include <string>

#include "../Models/PlaceholderModel.h" // Until FileModel and maybe DataModel is ready

using namespace std;


class SyncWrapper{
public:
    CommandModel *commandModel;
    DataLogic *dataLogic;
    NetworkLogic *networkLogic;
    FileLogic *fileLogic;
    
    SyncWrapper(CommandModel *commandModel);

    //---- Methods ----//
    string sync_with_remote();
};

#endif // SYNCWRAPPER_H