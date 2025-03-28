
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
    /**
     * @brief Reads a chunk of data from a local file which will be used for write_remote_data().
     * 
     * @param fileModel Pointer to the FileModel representing the local file.
     * @param chunk_size Number of bytes to read from the file.
     * @throws runtime_error if the file has already been fully read or if the file cannot be opened.
     */
    void read_local_data(FileModel* model, size_t chunk_size);
    
    
    /**
     * @brief Reads a chunk of data from a remote file via SFTP and updates its FileModel.
     *
     * This method opens the remote file specified in the FileModel and reads up to @p chunk_size bytes 
     * from the current position using the given SFTP session. It then updates the FileModel with the data read.
     *
     * @param fileModel Pointer to the FileModel representing the remote file.
     * @param sftpSessionModel Pointer to the active SftpSessionModel.
     * @param chunk_size The number of bytes to read from the remote file.
     * @throws runtime_error if the file is already fully read or any SFTP operation fails.
     */
    void read_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel, size_t chunk_size);
    

    /**
     * @brief Writes the buffered data from the FileModel to its local file.
     * @param fileModel Pointer to the FileModel representing the local file.
     */
    void write_local_data(FileModel* model);

    
     /**
     * @brief Writes the buffered data in a remote FileModel that was read via read_local_data() from a local file.
     * 
     * @param fileModel Pointer to the FileModel representing the remote file.
     * @param sftpSessionModel Pointer to the active SftpSessionModel.
     * @throws runtime_error if the remote write operation fails or is incomplete.
     * 
     * @details This function ensures that the necessary remote directories exist, creates the remote file if it does not exist,
     * and writes the contents of the FileModel's buffer to the remote file.
     */
    void write_remote_data(FileModel* fileModel, SftpSessionModel *sftpSessionModel);
    
    // Only used to resolve conflicts of files
    time_t get_local_last_modified(FileModel *fileModel);
    time_t get_remote_last_modified(FileModel *fileModel, SftpSessionModel *sftpSessionModel);
private:
    /**
     * @brief Retrieves the size of a remote file via SFTP.
     *
     * @param sftpSessionModel Pointer to the active SftpSessionModel.
     * @param file_name The full path of the remote file.
     * @return size_t The size of the remote file in bytes.
     * @throws runtime_error if the remote file attributes cannot be obtained.
     */
    size_t _get_remote_size(SftpSessionModel *sftpSessionModel, string file_name);

    /**
     * @brief Updates the FileModel with the provided data buffer.
     *
     * This method populates the FileModel's buffer with data read from the file and checks if the total
     * bytes read equals the file size. If so, the file is marked as fully read.
     *
     * @param fileModel Pointer to the FileModel to update.
     * @param buffer A vector of bytes containing the data read.
     * @throws runtime_error if the buffer is not empty or if the total bytes read exceed the file size.
     */
    void _update_model_with_data(FileModel *fileModel, const vector<byte> &buffer);

    /**
     * @brief Ensures that all remote directories for a given file path exist.
     *
     * This method extracts the directory portion from the full remote file path and checks each directory
     * component sequentially. If any directory does not exist, it attempts to create it using SFTP.
     *
     * @param sftpSessionModel Pointer to the active SftpSessionModel.
     * @param full_file_path The full remote file path.
     * @throws runtime_error if directory creation fails.
     */
    void ensure_remote_directories_exist(SftpSessionModel *sftpSessionModel, const string &full_file_path);

};

#endif // FILELOGIC_H
