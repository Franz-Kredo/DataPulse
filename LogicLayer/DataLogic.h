
#ifndef DATALOGIC_H
#define DATALOGIC_H

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

    DataLogic(FileLogic *fileLogic, NetworkLogic *networkLogic);
    
    // DataLogic(FileLogic *fileLogic);

    //---- Methods ----//

    /*
     * @brief Uses the CommandModel to read from local and remote directories using internal methods
     *
     * @param commandModel: Takes in the an instance of CommandModel made from args from the terminal (mostly to get local path)
     * @returns DataModel: 
     */
    DataModel *read_data(CommandModel *commandModel);
    /*
     * @brief Uses the DataModel to write at local and remote directories using internal methods
     *
     * @param dataModel: Takes in the an instance of DataModel made from DataLogic::read_data()
     * @returns response: A message if it has succeeded or not
     */
    PlaceholderModel *write_data(PlaceholderModel *dataModel);

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
    unordered_map<string, FileModel>  *read_remote(CommandModel *commandModel);    

    /*
     * @brief Uses the DataModel determine which files are syncable, then return the dataModel with syncable files marked
     *
     * @param dataModel: Takes in the an instance of DataModel made from args from the terminal
     * @returns DataModel: [using placeholder until FileModel is rdy]
     */
    DataModel *mark_syncable_files(DataModel *dataModel);



    //=====================================================//
    //=================== WRITING FIlES ===================//
    //=====================================================//

    /*
     * @brief Uses the DataModel to write to local directories
     *
     * @param commandModel: Takes in the an instance of DataModel
     * @returns ret_msg: A string message of xxx
     */
    PlaceholderModel *write_local(PlaceholderModel *dataModel);

    /*
     * @brief Uses the DataModel to write to remote directories
     *
     * @param commandModel: Takes in the an instance of DataModel
     * @returns ret_msg: A string message of xxx
     */
    PlaceholderModel *write_remote(PlaceholderModel *dataModel);

    //=========================================================//
    //=================== TEMPLATED METHODS ===================//
    //=========================================================//

    void populate_local_file_model(CommandModel *commandModel);


};

#endif // DATALOGIC_H