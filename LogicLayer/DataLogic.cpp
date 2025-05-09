#include "DataLogic.h"
#include <set>
#include <string>

DataLogic::DataLogic(FileLogic *fileLogic, NetworkLogic *networkLogic){
    this->fileLogic = fileLogic;
    this->networkLogic = networkLogic;
}

//==================================================================//
//===================== PUBLIC COMPARE METHODS =====================//
//==================================================================//

bool DataLogic::compare_synced_data(DataModel *dataModel, CommandModel *commandModel){

    unordered_map<string, FileModel*> local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*> remote_files = dataModel->get_remote_files();
    
    bool all_good = true;
    bool is_merge = commandModel->get_merge();
    if(is_merge){
        cout << "File MD5 Checksum:" << endl;
    }

    std::set<string> file_set;
    for (const auto &pair : local_files) {
        const string &relative_path = pair.first;
        FileModel* local_file = pair.second;
        auto remote_file_model = remote_files.find(relative_path);
        if(is_merge){
            cout << "Calling MD5 2" << endl; 
            cout << "local_file->get_local_file_path(): " << local_file->get_local_file_path() << endl;
            cout << "remote_files[relative_path]->get_remote_file_path(): " << remote_files[relative_path]->get_remote_file_path() << endl;
            string local_md5 = compute_md5_local(local_file->get_local_file_path());
            string remote_md5 = compute_md5_remote(this->networkLogic->sftpSession, remote_files[relative_path]->get_remote_file_path());

            if (local_md5 == remote_md5) {
                cout << relative_path << "\t-\tSame" << endl;
                file_set.insert(relative_path);
            } else {
                cout << relative_path << "\t-\tDifferent" << endl;
                file_set.insert(relative_path);
            }
        }
        else if(remote_file_model == remote_files.end()) {
            cout << "Some sync issue with: " << relative_path << endl;
            all_good = false;
        }


    }
    //--- Going through all remote files to mark files that don't exist locally ---//
    for (const auto &pair : remote_files) {
        const string &relative_path = pair.first;
        FileModel* remote_file = pair.second;
        auto local_file_model = local_files.find(relative_path);
        if(is_merge){ // should be if(is_merge), I'm doing if(!is_merge) for testing
            string remote_md5 = compute_md5_remote(this->networkLogic->sftpSession, remote_file->get_remote_file_path());
            string local_md5 = compute_md5_local(remote_files[relative_path]->get_local_file_path());
            if (local_md5 == remote_md5 && file_set.count(relative_path) == 0){
                cout << relative_path << "\t-\tSame" << endl;
            }else if (file_set.count(relative_path) == 0){
                cout << relative_path << "\t-\tDifferent" << endl;

            if (local_md5 == remote_md5) {
                // cout << "Files at " << relative_path << " are identical based on MD5 checksum." << endl;
            } 
            }
        }
        else if(local_file_model == local_files.end()) {
            cout << "Some sync issue with: " << relative_path << endl;
            all_good = false;
        }
    }

    return all_good;
}


string DataLogic::compute_md5_local(const string& file_path) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) 
        throw runtime_error("EVP_MD_CTX_new failed");

    if (1 != EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr)) {
        EVP_MD_CTX_free(mdctx);
        throw runtime_error("EVP_DigestInit_ex failed");
    }

    ifstream file(file_path, ios::binary);
    if (!file) {
        EVP_MD_CTX_free(mdctx);
        throw runtime_error("Cannot open local file: " + file_path);
    }
    
    const size_t buffer_size = 8192;
    vector<char> buffer(buffer_size);
    while (file.good()) {
        file.read(buffer.data(), buffer.size());
        streamsize count = file.gcount();
        if (count > 0) {
            if (1 != EVP_DigestUpdate(mdctx, buffer.data(), count)) {
                EVP_MD_CTX_free(mdctx);
                throw runtime_error("EVP_DigestUpdate failed");
            }
        }
    }
    file.close();

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;
    if (1 != EVP_DigestFinal_ex(mdctx, digest, &digest_len)) {
        EVP_MD_CTX_free(mdctx);
        throw runtime_error("EVP_DigestFinal_ex failed");
    }
    EVP_MD_CTX_free(mdctx);

    stringstream ss;
    for (unsigned int i = 0; i < digest_len; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)digest[i];
    }
    return ss.str();
}


