#ifndef FILE_MODEL_H
#define FILE_MODEL_H

// #include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <iostream>
#include <cstddef> // for std::byte
#include <filesystem>

#include "CommandModel.h"


using namespace std;


using std::byte;


class FileModel {
	string path;
	string remote_path;
    string relative_file_path;
	size_t size;
	bool read_perm;
	bool write_perm;
	bool can_sync = true;
	vector<byte> buffer;
	size_t bytes_read = 0;
	bool fully_read = false;
	bool has_conflict = false;

public:
	//FileModel() = default;
	FileModel(const string& path, const std::string& relative_file_path, std::size_t size);
    	~FileModel() = default;


	string get_relative_path() const {return this->relative_file_path;}
	string get_path() const {return this->path;}	
	string get_remote_path() const {return this->remote_path;}	
	// string get_name() const {return this->name;}	
	string get_name() const {return filesystem::path(this->relative_file_path).filename().string();} // Collect the name using the relative path
    
	size_t get_size() const {return this->size;}	
	bool get_read_perm() const {return this->read_perm;}	
	bool get_write_perm() const {return this->write_perm;}	
	bool get_can_sync() const {return this->can_sync;}	
	vector<byte> get_buffer() const {return this->buffer;}	
	bool get_fully_read() const {return this->fully_read;}	
	size_t get_bytes_read() const {return this->bytes_read;}
	bool get_conflict_bool() const  { return this->has_conflict; }	

	string get_remote_file_path();
	string get_local_file_path();


	void set_path(string path) { this->path = path;}	
	void set_remote_dir_path(string path) { this->remote_path = path;}	
	// void set_name(string name)  { this->name = name;}	
    
    // New set_name() method that replaces the file name portion in relative_file_path
    void set_name(const string& new_name) {
        size_t pos = relative_file_path.find_last_of("/\\");
        if (pos != string::npos) {
            // Preserve the directory structure and replace the file name
            relative_file_path = relative_file_path.substr(0, pos + 1) + new_name;
        } else {
            relative_file_path = new_name;
        }
    }
    
	void set_size(size_t size)  { this->size = size;}	
	void set_read_perm(bool perm)  { this->read_perm = perm;}	
	void set_write_perm(bool perm)  { this->write_perm = perm;}	
	void set_can_sync(bool status)  { this->can_sync = status;}	
	void set_fully_read(bool status)  { this->fully_read = status;}	
	void increase_bytes_read(size_t bytes) { this->bytes_read+=bytes;}
	void set_conflict_bool(bool status)  { this->has_conflict = status;}	


	void populate_buffer(const vector<byte> &data); 
	void clear_buffer();

    /**
     * @brief Generates a FileModel based on local files found using the CommandModel 
     * 
     * @param commandModel: An instance of CommandModel
     * @param relative_file_path: A file path relative to the sync directory
     */
    static FileModel *populate_local_file_model(CommandModel *commandModel, string relative_file_path);	

    /**
     * @brief Generates a FileModel based on remote files found using the CommandModel 
     * 
     * @param commandModel: An instance of CommandModel
     * @param relative_file_path: A file path relative to the sync directory
     * @param file_size: The size of the remote file
     */
    static FileModel *populate_remote_file_model(CommandModel *commandModel, string relative_file_path, size_t file_size);	

private:
};

#endif // FILE_MODEL 



