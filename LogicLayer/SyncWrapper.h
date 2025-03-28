

#ifndef SYNCWRAPPER_H
#define SYNCWRAPPER_H

#include "../Models/CommandModel.h"

#include "ConflictLogic.h"
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
    ConflictLogic *conflictLogic;
    
    SyncWrapper(CommandModel *commandModel);

    //---- Methods ----//
    DataModel *sync_and_resolve_conflict();
    string sync_with_remote(DataModel *dataModel);
};

#endif // SYNCWRAPPER_H
