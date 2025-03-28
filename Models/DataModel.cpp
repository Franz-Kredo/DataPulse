#include "DataModel.h"

DataModel::DataModel(){}

void DataModel::add_local_files(vector<FileModel*> *file_models){
    for (FileModel *file_model : *file_models){
        this->local_files[file_model->get_relative_path()] = file_model;
    }
}
void DataModel::add_remote_files(vector<FileModel*> *file_models){
    for (FileModel *file_model : *file_models){
        this->remote_files[file_model->get_relative_path()] = file_model;
    }
}

void DataModel::print(ostream &os) const {
    os << "\n\n\n";
    os << "===================================================================\n";
    os << "                         DataModel Summary                        \n";
    os << "===================================================================\n\n";

    // Print local files
    os << ">>> Local Files (" << this->local_files.size() << "):\n";
    os << "-------------------------------------------------------------------\n";
    for (const auto &pair : this->local_files) {
        const FileModel* file = pair.second;
        os << "File's relative path : " << file->get_relative_path() << "\n";
        os << "Size                 : " << file->get_size() << " bytes\n";
        os << "Local Path           : " << file->get_path() << "\n";
        os << "Syncable             : " << file->get_can_sync() << "\n";
        os << "-------------------------------------------------------------------\n";
    }

    os << "\n>>> Remote Files (" << this->remote_files.size() << "):\n";
    os << "-------------------------------------------------------------------\n";
    for (const auto &pair : this->remote_files) {
        const FileModel* file = pair.second;
        os << "File's relative path : " << file->get_relative_path() << "\n";
        os << "Size                 : " << file->get_size() << " bytes\n";
        os << "Remote Path          : " << file->get_remote_path() << "\n";
        os << "Syncable             : " << file->get_can_sync() << "\n";
        os << "-------------------------------------------------------------------\n";
    }
    os << "\n\n\n";
}

ostream& operator<<(ostream &os, const DataModel &dataModel) {
    dataModel.print(os);
    return os;
}


// bool DataModel::verify_syncable_files() const {
//     bool all_good = true;
    
//     // Check local files marked as syncable.
//     for (const auto& [relative_path, local_file] : local_files) {
//         if (local_file->get_can_sync()) {
//             auto remote_it = remote_files.find(relative_path);
//             if (remote_it == remote_files.end()) {
//                 cerr << "Error: Local file '" << relative_path 
//                           << "' is marked syncable but not found in remote files.\n";
//                 all_good = false;
//             } else if (local_file->get_size() != remote_it->second->get_size()) {
//                 cerr << "Error: Size mismatch for file '" << relative_path 
//                           << "': local size (" << local_file->get_size() 
//                           << ") vs remote size (" << remote_it->second->get_size() << ").\n";
//                 all_good = false;
//             }
//         }
//     }
    
//     // Check remote files marked as syncable.
//     for (const auto& [relative_path, remote_file] : remote_files) {
//         if (remote_file->get_can_sync()) {
//             auto local_lt = local_files.find(relative_path);
//             if (local_lt == local_files.end()) {
//                 cerr << "Error: Remote file '" << relative_path << "' is marked syncable but not found in local files.\n";
//                 all_good = false;
//             } else if (remote_file->get_size() != local_lt->second->get_size()) {
//                 cerr << "Error: Size mismatch for file '" << relative_path << "': remote size (" << remote_file->get_size() << ") vs local size (" << local_lt->second->get_size() << ").\n";
//                 all_good = false;
//             }
//         }
//     }
    
//     return all_good;
// }