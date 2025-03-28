#ifndef COMMAND_MODEL_H
#define COMMAND_MODEL_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <sys/stat.h>
#include <iostream>
#include <libssh/libssh.h>
#include <libssh/sftp.h>


using namespace std;

enum class LogLevel { NONE, BASIC, VERBOSE, DEBUG };

class CommandModel {
	string host;
	string username;
	string remote_path;
	string local_path;
	string priv_key_path; 
    bool merge = false;
    int port = 22;
    int timeout_seconds = 30;
    bool recursive = false;
    bool preserve_permissions = true;
    LogLevel log_level = LogLevel::BASIC;

public:
	string get_host() const { return host; }
	string get_username() const { return username; }
	string get_remote_path() const { return remote_path; }
	string get_local_path() const { return local_path; }
	string get_priv_key_path() const { return priv_key_path; }
	bool get_merge() const { return merge; }
	int get_port() const { return port; }
	int get_timeout() const { return timeout_seconds; }
	bool get_recursive() const { return recursive; }
	bool get_preserve_permissions() const { return preserve_permissions; }
	LogLevel get_log_level() const { return log_level; }

	// Setters with validation
	void set_host(const string& host);
	void set_username(const string& username);
	void set_remote_path(const string& path);
	void set_local_path(const string& path);
	void set_priv_key_path(const string& path);
	void set_merge(bool merge) { this->merge = merge; }
	void set_port(int port);
	void set_timeout(int seconds);
	void set_recursive(bool recursive) { this->recursive = recursive; }
	void set_preserve_permissions(bool preserve) { preserve_permissions = preserve; }
	void set_log_level(LogLevel level) { log_level = level; }

	// Utility methods
	bool validate() const;
	string to_string() const;
	string get_connection_string() const;
	string get_ssh_command() const;
};

#endif // COMMAND_MODEL 



