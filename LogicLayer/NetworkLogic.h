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
    CommandModel*commandModel;
    
    NetworkLogic(CommandModel *commandModel);

    ~NetworkLogic() {
        if (sftpSession) {
            delete sftpSession;
            sftpSession = nullptr;
        }
    }
    /**
     * @brief Lists the contents of the remote directory specified in the CommandModel.
     *
     * This method opens the remote directory via the SFTP session and prints details such as
     * the file name, size, and permissions for each regular file found.
     *
     * @param commandModel Instance of CommandModel containing the remote directory path.
     * @throws runtime_error if the remote directory cannot be opened.
     */
    // void list_remote_directory(CommandModel *commandModel); // Possibly not in use since it's a test function


private:
    /**
     * @brief Prompts the user for sensitive input (e.g., a passphrase) without echoing the input to the terminal.
     *
     * This function disables terminal echo, reads the input, and then restores the terminal state.
     *
     * @param prompt The message to display to the user.
     * @return The input string entered by the user.
     */
    string prompt_hidden(string const &prompt);


    SftpSessionModel *generate_sftp_session(CommandModel *commandModel);
};


#endif // NETWORKLOGIC_H

