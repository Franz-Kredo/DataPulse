

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

    ~SyncWrapper() {
        if (networkLogic) {
            delete networkLogic;
            networkLogic = nullptr;
        }
        
        if (fileLogic) {
            delete fileLogic;
            fileLogic = nullptr;
        }
        
        if (dataLogic) {
            delete dataLogic;
            dataLogic = nullptr;
        }
        
        if (conflictLogic) {
            delete conflictLogic;
            conflictLogic = nullptr;
        }
    }

    //---- Methods ----//

    /**
     * @brief Performs file synchronization between local and remote directories.
     *
     * This method coordinates the synchronization process by:
     *  - Listing the remote directory contents,
     *  - Collecting file data from both local and remote sources,
     *  - Writing updates to the respective locations as necessary.
     *
     * @return A string message indicating the outcome of the synchronization process.
     */
    string sync_with_remote(DataModel *dataModel);


    /**
     * @brief Can be used to verify a sync, making sure it properly went through
     *
     * @param oldDataModel: An instance of DataModel that was used while syncing
     * @return A boolean wether the sync was successful or not
     */
    bool verify_sync(DataModel *oldDataModel);
    
    DataModel *initialize_files();
};

#endif // SYNCWRAPPER_H
