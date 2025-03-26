#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include "FileModel.h"

#include <unordered_map>


// + local_files: unordered_map<name:string, FileModel*>
// + remote_files: unordered_map<name:string, FileModel*> 

// + add_local_file(FileModel): void
// + add_remote_file(FileModel): void


class DataModel {
    unordered_map<string, FileModel> local_files;
    unordered_map<string, FileModel> remote_files;

public:
	//FileModel() = default;
	DataModel();
    	~DataModel() = default;


    
    void add_local_files(unordered_map<string, FileModel> file_models);
    void add_remote_files(unordered_map<string, FileModel> file_models);

private:
};

#endif // DATA_MODEL 



