#ifndef COMMAND_MODEL_H
#define COMMAND_MODEL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <libssh/libssh.h>
#include <libssh/sftp.h>


using namespace std;



class CommandModel {
	string host;
	string username;
	string remote_path;
	string local_path;
	string priv_key_path; 
	string flag; 
	bool merge;

public:
	CommandModel() = default;

    	~CommandModel() = default;

	string get_host() const {return this->host;}	
	string get_username() const {return this->username;}	
	string get_remote_path() const {return this->remote_path;}	
	string get_local_path() const {return this->local_path;}	
	string get_priv_key_path() const {return this->priv_key_path;}	
	string get_flag() const {return this->flag;}	
	bool get_merge() const {return this->merge;}	

	void set_host(string host) { this->host = host;}	
	void set_username(string username) {this->username = username;}	
	void set_remote_path(string remote_path) {this->remote_path = remote_path;}	
	void set_local_path(string local_path) {this->local_path = local_path;}	
	void set_priv_key_path(string priv_key_path) {this->priv_key_path = priv_key_path;}	
	void set_flag(string flag) {this->flag = flag;}	
	void set_merge(bool merge) {this->merge = merge;}	


private:
};

#endif // COMMAND_MODEL 



