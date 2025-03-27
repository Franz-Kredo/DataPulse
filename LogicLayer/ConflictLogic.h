#ifndef CONFLICT_LOGIC_H
#define CONFLICT_LOGIC_H

#include <iostream>
#include <filesystem>
#include <cstddef>
#include <fstream>
#include "../Models/DataModel.h"
#include "../Models/FileModel.h"
#include "../Models/SftpSessionModel.h"

using namespace std;


class ConflictLogic {

public:
    ConflictLogic() = default;
    void mark_conlicting_files(DataModel *dataModel);
    void overide_with_newer_file(FileModel *local_file, FileModel *remote_filel);
    void overide_with_older_file(FileModel *local_file, FileModel *remote_file);
    void keep_both_files_auto_rename(DataModel *dataModel, FileModel *local_file, 
                                                FileModel *remote_file, SftpSessionModel * sftpSessionModel);
    void omit_from_sync(FileModel *local_file, FileModel *remote_file);

    // This is a bit racket-pilled but it's less code :D
    void apply_to_rest(void (ConflictLogic::*func)(DataModel*), DataModel* dataModel);
    // ***| Example of usage i know this is a bit much:
    // ***> ConflictLogic* logic = new ConflictLogic();
    // ***> logic->apply_to_rest(&ConflictLogic::omit_from_sync, dataModel);

private:
    bool _is_conflict(FileModel *local_file, FileModel *remote_file); 



};

#endif //CONFLIC_LOGIC_H
