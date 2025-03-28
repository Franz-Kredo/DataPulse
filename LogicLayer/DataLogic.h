
#ifndef DATALOGIC_H
#define DATALOGIC_H

#include "ConflictLogic.h"
#include "FileLogic.h"
#include "NetworkLogic.h"

#include "../Models/CommandModel.h"
#include "../Models/FileModel.h"
#include "../Models/DataModel.h"
#include "../Models/PlaceholderModel.h"


#include <unordered_map>

class DataLogic{
public:
    FileLogic *fileLogic;
    NetworkLogic *networkLogic;
    ConflictLogic *conflictLogic;

    DataLogic(FileLogic *fileLogic, NetworkLogic *networkLogic, ConflictLogic *conflictLogic);
    
    // DataLogic(FileLogic *fileLogic);

    //---- Methods ----//

    bool compare_synced_data(DataModel *dataModel, CommandModel *commandModel);



    /*
     * @brief Takes in CommandModel and collects local and remote file info and imports it to DataModel as FileModels
     *
     * @param commandModel: Takes in the an instance of CommandModel
     * @returns DataModel: 
     */
    DataModel *collect_files(CommandModel *commandModel);

    /*
     * @brief Uses the DataModel to write at local and remote directories using internal methods
     *
     * @param dataModel: Takes in the an instance of DataModel made from DataLogic::collect_files()
     * @returns response: A message if it has succeeded or not
     */
    DataModel *write_data(DataModel *dataModel, CommandModel *commandModel);



    string compute_md5_local(const std::string& file_path);
    string compute_md5_remote(SftpSessionModel *sftpSessionModel, const std::string& remote_path);
        

private:
    //=====================================================//
    //=================== READING FIlES ===================//
    //=====================================================//
    /*
     * @brief Uses the CommandModel to read from local directories, and store them in an unordered map of key:name(str) and value:FileModel
     *
     * @param commandModel: Takes in the an instance of CommandModel made from args from the terminal (mostly to get local path)
     * @returns unordered_map<string, FileModel>:
     */
    unordered_map<string, FileModel> *read_local(CommandModel *commandModel);

    /*
     * @brief Uses the CommandModel to read from remote directories via SFTP, and store them in an unordered map of key:name(str) and value:FileModel
     *
     * @param commandModel: Takes in the an instance of CommandModel made from args from the terminal
     * @returns unordered_map<string, FileModel>: 
     */
    // unordered_map<string, FileModel>  *read_remote(CommandModel *commandModel);    

    /*
     * @brief Uses the DataModel determine which files are syncable, then return the dataModel with syncable files marked
     *
     * @param dataModel: Takes in the an instance of DataModel made from args from the terminal
     * @returns DataModel: [using placeholder until FileModel is rdy]
     */
    DataModel *mark_syncable_files(DataModel *dataModel, CommandModel *commandModel);



    //=====================================================//
    //=================== WRITING FIlES ===================//
    //=====================================================//

    /*
     * @brief Uses the DataModel to write to local directories
     *
     * @param commandModel: Takes in the an instance of DataModel
     * @returns ret_msg: A string message of xxx
     */
    DataModel *write_local(DataModel *dataModel, CommandModel *commandModel);

    /*
     * @brief Uses the DataModel to write to remote directories
     *
     * @param commandModel: Takes in the an instance of DataModel
     * @returns ret_msg: A string message of xxx
     */
    DataModel *write_remote(DataModel *dataModel, CommandModel *commandModel);


    //=============================================================//
    //=================== PRIVATE FILES METHODS ===================//
    //=============================================================//

    /*
     * @brief Collects local files as a vector of FileModels using the CommandModel
     * @param commandModel: Instance of CommandModel
     * @returns vector<FileModel> for local files
     */
    vector<FileModel*> *collect_local_files(CommandModel *commandModel);

    /*
     * @brief Collects remote files as a vector of FileModels using the CommandModel
     * @param commandModel: Instance of CommandModel
     * @returns vector<FileModel> for remote files
     */
    vector<FileModel*> *collect_remote_files(CommandModel *commandModel);

    /*
     * @brief Recursively traverses the remote directory structure to collect regular files.
     *
     * @param sftp The active SFTP session used for remote file operations.
     * @param base_path The base remote directory path used to compute relative paths.
     * @param current_path The current remote directory path.
     * @param commandModel Instance of CommandModel.
     * @param files Reference to a vector<FileModel*>.
     * 
     * @returns Nothing. The collected FileModels are appended to the provided vector.
     */
    void collect_remote_files_recursive(sftp_session sftp, const string &base_path, const string &current_path, CommandModel *commandModel, vector<FileModel*> &files);






};

#endif // DATALOGIC_H
