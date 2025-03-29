#include "ConflictLogic.h"
#include "FileLogic.h"
#include "NetworkLogic.h"
#include <filesystem>
#include <libssh/libssh.h>
#include <unordered_map>
#include <vector>


ConflictLogic::ConflictLogic(FileLogic* fileLogic, NetworkLogic *networkLogic, DataLogic *dataLogic){
    this->fileLogic = fileLogic;
    this->networkLogic = networkLogic;
    this->dataLogic = dataLogic;
}
    

string ConflictLogic::conflict_handler(int option, DataModel *dataModel, FileModel *local_file, FileModel *remote_file){
    if (option == 1){
        this->overide_with_newer_file(dataModel, local_file, remote_file);
        return "Merging... keeping newer";

    }else if (option == 2){
        this->overide_with_older_file(dataModel, local_file, remote_file);
        return "Merging... keeping older";

    }else if (option == 3){
        this->keep_both_files_auto_rename(dataModel, local_file, remote_file);
        return "Merging... renaming";
    }else if (option == 4){
        this->omit_from_sync(local_file, remote_file);
        return "Merging... skipping sync";
    }
    return "Merge failed for" + local_file->get_name();

} 


vector<FileModel*> ConflictLogic::mark_conlicting_files(DataModel *dataModel){
    unordered_map<string, FileModel*>&  local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*>&  remote_files = dataModel->get_remote_files();
    bool conflict;
    vector<FileModel*> local_file_conflicts;
    for (const auto& [local_filename, local_file_model]  : local_files) {
        conflict = false;
        if (remote_files.count(local_filename) != 0){
            FileModel* remote_file_model = remote_files[local_filename];
            conflict = this->_is_conflict(local_file_model, remote_file_model);
            local_file_model->set_conflict_bool(conflict);
            remote_file_model->set_conflict_bool(conflict);
            if (conflict) local_file_conflicts.push_back(local_file_model);
        }
    }
    return local_file_conflicts;

}


// Option 1
void ConflictLogic::overide_with_newer_file(DataModel *dataModel, FileModel *local_file, FileModel *remote_file){
    SftpSessionModel *sftpSessionModel = this->networkLogic->sftpSession;
    time_t local_time = this->fileLogic->get_local_last_modified(local_file);
    time_t remote_time = this->fileLogic->get_remote_last_modified(remote_file, sftpSessionModel);
    bool delete_remote;
    if (local_time > remote_time)
        delete_remote = true;
    else
        delete_remote =false;
    this->_overide_file(delete_remote, dataModel, local_file, remote_file);
}


// Option 2
void ConflictLogic::overide_with_older_file(DataModel *dataModel, FileModel *local_file, FileModel *remote_file){
    SftpSessionModel *sftpSessionModel = this->networkLogic->sftpSession;
    time_t local_time = this->fileLogic->get_local_last_modified(local_file);
    time_t remote_time = this->fileLogic->get_remote_last_modified(remote_file, sftpSessionModel);
    bool delete_remote;
    if (local_time < remote_time)
        delete_remote = true;
    else
        delete_remote =false;
    this->_overide_file(delete_remote, dataModel, local_file, remote_file);
}


// Option 3, I know this is kind of insane whoops
void ConflictLogic::keep_both_files_auto_rename(DataModel *dataModel, FileModel *local_file, FileModel *remote_file){
    SftpSessionModel *sftpSessionModel = this->networkLogic->sftpSession;
    unordered_map<string, FileModel*>&  local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*>&  remote_files = dataModel->get_remote_files();

    string old_local_key = local_file->get_relative_path();
    string old_remote_key = remote_file->get_relative_path();
    string remote_file_path = remote_file->get_remote_file_path();

    string new_remote_name = remote_file->get_name() + "_r";

    while (local_files.count(new_remote_name) != 0 && remote_files.count(new_remote_name) != 0)
        new_remote_name +="_r";
        
    remote_file->set_name(new_remote_name);
    string new_remote_file_path = remote_file->get_remote_file_path();

    string local_file_path = local_file->get_local_file_path();
    string new_local_name = local_file->get_name() + "_l";
    while (local_files.count(new_local_name) != 0 && remote_files.count(new_local_name) != 0)
        new_local_name +="_l";

    local_file->set_name(new_local_name);
    string new_local_file_path = local_file->get_local_file_path();

    
    int rc = sftp_rename(sftpSessionModel->get(), remote_file_path.c_str(), new_remote_file_path.c_str());
    if (rc < 0) {
        int error_code = sftp_get_error(this->networkLogic->sftpSession->get());
        const char *err_str = ssh_get_error(this->networkLogic->sftpSession->get()); 

        throw std::runtime_error("Failed to rename remote file. Error code: " + std::to_string(error_code) + " Error: " + err_str);
    }

    std::filesystem::rename(local_file_path, new_local_file_path);

   
    local_file->set_name(new_local_name);
    local_file->set_can_sync(true);
    local_file->set_conflict_bool(false);
    this->_update_key_in_datmodel(old_local_key, local_file->get_relative_path(), local_files);
    
    
    remote_file->set_name(new_remote_name);
    remote_file->set_can_sync(true);
    remote_file->set_conflict_bool(false);
    this->_update_key_in_datmodel(old_remote_key, remote_file->get_relative_path(), remote_files);


}


// Option 4
void ConflictLogic::omit_from_sync(FileModel *local_file, FileModel *remote_file){
    local_file->set_can_sync(false);
    local_file->set_conflict_bool(false);

    remote_file->set_can_sync(false);
    remote_file->set_conflict_bool(false);
}

//PRIVATES

bool ConflictLogic::_is_conflict(FileModel *local_file, FileModel *remote_file){
    string local_md5 = this->dataLogic->compute_md5_local(local_file->get_local_file_path());
    string remote_md5 = this->dataLogic->compute_md5_remote(this->networkLogic->sftpSession, remote_file->get_remote_file_path());
    return (local_md5 != remote_md5);
}


void ConflictLogic::_overide_file(bool delete_remote, DataModel *dataModel, FileModel *local_file, FileModel *remote_file) {
    SftpSessionModel *sftpSessionModel = this->networkLogic->sftpSession;
    unordered_map<string, FileModel*>&  local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*>&  remote_files = dataModel->get_remote_files();

    if (delete_remote) {
        int r_stat = sftp_unlink(sftpSessionModel->get(), remote_file->get_remote_file_path().c_str());
        if (r_stat != SSH_OK){
            cout << "Failed to overide: " << remote_file->get_remote_file_path() << endl;
            return this->omit_from_sync(local_file, remote_file);
        }
        auto it = remote_files.find(remote_file->get_relative_path());
        if (it != remote_files.end()) {
            delete it->second;
            remote_files.erase(it);
        }else
            delete remote_file;
        local_file->set_can_sync(true);
        local_file->set_conflict_bool(false);

    } else {
        bool l_stat = std::filesystem::remove(local_file->get_local_file_path());
        if (!l_stat){
            cout << "Failed to overide: " << local_file->get_local_file_path() << endl;
            return this->omit_from_sync(local_file, remote_file);
        }
        auto it = local_files.find(local_file->get_relative_path());
        if (it != local_files.end()) {
            delete it->second;
            local_files.erase(it);
        }else
            delete local_file;
        remote_file->set_can_sync(true);
        remote_file->set_conflict_bool(false);
    }
}


void ConflictLogic::_update_key_in_datmodel(string old_key, string new_key, unordered_map<string, FileModel*>& map){
    auto node = map.extract(old_key);
    if (!node.empty()) {
        node.key() = new_key;
        map.insert(std::move(node));
    }
}
