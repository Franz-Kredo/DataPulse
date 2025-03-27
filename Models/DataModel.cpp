#include "DataModel.h"

DataModel::DataModel(){}

void DataModel::add_local_files(vector<FileModel*> *file_models){
    for (FileModel *file_model : *file_models){
        this->local_files[file_model->get_name()] = file_model;
    }
}
void DataModel::add_remote_files(vector<FileModel*> *file_models){
    for (FileModel *file_model : *file_models){
        this->remote_files[file_model->get_name()] = file_model;
    }
}

void DataModel::print(std::ostream &os) const {
    os << "\n\n\n";
    os << "===================================================================\n";
    os << "                         DataModel Summary                        \n";
    os << "===================================================================\n\n";

    // Print local files
    os << ">>> Local Files (" << this->local_files.size() << "):\n";
    os << "-------------------------------------------------------------------\n";
    for (const auto &pair : this->local_files) {
        const FileModel* file = pair.second;
        os << "Name       : " << file->get_name() << "\n";
        os << "Size       : " << file->get_size() << " bytes\n";
        os << "Local Path : " << file->get_path() << "\n";
        os << "Syncable   : " << file->get_can_sync() << "\n";
        os << "-------------------------------------------------------------------\n";
    }

    os << "\n>>> Remote Files (" << this->remote_files.size() << "):\n";
    os << "-------------------------------------------------------------------\n";
    for (const auto &pair : this->remote_files) {
        const FileModel* file = pair.second;
        os << "Name         : " << file->get_name() << "\n";
        os << "Size         : " << file->get_size() << " bytes\n";
        os << "Remote Path  : " << file->get_remote_path() << "\n";
        os << "Syncable     : " << file->get_can_sync() << "\n";
        os << "-------------------------------------------------------------------\n";
    }
    os << "\n\n\n";
}

std::ostream& operator<<(std::ostream &os, const DataModel &dataModel) {
    dataModel.print(os);
    return os;
}

// void DataModel::print(std::ostream &os) const {
//     os << "DataModel:\n";
    
//     // Print local files
//     os << "Local Files (" << local_files.size() << "):\n";
//     for (const auto &pair : local_files) {
//         const FileModel* file = pair.second;
//         os << "  Name: " << file->get_name()
//            << ", Size: " << file->get_size()
//            << " bytes, Path: " << file->get_path() << "\n";
//     }
    
//     // Print remote files
//     os << "Remote Files (" << remote_files.size() << "):\n";
//     for (const auto &pair : remote_files) {
//         const FileModel* file = pair.second;
//         os << "  Name: " << file->get_name()
//            << ", Size: " << file->get_size()
//            << " bytes, Remote Path: " << file->get_remote_path() << "\n";
//     }
// }

// std::ostream& operator<<(std::ostream &os, const DataModel &dataModel) {
//     dataModel.print(os);
//     return os;
// }