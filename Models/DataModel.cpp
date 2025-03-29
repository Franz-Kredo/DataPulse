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

