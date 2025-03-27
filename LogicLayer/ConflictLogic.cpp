#include "ConflictLogic.h"
#include <filesystem>
#include <unordered_map>

void ConflictLogic::mark_conlicting_files(DataModel *dataModel){
    unordered_map<string, FileModel*>  local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*>  remote_files = dataModel->get_remote_files();
    bool conflict;
    for (const auto& [local_filename, local_file_model]  : local_files) {
        conflict = false;
        //std::cout << pair.first << " -> " << pair.second << std::endl;
        if (remote_files.count(local_filename) != 0){
            FileModel* remote_file_model = remote_files[local_filename];
            conflict = this->_is_conflict(local_file_model, remote_file_model);
            local_file_model->set_conflict_bool(conflict);
            remote_file_model->set_conflict_bool(conflict);
        }
    }
}

// Option 1
void ConflictLogic::overide_with_newer_file(FileModel *local_file, FileModel *remote_file){

}

// Option 2
void ConflictLogic::overide_with_older_file(FileModel *local_file, FileModel *remote_file){

}

// Option 3, I know this is kind of insane whoops
void ConflictLogic::keep_both_files_auto_rename(DataModel *dataModel, FileModel *local_file, 
                                                FileModel *remote_file, SftpSessionModel * sftpSessionModel){
    unordered_map<string, FileModel*>  local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*>  remote_files = dataModel->get_remote_files();

    string remote_file_path = remote_file->get_remote_path() + "/" + remote_file->get_name();
    string new_remote_name = remote_file->get_name() + "_r";
    while (local_files.count(new_remote_name) != 0 && remote_files.count(new_remote_name) != 0)
        new_remote_name +="_r";
    string new_remote_file_path = remote_file->get_remote_path() + "/" + new_remote_name;

    string local_file_path = local_file->get_path() + "/" + local_file->get_name();
    string new_local_name = local_file->get_name() + "_l";
    while (local_files.count(new_local_name) != 0 && remote_files.count(new_local_name) != 0)
        new_local_name +="_l";
    string new_local_file_path = local_file->get_path() + "/" + new_local_name;

    int rc = sftp_rename(sftpSessionModel->get(), remote_file_path.c_str(), new_remote_file_path.c_str());
    if (rc < 0) 
        throw std::runtime_error("Failed to rename remote file.");

    std::filesystem::rename(local_file_path, new_local_name);

    local_file->set_name(new_local_name);
    local_file->set_can_sync(true);
    local_file->set_conflict_bool(false);
    
    remote_file->set_name(new_remote_name);
    remote_file->set_can_sync(true);
    remote_file->set_conflict_bool(false);
}

// Option 4
void ConflictLogic::omit_from_sync(FileModel *local_file, FileModel *remote_file){
    local_file->set_can_sync(false);
    local_file->set_conflict_bool(false);

    remote_file->set_can_sync(false);
    remote_file->set_conflict_bool(false);

}

void ConflictLogic::apply_to_rest(void (ConflictLogic::*func)(DataModel*), DataModel* dataModel){

}



bool ConflictLogic::_is_conflict(FileModel *local_file, FileModel *remote_file){
    // later replace with md5 checksum
    if (local_file->get_size() == remote_file->get_size()) return true;
    return false;
}

