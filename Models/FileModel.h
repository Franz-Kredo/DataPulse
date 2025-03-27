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

#include "CommandModel.h"


using namespace std;


using std::byte;


class FileModel {
	string path;
	string remote_path;
	string name;
	size_t size;
	bool read_perm;
	bool write_perm;
	bool can_sync = false;
	vector<byte> buffer;
	size_t bytes_read = 0;
	bool fully_read = false;
	bool has_conflict = false;

public:
	//FileModel() = default;
	FileModel(const string& path, const std::string& name, std::size_t size);
    	~FileModel() = default;


	string get_path() const {return this->path;}	
	string get_remote_path() const {return this->remote_path;}	
	string get_name() const {return this->name;}	
	size_t get_size() const {return this->size;}	
	bool get_read_perm() const {return this->read_perm;}	
	bool get_write_perm() const {return this->write_perm;}	
	bool get_can_sync() const {return this->can_sync;}	
	vector<byte> get_buffer() const {return this->buffer;}	
	bool get_fully_read() const {return this->fully_read;}	
	size_t get_bytes_read() const {return this->bytes_read;}
	bool get_conflict_bool() const  { return this->has_conflict; }	


	void set_path(string path) { this->path = path;}	
	void set_remote_path(string path) { this->remote_path = path;}	
	void set_name(string name)  { this->name = name;}	
	void set_size(size_t size)  { this->size = size;}	
	void set_read_perm(bool perm)  { this->read_perm = perm;}	
	void set_write_perm(bool perm)  { this->write_perm = perm;}	
	void set_can_sync(bool status)  { this->can_sync = status;}	
	void set_fully_read(bool status)  { this->fully_read = status;}	
	void increase_bytes_read(size_t bytes) { this->bytes_read+=bytes;}
	void set_conflict_bool(bool status)  { this->has_conflict = status;}	


	void populate_buffer(const vector<byte> &data); 
	void clear_buffer();

    /*
     * @brief Generates a FileModel based on CommandModel [NOTE: The is a default name filename for testing ]
     */
    static FileModel *populate_local_file_model(CommandModel *commandModel, string filename);	
    static FileModel *populate_remote_file_model(CommandModel *commandModel, string filename, size_t file_size);	

private:
};

#endif // FILE_MODEL 



