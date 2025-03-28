
#ifndef FILELOGIC_H
#define FILELOGIC_H


#include "../Models/FileModel.h"
#include "../Models/SftpSessionModel.h"
#include <iostream>
#include <filesystem>
#include <cstddef>
#include <fstream>
#include <libssh/sftp.h>
#include <chrono>
using namespace std;


class FileLogic {
    const size_t MAX_BUFFER_SIZE = 16384;

public:
    void read_local_data(FileModel* model, std::size_t chunk_size);
    void write_local_data(FileModel* model);

    void read_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel, size_t chunk_size);
    void write_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel);
    // Only used to resolve conflicts of files
    time_t get_local_last_modified(FileModel *fileModel);
    time_t get_remote_last_modified(FileModel *fileModel, SftpSessionModel *sftpSessionModel);
private:
    size_t _get_remote_size(SftpSessionModel *sftpSessionModel, string file_name);
    void _update_model_with_data(FileModel *fileModel, const vector<byte> &buffer);


};

#endif // FILELOGIC_H
