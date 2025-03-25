#ifndef NETWORKLOGIC_H
#define NETWORKLOGIC_H

#include "../Models/SftpSessionModel.h"
#include <iostream>
#include <string>
class CommandModel{}; // replace with CommandModel include once ready
using std::string;


class NetworkLogic{
    public:
    SftpSessionModel *sftpSession;
    NetworkLogic(CommandModel *commandModel);

    void list_remote_directory(CommandModel *commandModel);
    //private:

};


#endif // NETWORKLOGIC_H

