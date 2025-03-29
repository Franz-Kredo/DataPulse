#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include "FileModel.h"

#include <unordered_map>
#include <vector>



class DataModel {
    unordered_map<string, FileModel*> local_files;
    unordered_map<string, FileModel*> remote_files;

public:
	//FileModel() = default;
	DataModel();
    	// ~DataModel() = default;

    ~DataModel() {
        for (auto &pair : local_files)
            delete pair.second;
        for (auto &pair : remote_files)
            delete pair.second;
    }

    void add_local_files(vector<FileModel*> *file_models);
    void add_remote_files(vector<FileModel*> *file_models);
    unordered_map<string, FileModel*>& get_local_files(){return this->local_files;}
    unordered_map<string, FileModel*>& get_remote_files(){return this->remote_files;}

    // Member print method that outputs the contents of the DataModel
    void print(ostream &os) const;
    friend ostream& operator<<(ostream &os, const DataModel &dataModel);

private:
};

#endif // DATA_MODEL 



