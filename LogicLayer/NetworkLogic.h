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
    private:
    string prompt_hidden(string const &prompt);

};


#endif // NETWORKLOGIC_H

