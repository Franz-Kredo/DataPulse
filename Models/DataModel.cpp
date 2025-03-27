#include "DataModel.h"

DataModel::DataModel(){}

void DataModel::add_local_files(vector<FileModel*> file_models){
    for (FileModel *file_model : file_models){
        this->local_files[file_model->get_name()] = file_model;
    }
}
void DataModel::add_remote_files(vector<FileModel*> file_models){
    for (FileModel *file_model : file_models){
        this->remote_files[file_model->get_name()] = file_model;
    }
}

