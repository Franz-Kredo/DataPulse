#ifndef NETWORKLOGIC_H
#define NETWORKLOGIC_H

#include "../Models/SftpSessionModel.h"
#include "../Models/CommandModel.h"
#include <iostream>
#include <string>
using std::string;


class NetworkLogic{
    public:
    SftpSessionModel *sftpSession;

    
    NetworkLogic(CommandModel *commandModel);

    void list_remote_directory(CommandModel *commandModel);


    // string sftp_read_from_remote();
    // string sftp_write_to_remote();

    // sftp_read_from_remote
    private:
    string prompt_hidden(string const &prompt);

};


#endif // NETWORKLOGIC_H

