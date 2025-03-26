#include "FileLogic.h"


void FileLogic::read_local_data(FileModel* fileModel, size_t chunk_size){
    string file_name = fileModel->get_path() + fileModel->get_name(); 
    file_name = "FileLogic.h"; // should be repalace
    ofstream file; 
    file.open (file_name);

    //Update the size if it is not set
    size_t size = filesystem::file_size(file_name);
    fileModel->set_size(size);
    
    if (chunk_size > size) chunk_size = size;
        
    vector<byte> chunks ;
    vector<byte> buffer(chunk_size);

}
