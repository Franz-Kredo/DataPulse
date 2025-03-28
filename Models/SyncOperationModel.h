#ifndef SYNC_OPERATION_MODEL_H
#define SYNC_OPERATION_MODEL_H
#include <memory>  // for shared_ptr
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <sys/stat.h>  // for mode_t
#include "FileModel.h"
#include <chrono>

enum class OperationType { UPLOAD, DOWNLOAD, DELETE_REMOTE, DELETE_LOCAL, MERGE };
enum class OperationStatus { PENDING, IN_PROGRESS, COMPLETED, FAILED };

using namespace std;


class SyncOperationModel {
private:
    OperationType type;
    shared_ptr<FileModel> file;
    OperationStatus status = OperationStatus::PENDING;
    chrono::system_clock::time_point timestamp;
    string error_message;

public:
    SyncOperationModel(OperationType type, const shared_ptr<FileModel>& file);
    
    // Getters
    OperationType get_type() const { return type; }
    shared_ptr<FileModel> get_file() const { return file; }
    OperationStatus get_status() const { return status; }
    auto get_timestamp() const { return timestamp; }
    string get_error_message() const { return error_message; }
    
    // Setters
    void set_status(OperationStatus status);
    void set_error_message(const string& message);
    
    // Utility
    string to_string() const;
};

#endif // SYNC_OPERATION_MODEL_H