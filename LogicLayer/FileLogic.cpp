#include "FileLogic.h"
#include <charconv>
#include <filesystem>
#include <ios>
#include <iostream>
#include <string>



void FileLogic::read_local_data(FileModel* fileModel, size_t chunk_size){
    if (fileModel->get_fully_read()) throw runtime_error("Attempting to read a fully read file: " + fileModel->get_name());
    string file_name = fileModel->get_path() + fileModel->get_name(); 
    //file_name = "FileLogic.h"; // should be repalace
    ifstream file(file_name, ios::binary);
    if (!file.is_open()){
        fileModel->set_read_perm(false);
        cout << "No read perms on " + fileModel->get_name() << endl;
        return;
    }
    fileModel->set_read_perm(true);
    cout << "Initial buffer size: " << fileModel->get_buffer().size() << endl;
    //Update the size if it is not set
    size_t size = filesystem::file_size(file_name);

    fileModel->set_size(size);
    
    if (chunk_size > size) chunk_size = size;
    size_t start = fileModel->get_bytes_read();
    
    vector<byte> buffer(chunk_size);

    file.seekg(start);
    file.read(reinterpret_cast<char*>(buffer.data()), chunk_size);
    file.close();

    fileModel->increase_bytes_read(chunk_size);

    if (fileModel->get_buffer().size() > 0)
        throw runtime_error("Data of file " + fileModel->get_name() + " on the local is being overidden");
    fileModel->populate_buffer(buffer);
    if (fileModel->get_bytes_read() > size) 
        throw runtime_error("We read magic from" + fileModel->get_name() + " on the local");
    if (fileModel->get_bytes_read() == size)
        fileModel->set_fully_read(true);

}
void FileLogic::write_local_data(FileModel* fileModel){
    string file_name = fileModel->get_path() + fileModel->get_name(); 
    //file_name = "test_write_to.txt"; // should be repalace
    
    if (!filesystem::exists(file_name)){
        ofstream create(file_name, ios::binary);
        create.close();
    }

    if (fileModel->get_buffer().size() == 0){
        cout << "Skipping write on an empty buffer for:" + fileModel->get_name() << endl;
        return;
    }

    ofstream file(file_name, ios::binary | ios::app);
    if (!file.is_open()){
        fileModel->set_write_perm(false);
        cout << "No write perms on " + fileModel->get_name() << endl;
        return;
    }
    fileModel->set_write_perm(true);

    
    //size_t size = filesystem::file_size(file_name);
    //file.seekp(size);
    vector<byte> buffer = fileModel->get_buffer();;
    //for (byte bits : buffer){
    //    cout << static_cast<int>(bits) << endl;
    //} 

    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    file.close();
    fileModel->clear_buffer();


}