string DataLogic::compute_md5_remote(SftpSessionModel *sftpSessionModel, const string& remote_path) { 
    sftp_session sftp = sftpSessionModel->get();
    sftp_file file = sftp_open(sftp, remote_path.c_str(), O_RDONLY, 0);
    if (!file) {
        throw runtime_error("Cannot open remote file: " + remote_path);
    }
    
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        sftp_close(file);
        throw runtime_error("EVP_MD_CTX_new failed");
    }

    if (1 != EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr)) {
        EVP_MD_CTX_free(mdctx);
        sftp_close(file);
        throw runtime_error("EVP_DigestInit_ex failed");
    }
    
    const size_t buffer_size = 8192;
    vector<char> buffer(buffer_size);
    int bytes_read;
    
    try {
        while ((bytes_read = sftp_read(file, buffer.data(), buffer.size())) > 0) {
            if (1 != EVP_DigestUpdate(mdctx, buffer.data(), bytes_read)) {
                throw runtime_error("EVP_DigestUpdate failed");
            }
        }
        
        if (bytes_read < 0) {
            throw runtime_error("Error reading remote file: " + remote_path);
        }
        
        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_len = 0;
        if (1 != EVP_DigestFinal_ex(mdctx, digest, &digest_len)) {
            throw runtime_error("EVP_DigestFinal_ex failed");
        }
        
        stringstream ss;
        for (unsigned int i = 0; i < digest_len; ++i) {
            ss << hex << setw(2) << setfill('0') << (int)digest[i];
        }
        
        // Cleanup 
        EVP_MD_CTX_free(mdctx);
        sftp_close(file);
        
        return ss.str();
    } catch (const exception& e) {
        // Cleanup
        EVP_MD_CTX_free(mdctx);
        sftp_close(file);
        throw;
    }
}


//================================================================//
//=============== PUBLIC READING & WRITING METHODS ===============//
//================================================================//

DataModel *DataLogic::collect_files(CommandModel *commandModel, bool print_model){ 
    vector<FileModel*> *local_files = this->collect_local_files(commandModel);
    vector<FileModel*> *remote_files = this->collect_remote_files(commandModel);
    DataModel *dataModel = nullptr;

    try{
        if (local_files == nullptr && remote_files == nullptr) throw runtime_error("!Error: Failed reading both local and remote files.");
        if (local_files == nullptr) throw runtime_error("!Error: Failed reading local files.");
        if (remote_files == nullptr) throw runtime_error("!Error: Failed reading remote files.");
    
        DataModel *dataModel = new DataModel();
        
        dataModel->add_local_files(local_files);
        dataModel->add_remote_files(remote_files);
        
        dataModel = this->mark_syncable_files(dataModel, commandModel);
        
        if(print_model){
            //--- Printing Data Model Pretty! ---//
            cout << *dataModel << endl;
        }
        delete local_files;
        delete remote_files;
        
        
        return dataModel;


    } catch (...) {
        if (local_files) delete local_files;
        if (remote_files) delete remote_files;
        if (dataModel) delete dataModel;
        throw; // Re-throw the exception after cleanup
    }
}


DataModel *DataLogic::write_data(DataModel *dataModel, CommandModel *commandModel){
    if (dataModel == nullptr) throw runtime_error("!Error: dataModel is empty in DataLogic::write_data().");
    
    dataModel = this->write_local(dataModel, commandModel); 
    if (dataModel == nullptr) throw runtime_error("!Error: Failed writing to local files.");
    
    dataModel = this->write_remote(dataModel, commandModel); 
    if (dataModel == nullptr) throw runtime_error("!Error: Failed writing to remote files.");
    

    // Throw messages to main to print more info for user
    return dataModel;
}



//===============================================================//
//=================== PRIVATE READING METHODS ===================//
//===============================================================//

DataModel *DataLogic::mark_syncable_files(DataModel *dataModel, CommandModel *commandModel){
    bool is_merge = commandModel->get_merge();
    unordered_map<string, FileModel*> local_files = dataModel->get_local_files();
    unordered_map<string, FileModel*> remote_files = dataModel->get_remote_files();
    
    //=== For a sync that is non-merge ===//
    if(!is_merge){
        //--- Going through all local files to mark files that don't exist remotely ---//
        for (const auto &pair : local_files) {
            const string &relative_path = pair.first;
            FileModel* local_file = pair.second;
            auto remote_file_model = remote_files.find(relative_path);
            if(remote_file_model == remote_files.end()) {
                local_file->set_can_sync(true);
            }
        }
        //--- Going through all remote files to mark files that don't exist locally ---//
        for (const auto &pair : remote_files) {
            const string &relative_path = pair.first;
            FileModel* remote_file = pair.second;
            auto local_file_model = local_files.find(relative_path);
            if(local_file_model == local_files.end()) {
                remote_file->set_can_sync(true);
            }
        }

        //--- Return the model with marked can_sync values ---//
        return dataModel;
    } 
    
    
    return dataModel;
}



