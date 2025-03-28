#include "FileLogic.h"
#include <cstddef>
#include <filesystem>
#include <ios>
#include <iostream>
#include <string>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <fcntl.h>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <sys/stat.h>

// Good chunk size, according to docs 16kb

void FileLogic::read_local_data(FileModel* fileModel, size_t chunk_size){
    // cout << "=========================================================\nFileLogic::read_local_data() -> Reading the actual data\n=========================================================\n" << endl;

    if (fileModel->get_fully_read()) throw runtime_error("!Attempting to read a fully read file: " + fileModel->get_name());
    string file_name = fileModel->get_path() + "/" + fileModel->get_name(); 
    //file_name = "FileLogic.h"; // should be repalace
    ifstream file(file_name, ios::binary);
    if (!file.is_open()){
        fileModel->set_read_perm(false);
        cout << "No read perms on local: " + fileModel->get_name() << endl;
        return;
    }
    fileModel->set_read_perm(true);
    //Update the size if it is not set
    size_t size = filesystem::file_size(file_name);
    if (size != fileModel->get_size()) 
        fileModel->set_size(size);
   
    size_t curr_bytes = fileModel->get_bytes_read();
    if (chunk_size > size-curr_bytes) chunk_size = size-curr_bytes;
    
    vector<byte> buffer(chunk_size);

    file.seekg(curr_bytes);
    file.read(reinterpret_cast<char*>(buffer.data()), chunk_size);
    fileModel->increase_bytes_read(file.gcount());
    file.close();
    this->_update_model_with_data(fileModel, buffer);
}

void FileLogic::write_local_data(FileModel* fileModel){
    string file_name = fileModel->get_path() + "/" + fileModel->get_name(); 
    
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

    vector<byte> buffer = fileModel->get_buffer();;

    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    file.close();
    fileModel->clear_buffer();
}

void FileLogic::read_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel, size_t chunk_size){
    if (fileModel->get_fully_read()) 
        throw runtime_error("Attempting to read a fully read file: " + fileModel->get_name());

    sftp_session sftp = sftpSessionModel->get();
    string file_name = fileModel->get_remote_path() + "/" + fileModel->get_name(); 
    int access_type = O_RDONLY;

    sftp_file file = sftp_open(sftp, file_name.c_str(), access_type, 0);
    if (!file){
        fileModel->set_read_perm(false);
        cout << "No read perms on remote: " + fileModel->get_name() << endl;
        return;
    }
    fileModel->set_read_perm(true);
    size_t size = this->_get_remote_size(sftpSessionModel, file_name);
    if (size != fileModel->get_size()) 
        fileModel->set_size(size);

    size_t curr_bytes = fileModel->get_bytes_read();
    if (chunk_size > size-curr_bytes) chunk_size = size-curr_bytes;
    
    vector<byte> buffer(chunk_size);

    if (sftp_seek64(file, static_cast<uint64_t>(curr_bytes)) < 0) {
        sftp_close(file);
        throw runtime_error("Failed to seek to remote position " + to_string(curr_bytes) + " in file: " + file_name);
    }

    int bytes_read = sftp_read(file, reinterpret_cast<char*>(buffer.data()), chunk_size);

    if(!bytes_read) cout << "Read nothing from remote file: " + file_name  << endl;

    fileModel->increase_bytes_read(bytes_read);
    this->_update_model_with_data(fileModel, buffer);
}

void FileLogic::write_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel) {
    string full_file_path = fileModel->get_remote_path() + "/" + fileModel->get_name();
    sftp_session sftp = sftpSessionModel->get();

    // cout << "|=====| FileLogic::write_remote_data() -> full_file_path: " << full_file_path << endl;

    // Always ensure file exists
    bool file_exists = sftp_stat(sftp, full_file_path.c_str()) != nullptr;
    if (!file_exists){
        cout << endl << "|----- FileLogic::write_remote_data() -> !file_exits -----|" << endl;
        sftp_file create = sftp_open(sftp, full_file_path.c_str(), O_WRONLY | O_CREAT, S_IRWXU);
        
        if (!create) {
            cout << "|----- FileLogic::write_remote_data() -> !create -----|" << endl;
            fileModel->set_write_perm(false);
            cerr << "Failed to create remote file " << fileModel->get_name() << endl;
            return;
        }
        // Close after ensuring existence
        sftp_close(create); 
    }
    // If there's nothing to write, stop here
    if (fileModel->get_buffer().empty()) {
        cout << "Skipping remote write on an empty buffer for: " << fileModel->get_name() << endl;
        return;
    }

    // Open again to append
    sftp_file file = sftp_open(sftp, full_file_path.c_str(), O_WRONLY, 0);
    if (!file) {
        fileModel->set_write_perm(false);
        cerr << "No write perms on remote for " << fileModel->get_name() << endl;
        return;
    }
    fileModel->set_write_perm(true);
    
    // Seek to end (emulate append)
    size_t size = this->_get_remote_size(sftpSessionModel, full_file_path);

    if (sftp_seek64(file, size) < 0) {
        sftp_close(file);
        throw runtime_error("Failed to seek to end for: " + full_file_path);
    }

    const vector<byte>& buffer = fileModel->get_buffer();
    int bytes_written = sftp_write(file, reinterpret_cast<const char*>(buffer.data()), buffer.size());

    sftp_close(file);

    if (bytes_written != static_cast<int>(buffer.size()))
        throw runtime_error("Incomplete remote write to: " + fileModel->get_name());

    fileModel->clear_buffer();
}

size_t FileLogic::_get_remote_size(SftpSessionModel *sftpSessionModel, string full_file_path){
    sftp_attributes attr = sftp_stat(sftpSessionModel->get(), full_file_path.c_str());
    if (!attr) {
        throw runtime_error("Failed to stat remote file: " + full_file_path);
    }
    size_t size = attr->size;
    sftp_attributes_free(attr);
    return size;
}

void FileLogic::_update_model_with_data(FileModel *fileModel, const vector<byte> &buffer){
    if (!fileModel->get_buffer().empty())
        throw runtime_error("Data of file " + fileModel->get_name() + " on the local buffer is being overidden");
    fileModel->populate_buffer(buffer);
    if (fileModel->get_bytes_read() > fileModel->get_size()) 
        throw runtime_error("We read magic from: " + fileModel->get_name());
    if (fileModel->get_bytes_read() == fileModel->get_size())
        fileModel->set_fully_read(true);
}

// Yeah I know c code, but this is absurd to do in cpp
time_t FileLogic::get_local_last_modified(FileModel *fileModel) {
    struct stat attr;
    if (stat(fileModel->get_local_file_path().c_str(), &attr) == 0) {
        return attr.st_mtime;
    }
    throw runtime_error("Failed to stat local file: " + fileModel->get_local_file_path());
    return -1;
}

time_t FileLogic::get_remote_last_modified(FileModel *fileModel, SftpSessionModel *sftpSessionModel){
    sftp_attributes attr = sftp_stat(sftpSessionModel->get(), fileModel->get_remote_file_path().c_str());
    if (!attr) throw std::runtime_error("Failed to stat remote file: " + fileModel->get_remote_file_path());
    time_t modified_time = attr->mtime;
    sftp_attributes_free(attr);
    return modified_time;

}


