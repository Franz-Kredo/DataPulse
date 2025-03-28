#ifndef FILE_MODEL_H
#define FILE_MODEL_H

#include <memory>  
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <sys/stat.h> 
#include <sys/stat.h>
using namespace std;
enum class SyncStatus { UNSYNCED, SYNCED, CONFLICT, IN_PROGRESS };

class FileModel {
private:
    string path;
    string name;
    size_t size = 0;
    bool read_perm = false;
    bool write_perm = false;
    bool can_sync = false;
    time_t modified_time = 0;
    mode_t permissions = 0;
    string checksum;
    SyncStatus sync_status = SyncStatus::UNSYNCED;

public:
    FileModel() = default;
    FileModel(const string& path, const struct stat& file_stat);
    
    string get_path() const { return path; }
    string get_name() const { return name; }
    size_t get_size() const { return size; }
    bool get_read_perm() const { return read_perm; }
    bool get_write_perm() const { return write_perm; }
    bool get_can_sync() const { return can_sync; }
    time_t get_modified_time() const { return modified_time; }
    mode_t get_permissions() const { return permissions; }
    string get_checksum() const { return checksum; }
    SyncStatus get_sync_status() const { return sync_status; }

    // Setters
    void set_path(const string& path);
    void set_name(const string& name);
    void set_permissions(mode_t permissions);
    void set_checksum(const string& checksum);
    void set_sync_status(SyncStatus status);

    // Methods
    bool has_changed_since(time_t timestamp) const;
    string to_string() const;
    void update_from_stat(const struct stat& file_stat);
    void calculate_checksum();
};

#endif // FILE_MODEL_H