//===============================================================//
//=================== PRIVATE WRITING METHODS ===================//
//===============================================================//

DataModel *DataLogic::write_local(DataModel *dataModel, CommandModel *commandModel){
    size_t chunk_size = 16384;
    
    unordered_map<string, FileModel *> local_files = dataModel->get_local_files();
    unordered_map<string, FileModel *> remote_files = dataModel->get_remote_files();

    // Create logic for non-merge sync
    for (const auto &pair : remote_files) {
        const string &relative_path = pair.first;
        FileModel* local_file = pair.second;
        const bool can_sync = local_file->get_can_sync();
        if(!can_sync)
            continue;

        auto local_file_model = local_files.find(relative_path);

        // If the file is not found locally (which we want), then write it locally
        if(local_file_model == local_files.end()) {
            while(!local_file->get_fully_read()){
                this->fileLogic->read_remote_data(local_file, this->networkLogic->sftpSession ,chunk_size);
                this->fileLogic->write_local_data(local_file);
            }
        }
    }

    return dataModel;
}


DataModel *DataLogic::write_remote(DataModel *dataModel, CommandModel *commandModel){
    size_t chunk_size = 16384;
    
    unordered_map<string, FileModel *> local_files = dataModel->get_local_files();
    unordered_map<string, FileModel *> remote_files = dataModel->get_remote_files();

    // Create logic for non-merge sync
    for (const auto &pair : local_files) {
        const string &relative_path = pair.first;
        FileModel* local_file = pair.second;
        const bool can_sync = local_file->get_can_sync();
        if(!can_sync)
            continue;

        auto remote_file_model = remote_files.find(relative_path);
        // If the file is not found on remote (which we want), then write it on remote
        if(remote_file_model == remote_files.end()) {
            while(!local_file->get_fully_read()){
                this->fileLogic->read_local_data(local_file, chunk_size);
                this->fileLogic->write_remote_data(local_file, this->networkLogic->sftpSession);
            }
        }
    }

    return dataModel;
}


//=============================================================//
//=================== PRIVATE FILES METHODS ===================//
//=============================================================//

vector<FileModel*> *DataLogic::collect_local_files(CommandModel *commandModel){
    vector<FileModel*> *all_file_models = new vector<FileModel*>();;

    string local_path = commandModel->get_local_dir_path();
    for (const auto &entry : filesystem::recursive_directory_iterator(local_path)) {
        if (!filesystem::is_regular_file(entry))
            continue; // Skip dirs

        string relative_path = filesystem::relative(entry.path(), local_path).string();
        FileModel *fileModel = FileModel::populate_local_file_model(commandModel, relative_path);

        all_file_models->push_back(fileModel);
    }

    return all_file_models;
}


vector<FileModel*> *DataLogic::collect_remote_files(CommandModel *commandModel){
    vector<FileModel*> *all_file_models = new vector<FileModel*>();

    string remote_base = commandModel->get_remote_dir_path();
    // Start the recursive traversal from the base remote directory.
    collect_remote_files_recursive(this->networkLogic->sftpSession->get(), remote_base, remote_base, commandModel, *all_file_models);

    return all_file_models;
}


// Helper function for recursive remote file collection.
void DataLogic::collect_remote_files_recursive(sftp_session sftp, const string &base_path, const string &current_path, CommandModel *commandModel, vector<FileModel*> &files) {

    sftp_dir dir = sftp_opendir(sftp, current_path.c_str());
    if (!dir) {
        throw runtime_error("!Unable to open remote directory: " + current_path);
    }

    sftp_attributes attrs;
    while ((attrs = sftp_readdir(sftp, dir)) != nullptr) {
        string name = attrs->name;
        // Skip the current and parent directory entries.
        if (name == "." || name == "..") {
            sftp_attributes_free(attrs);
            continue;
        }

        // Build the full remote path for the current entry.
        string full_path = current_path + "/" + name;

        if (attrs->type == SSH_FILEXFER_TYPE_DIRECTORY) {
            // Recursively traverse the subdirectory.
            collect_remote_files_recursive(sftp, base_path, full_path, commandModel, files);
        } else if (attrs->type == SSH_FILEXFER_TYPE_REGULAR) {
            // Compute the relative path by removing the base path prefix.
            string relative_path = full_path.substr(base_path.size());
            if (!relative_path.empty() && relative_path[0] == '/')
                relative_path = relative_path.substr(1);
            
            FileModel *fileModel = FileModel::populate_remote_file_model(commandModel, relative_path, attrs->size);
            files.push_back(fileModel);
        }

        sftp_attributes_free(attrs);
    }
    sftp_closedir(dir);
}





