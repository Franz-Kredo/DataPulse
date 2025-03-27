#include "DataModel.h"


DataModel::DataModel(){}


void DataModel::add_local_files(unordered_map<string, FileModel> file_models){
    cout << "currently testing: add_local_files()" << endl;
    for (const auto& [filename, fileModel] : file_models) {
        cout << "Filename: " << fileModel.get_name() << endl;
        // Use fileModel here
    }
    
}


void DataModel::add_remote_files(unordered_map<string, FileModel> file_models){

}

