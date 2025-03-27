#include "DataLogic.h"


DataLogic::DataLogic(FileLogic *fileLogic, NetworkLogic *networkLogic){
    this->fileLogic = fileLogic;
    this->networkLogic = networkLogic;
}


//================================================================//
//=============== PUBLIC READING & WRITING METHODS ===============//
//================================================================//

DataModel *DataLogic::collect_files(CommandModel *commandModel){ 
    // cout << "Collecting all local files as vector of FileModels" << endl;
    vector<FileModel*> *local_files = this->collect_local_files(commandModel);
    // cout << "Collecting all remote files as vector of FileModels" << endl;
    vector<FileModel*> *remote_files = this->collect_remote_files(commandModel);
    // cout << "\n\n\n\n\n\n";

    cout << "After collecting FileModel for each file both local and remote" << endl;
    // Throw messages to main to print more info for user
    if (local_files == nullptr && remote_files == nullptr) throw runtime_error("Error: Failed reading both local and remote files.");
    if (local_files == nullptr) throw runtime_error("Error: Failed reading local files.");
    if (remote_files == nullptr) throw runtime_error("Error: Failed reading remote files.");

    cout << "Creating DataModel" << endl;
    DataModel *dataModel = new DataModel();
    
    // cout << "Populating local files to DataModel" << endl;
    dataModel->add_local_files(local_files);
    // cout << "Populating remote files to DataModel" << endl;
    dataModel->add_remote_files(remote_files);
    
    // cout << "Mark syncable files to DataModel" << endl;
    dataModel = this->mark_syncable_files(dataModel, commandModel);

    
    //--- Printing Data Model Pretty! ---//
    cout << *dataModel << endl;
    
    // cout << "Return DataModel" << endl;
    return dataModel;
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

unordered_map<string, FileModel> *DataLogic::read_remote(CommandModel *commandModel){
    return nullptr;
}

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
    
    
    //=== For a merge sync ===//
    cout << "DataLogic::mark_syncable_files() -> You just tried to mark syncable files with merge, this might not be implemented yet..." << endl;
    return dataModel;
}



//===============================================================//
//=================== PRIVATE WRITING METHODS ===================//
//===============================================================//

DataModel *DataLogic::write_local(DataModel *dataModel, CommandModel *commandModel){
    cout << "DataLogic::write_local() was called" << endl;
    
    size_t chunk_size = 8;
    
    unordered_map<std::string, FileModel *> local_files = dataModel->get_local_files();
    unordered_map<std::string, FileModel *> remote_files = dataModel->get_remote_files();

    // Create logic for non-merge sync
    if(!commandModel->get_merge()){
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
    } else {
        cout << "[yet to be implemented] Here we would write files when doing a merge sync" << endl;
        return nullptr;
    }

    return dataModel;
}


DataModel *DataLogic::write_remote(DataModel *dataModel, CommandModel *commandModel){
    cout << "DataLogic::write_remote() was called" << endl;

    size_t chunk_size = 8;
    
    unordered_map<std::string, FileModel *> local_files = dataModel->get_local_files();
    unordered_map<std::string, FileModel *> remote_files = dataModel->get_remote_files();

    // Create logic for non-merge sync
    if(!commandModel->get_merge()){
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
    } else {
        cout << "[yet to be implemented] Here we would write files when doing a merge sync" << endl;
        return nullptr;
    }

    return dataModel;
}



//=============================================================//
//=================== PRIVATE FILES METHODS ===================//
//=============================================================//

vector<FileModel*> *DataLogic::collect_local_files(CommandModel *commandModel){
    vector<FileModel*> *all_file_models = new vector<FileModel*>();;
    cout << "\n\n-=-=-=-=-= DataLogic::get_all_local_files -=-=-=-=-=" << endl;

    string local_path = commandModel->get_local_dir_path();
    for (const auto & entry : filesystem::directory_iterator(local_path)){
        // string relative_path = entry.path().filename(); // Outdated, we are now using relative_path
        string relative_path = filesystem::relative(entry.path(), local_path).string();
        cout << "---- RELATIVE PATH 1: "<< relative_path <<" ----";
        FileModel *fileModel = FileModel::populate_local_file_model(commandModel, relative_path);
        std::cout << relative_path << std::endl; // Outdated, we are now using relative_path

        all_file_models->push_back(fileModel);
    }

    return all_file_models;
}

vector<FileModel*> *DataLogic::collect_remote_files(CommandModel *commandModel){
    vector<FileModel*> *all_file_models = new vector<FileModel*>();;
    cout << "\n\n-=-=-=-=-= DataLogic::get_all_remote_files -=-=-=-=-=" << endl;

    string remote_path = commandModel->get_remote_dir_path();
    
    sftp_dir dir = sftp_opendir(this->networkLogic->sftpSession->get(), commandModel->get_remote_dir_path().c_str());

    if (!dir) throw std::runtime_error("Unable to open remote directory: " + commandModel->get_remote_dir_path());

    sftp_attributes attrs;
    while ((attrs = sftp_readdir(this->networkLogic->sftpSession->get(), dir)) != nullptr) {
        if (attrs->type == SSH_FILEXFER_TYPE_REGULAR) {
            string relative_path = attrs->name; // This should give us the relative path
            cout << "---- RELATIVE PATH 2: "<< relative_path <<" ----" << endl;
        // FileModel::populate_remote_file_model(commandModel, attrs->size, attrs->name);
            // std::cout << "File: " << attrs->name << "\n"; // std::cout << "  Size: " << attrs->size << " bytes\n\n";
            std::cout << "File's relative path: " << relative_path << endl; 
            FileModel *fileModel = FileModel::populate_remote_file_model(commandModel, relative_path, attrs->size);
            all_file_models->push_back(fileModel);
        }
        sftp_attributes_free(attrs);
    }
    sftp_closedir(dir);

    return all_file_models;
}







