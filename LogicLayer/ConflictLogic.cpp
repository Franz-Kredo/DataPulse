#include "ConflictLogic.h"
#include <unordered_map>

void ConflictLogic::mark_conlicting_files(DataModel *dataModel){
    unordered_map<string, FileModel*>  local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*>  remote_files = dataModel->get_remote_files();

}

void ConflictLogic::overide_with_newer_file(DataModel *dataModel){}
void ConflictLogic::overide_with_older_file(DataModel *dataModel){}
void ConflictLogic::keep_both_files_auto_rename(DataModel *dataModel){}
void ConflictLogic::omit_from_sync(DataModel *dataModel){}
void ConflictLogic::apply_to_rest(void (ConflictLogic::*func)(DataModel*), DataModel* dataModel){}



bool ConflictLogic::_is_conflict(FileModel *local_file, FileModel *remote_file){
    return false; // lol sorry if this messed with you placeholder for linter
}

