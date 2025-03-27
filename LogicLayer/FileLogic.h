#include "../Models/FileModel.h"
#include "../Models/SftpSessionModel.h"
#include <iostream>
#include <filesystem>
#include <cstddef>
#include <fstream>
#include <libssh/sftp.h>
using namespace std;


class FileLogic {
public:
    void read_local_data(FileModel* model, std::size_t chunk_size);
    void write_local_data(FileModel* model);

    void read_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel, size_t chunk_size);
    void write_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel);
};

