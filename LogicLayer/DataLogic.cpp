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
    dataModel = this->mark_syncable_files(dataModel);

    
    //--- Printing Data Model Pretty! ---//
    cout << *dataModel << endl;
    
    // cout << "Return DataModel" << endl;
    return dataModel;
}

PlaceholderModel *DataLogic::write_data(PlaceholderModel *dataModel){
    PlaceholderModel *write_locally = this->write_local(dataModel); 
    PlaceholderModel *write_remote = this->write_remote(dataModel); 
    
    
    // Throw messages to main to print more info for user
    if (write_locally == nullptr && write_remote == nullptr) throw runtime_error("Error: Failed writing both local and remote files.");
    if (write_locally == nullptr) throw runtime_error("Error: Failed writing to local files.");
    if (write_remote == nullptr) throw runtime_error("Error: Failed writing to remote files.");

    return nullptr;
}



//===============================================================//
//=================== PRIVATE READING METHODS ===================//
//===============================================================//

unordered_map<string, FileModel>* DataLogic::read_local(CommandModel *commandModel){
    // TEST READING LOCAL //
    cout << "=========================================================\nDataLogic::read_local() -> Reading single file to test methods\n=========================================================\n" << endl;
    // string path = "SyncTester/syncable_file.nice";
    size_t chunk_size = 8;
    
    // string path = commandModel->get_local_path();
    // // string path = string(filesystem::current_path()) + "/SyncTester/";
	// string filename = "syncable_file.read";
    // size_t file_size = filesystem::file_size(path + filename);


    FileModel *fileModel = FileModel::populate_local_file_model(commandModel, "");
    

    // string write_path = string(filesystem::current_path()) + "/SyncTester/";
	string filename = "syncable_file.md";


    // cout << "\n\n\n[INFO DUMP]" << endl;
    while(!fileModel->get_fully_read()){
        // cout << "[START] READING LOCAL DATA" << endl;
        this->fileLogic->read_local_data(fileModel, chunk_size);
        // cout << "[STOP] READING LOCAL DATA" << endl;
        // cout << "[START] WRITE REMOTE DATA" << endl;
        // this->fileLogic->write_local_data(fileModel); // Simply for testing...
        this->fileLogic->write_remote_data(fileModel, this->networkLogic->sftpSession);
        // cout << "[STOP] WRITE REMOTE DATA" << endl;
    }

    bool was_read = fileModel->get_fully_read();

    // fileModel->set_name(write_filename);

    if(was_read) {
        cout << "|-----------------------------------|" << endl;
        cout << "|------| File was fully read |------|" << endl;
        cout << "|-----------------------------------|" << endl << endl;
    } else {
        cout << "|---------------------------------------|" << endl;
        cout << "|------| File was NOT fully read |------|" << endl;
        cout << "|---------------------------------------|" << endl << endl;
    }


    unordered_map<string, FileModel*> ret_map = {
        { filename, fileModel }
    };

    // Need to return the ret_map to begin with, changes incoming so returning nullptr for now
    return nullptr;
}

unordered_map<string, FileModel> *DataLogic::read_remote(CommandModel *commandModel){
    return nullptr;
}

DataModel *DataLogic::mark_syncable_files(DataModel *dataModel){
    return dataModel;
}



//===============================================================//
//=================== PRIVATE WRITING METHODS ===================//
//===============================================================//

PlaceholderModel *DataLogic::write_local(PlaceholderModel *dataModel){
    return nullptr;
}

PlaceholderModel *DataLogic::write_remote(PlaceholderModel *dataModel){
    return nullptr;
}



//=============================================================//
//=================== PRIVATE FILES METHODS ===================//
//=============================================================//

vector<FileModel*> *DataLogic::collect_local_files(CommandModel *commandModel){
    vector<FileModel*> *all_file_models = new vector<FileModel*>();;
    cout << "\n\n-=-=-=-=-= DataLogic::get_all_local_files -=-=-=-=-=" << endl;

    string local_path = commandModel->get_local_path();
    for (const auto & entry : filesystem::directory_iterator(local_path)){
        string filename = entry.path().filename();
        FileModel *fileModel = FileModel::populate_local_file_model(commandModel, filename);
        std::cout << entry.path().filename() << std::endl;
        // std::cout << entry.path().filename() << std::endl;
        // std::cout << entry.path().filename() << std::endl;
        all_file_models->push_back(fileModel);
    }

    return all_file_models;
}

vector<FileModel*> *DataLogic::collect_remote_files(CommandModel *commandModel){
    vector<FileModel*> *all_file_models = new vector<FileModel*>();;
    cout << "\n\n-=-=-=-=-= DataLogic::get_all_remote_files -=-=-=-=-=" << endl;

    string remote_path = commandModel->get_remote_path();
    
    sftp_dir dir = sftp_opendir(this->networkLogic->sftpSession->get(), commandModel->get_remote_path().c_str());
    if (!dir) throw std::runtime_error("Unable to open remote directory: " + commandModel->get_remote_path());

    sftp_attributes attrs;
    while ((attrs = sftp_readdir(this->networkLogic->sftpSession->get(), dir)) != nullptr) {
        if (attrs->type == SSH_FILEXFER_TYPE_REGULAR) {
            // FileModel::populate_remote_file_model(commandModel, attrs->size, attrs->name);
            // std::cout << "File: " << attrs->name << "\n"; // std::cout << "  Size: " << attrs->size << " bytes\n\n";
            std::cout << "File: " << attrs->name << "\n"; 
            FileModel *fileModel = FileModel::populate_remote_file_model(commandModel, attrs->name, attrs->size);
            all_file_models->push_back(fileModel);
        }
        sftp_attributes_free(attrs);
    }
    sftp_closedir(dir);

    return all_file_models;
}

// void NetworkLogic::list_remote_directory(CommandModel *commandModel) {
//     sftp_dir dir = sftp_opendir(this->sftpSession->get(), commandModel->get_remote_path().c_str());
//     if (!dir) throw std::runtime_error("Unable to open remote directory: " + commandModel->get_remote_path());

//     sftp_attributes attrs;
//     while ((attrs = sftp_readdir(this->sftpSession->get(), dir)) != nullptr) {
//         if (attrs->type == SSH_FILEXFER_TYPE_REGULAR) {
//             std::cout << "File: " << attrs->name << "\t\t\t";
//             std::cout << "  Size: " << attrs->size << " bytes\t";
//         }
//         sftp_attributes_free(attrs);
//     }
//     sftp_closedir(dir);
// }


//=========================================================//
//=================== TEMPLATED METHODS ===================//
//=========================================================//

void DataLogic::populate_local_file_model(CommandModel *commandModel){
    string path = filesystem::current_path();
        path+="/SyncTester/";

    string test_filename = "syncable_file.nice";
    size_t test_size = filesystem::file_size(path + test_filename);

    FileModel fileModel(path, test_filename, test_size);

}


// /localpath/somefile.txt

// /localpath/somefile.txt.bak +a +a +a






