#include "FileLogic.h"
#include <charconv>
#include <cstddef>
#include <filesystem>
#include <ios>
#include <iostream>
#include <string>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <fcntl.h>
#include <vector>

// Good chunk size, according to docs 16kb

void FileLogic::read_local_data(FileModel* fileModel, size_t chunk_size){
    // cout << "=========================================================\nFileLogic::read_local_data() -> Reading the actual data\n=========================================================\n" << endl;

    if (fileModel->get_fully_read()) throw runtime_error("!Attempting to read a fully read file: " + fileModel->get_relative_path());
    string file_name = fileModel->get_path() + "/" + fileModel->get_relative_path(); 
    //file_name = "FileLogic.h"; // should be repalace
    ifstream file(file_name, ios::binary);
    if (!file.is_open()){
        fileModel->set_read_perm(false);
        cout << "No read perms on local: " + fileModel->get_relative_path() << endl;
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
    string file_name = fileModel->get_path() + "/" + fileModel->get_relative_path(); 


    // Create directory if it doesn't exist
    filesystem::path filePath(file_name);
    filesystem::path dirPath = filePath.parent_path();
    if (!filesystem::exists(dirPath)){
        filesystem::create_directories(dirPath);
    }
    
    if (!filesystem::exists(file_name)){
        ofstream create(file_name, ios::binary);
        create.close();
    }

    if (fileModel->get_buffer().size() == 0){
        cout << "Skipping write on an empty buffer for:" + fileModel->get_relative_path() << endl;
        return;
    }

    ofstream file(file_name, ios::binary | ios::app);
    if (!file.is_open()){
        fileModel->set_write_perm(false);
        cout << "No write perms on " + fileModel->get_relative_path() << endl;
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
        throw runtime_error("Attempting to read a fully read file: " + fileModel->get_relative_path());

    sftp_session sftp = sftpSessionModel->get();
    string file_name = fileModel->get_remote_path() + "/" + fileModel->get_relative_path(); 
    int access_type = O_RDONLY;

    sftp_file file = sftp_open(sftp, file_name.c_str(), access_type, 0);
    if (!file){
        fileModel->set_read_perm(false);
        cout << "No read perms on remote: " + fileModel->get_relative_path() << endl;
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
    string full_file_path = fileModel->get_remote_path() + "/" + fileModel->get_relative_path();
    sftp_session sftp = sftpSessionModel->get();

    // cout << "|=====| FileLogic::write_remote_data() -> full_file_path: " << full_file_path << endl;
    // Make sure that remote directories exist
    try {
        this->ensure_remote_directories_exist(sftpSessionModel, full_file_path);
    } catch (const std::exception &e) {
        fileModel->set_write_perm(false);
        cerr << "Failed to create remote directories: " << e.what() << endl;
        return;
    }

    // Always ensure file exists
    bool file_exists = sftp_stat(sftp, full_file_path.c_str()) != nullptr;
    if (!file_exists){
        cout << endl << "|----- FileLogic::write_remote_data() -> !file_exits -----|" << endl;
        sftp_file create = sftp_open(sftp, full_file_path.c_str(), O_WRONLY | O_CREAT, S_IRWXU);
        
        if (!create) {
            cout << "|----- FileLogic::write_remote_data() -> !create -----|" << endl;
            fileModel->set_write_perm(false);
            cerr << "Failed to create remote file " << fileModel->get_relative_path() << endl;
            return;
        }
        // Close after ensuring existence
        sftp_close(create); 
    }
    // If there's nothing to write, stop here
    if (fileModel->get_buffer().empty()) {
        cout << "Skipping remote write on an empty buffer for: " << fileModel->get_relative_path() << endl;
        return;
    }

    // Open again to append
    sftp_file file = sftp_open(sftp, full_file_path.c_str(), O_WRONLY, 0);
    if (!file) {
        fileModel->set_write_perm(false);
        cerr << "No write perms on remote for " << fileModel->get_relative_path() << endl;
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
        throw runtime_error("Incomplete remote write to: " + fileModel->get_relative_path());

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
        throw runtime_error("Data of file " + fileModel->get_relative_path() + " on the local buffer is being overidden");
    fileModel->populate_buffer(buffer);
    if (fileModel->get_bytes_read() > fileModel->get_size()) 
        throw runtime_error("We read magic from: " + fileModel->get_relative_path());
    if (fileModel->get_bytes_read() == fileModel->get_size())
        fileModel->set_fully_read(true);
}



void FileLogic::ensure_remote_directories_exist(SftpSessionModel *sftpSessionModel, const string &full_file_path) {
    sftp_session sftp = sftpSessionModel->get();
    // Get the directory portion of the file path.
    size_t pos = full_file_path.find_last_of("/");
    if (pos == string::npos) return; // No directory part
    string remote_dir = full_file_path.substr(0, pos);

    // Split the directory path into its components.
    vector<string> components;
    size_t start = 0;
    if (!remote_dir.empty() && remote_dir[0] == '/') {
        start = 1; // skip leading '/'
    }
    while (start < remote_dir.size()) {
        size_t end = remote_dir.find('/', start);
        if (end == string::npos) {
            components.push_back(remote_dir.substr(start));
            break;
        } else {
            components.push_back(remote_dir.substr(start, end - start));
            start = end + 1;
        }
    }

    // Build the directory path step-by-step.
    string currentPath;
    if (!remote_dir.empty() && remote_dir[0] == '/') {
        currentPath = "/";
    }
    for (const auto &comp : components) {
        if (currentPath != "/" && !currentPath.empty()) {
            currentPath += "/";
        }
        currentPath += comp;
        // Check if the directory exists.
        if (sftp_stat(sftp, currentPath.c_str()) == nullptr) {
            // Create the directory
            if (sftp_mkdir(sftp, currentPath.c_str(), S_IRWXU) != SSH_OK) {
                throw runtime_error("Failed to create remote directory: " + currentPath);
            }
        }
    }
}