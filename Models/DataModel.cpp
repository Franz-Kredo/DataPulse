#include "DataModel.h"


DataModel::DataModel(){}


void DataModel::add_local_files(unordered_map<string, FileModel> file_models){
    cout << "currently testing: add_local_files()" << endl;
    for (const auto& [filename, fileModel] : file_models) {
        cout << "Filename: " << fileModel.get_name() << endl;
        // Use fileModel here
        // fileModel.do_something();  // assuming there's a method like this
    }
    
}


void DataModel::add_remote_files(unordered_map<string, FileModel> file_models){

}

