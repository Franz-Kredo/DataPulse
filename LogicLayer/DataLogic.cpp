#include "DataLogic.h"


DataLogic::DataLogic(FileLogic *fileLogic, NetworkLogic *networkLogic){
    this->fileLogic = fileLogic;
    this->networkLogic = networkLogic;
}


//================================================================//
//=============== PUBLIC READING & WRITING METHODS ===============//
//================================================================//
DataModel *DataLogic::read_data(CommandModel *commandModel){ 
    unordered_map<string, FileModel> *local_files = this->read_local(commandModel); 
    unordered_map<string, FileModel> *remote_files = this->read_remote(commandModel); 
    
    // Throw messages to main to print more info for user
    if (local_files == nullptr && remote_files == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading both local and remote files.");
    if (local_files == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading local files.");
    if (remote_files == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed reading remote files.");

    DataModel *dataModel = new DataModel();

    dataModel->add_local_files(*local_files);
    dataModel->add_remote_files(*remote_files);
    
    dataModel = this->mark_syncable_files(dataModel);
    
    return dataModel;
}

PlaceholderModel *DataLogic::write_data(PlaceholderModel *dataModel){
    PlaceholderModel *write_locally = this->write_local(dataModel); 
    PlaceholderModel *write_remote = this->write_remote(dataModel); 
    
    
    // Throw messages to main to print more info for user
    if (write_locally == nullptr && write_remote == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed writing both local and remote files.");
    if (write_locally == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed writing to local files.");
    if (write_remote == nullptr) throw runtime_error("[Need DataModel and FileModel to implement fully] Error: Failed writing to remote files.");

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


    FileModel *fileModel = FileModel::populate_file_model(commandModel);
    

    // string write_path = string(filesystem::current_path()) + "/SyncTester/";
	string filename = "syncable_file.md";


    cout << "\n\n\n[INFO DUMP]" << endl;
    while(!fileModel->get_fully_read()){
        cout << "[START] READING LOCAL DATA" << endl;
        this->fileLogic->read_local_data(fileModel, chunk_size);
        cout << "[STOP] READING LOCAL DATA" << endl;
        cout << "[START] WRITE REMOTE DATA" << endl;
        // this->fileLogic->write_local_data(fileModel); // Simply for testing...
        this->fileLogic->write_remote_data(fileModel, this->networkLogic->sftpSession);
        cout << "[STOP] WRITE REMOTE DATA" << endl;
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






