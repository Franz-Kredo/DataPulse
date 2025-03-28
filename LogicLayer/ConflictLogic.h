#ifndef CONFLICT_LOGIC_H
#define CONFLICT_LOGIC_H

#include "../Models/DataModel.h"
#include "../Models/FileModel.h"
#include "FileLogic.h"
#include "NetworkLogic.h"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;

class ConflictLogic {
    FileLogic *fileLogic;
    NetworkLogic *networkLogic;

public:
    ConflictLogic(FileLogic *fileLogic, NetworkLogic *networkLogic);
    string conflict_handler(int option, DataModel *dataModel, FileModel *local_file, FileModel *remote_file); 
    //void mark_conlicting_files(DataModel *dataModel);
    vector<FileModel*> mark_conlicting_files(DataModel *dataModel);
    
    void overide_with_newer_file(DataModel *datamodel, FileModel *local_file, FileModel *remote_filel);
    
    void overide_with_older_file(DataModel *datamodel, FileModel *local_file, FileModel *remote_file);
    
    void keep_both_files_auto_rename(DataModel *dataModel, FileModel *local_file, FileModel *remote_file);
    
    void omit_from_sync(FileModel *local_file, FileModel *remote_file);
    
private:
    bool _is_conflict(FileModel *local_file, FileModel *remote_file);
    void _overide_file(bool delete_remote, DataModel *dataModel, FileModel *local_file, FileModel *remote_file);
    void _update_key_in_datmodel(string old_key, string new_key, unordered_map<string, FileModel*>& map);
};

#endif // CONFLIC_LOGIC_H
