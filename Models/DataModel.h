#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include "FileModel.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <ctime>
#include <sys/stat.h>

class SyncStrategyInterface;
class ConflictResolutionPolicy;

using namespace std;

class DataModel {
private:
    unordered_map<string, shared_ptr<FileModel>> local_files;
    unordered_map<string, shared_ptr<FileModel>> remote_files;
    shared_ptr<SyncStrategyInterface> sync_strategy;
    shared_ptr<ConflictResolutionPolicy> conflict_resolution;

public:
    void add_local_file(const shared_ptr<FileModel>& file);
    void add_remote_file(const shared_ptr<FileModel>& file);
    void remove_local_file(const string& name);
    void remove_remote_file(const string& name);

    vector<shared_ptr<FileModel>> compare_files() const;
    vector<shared_ptr<FileModel>> get_sync_operations() const;
    vector<shared_ptr<FileModel>> detect_conflicts() const;

    const auto& get_local_files() const { return local_files; }
    const auto& get_remote_files() const { return remote_files; }
    
    void set_sync_strategy(const shared_ptr<SyncStrategyInterface>& strategy);
    void set_conflict_resolution(const shared_ptr<ConflictResolutionPolicy>& resolution);
};

#endif // DATA_MODEL_H