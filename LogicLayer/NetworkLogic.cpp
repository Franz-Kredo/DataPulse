#include "NetworkLogic.h"
#include <string>
#include <sys/stat.h>
#include <termios.h>


NetworkLogic::NetworkLogic(CommandModel *commandModel)
{
    this->commandModel = commandModel;
    this->sftpSession = this->generate_sftp_session(this->commandModel);
}


SftpSessionModel *NetworkLogic::generate_sftp_session(CommandModel *commandModel) {
    ssh_session sshSession = nullptr;
    SftpSessionModel *sftpSession = nullptr;
    
    try {
        sshSession = ssh_new();
        if (!sshSession) throw runtime_error("!Failed to create SSH session");

        // Set SSH options
        ssh_options_set(sshSession, SSH_OPTIONS_HOST, commandModel->get_host().c_str());
        ssh_options_set(sshSession, SSH_OPTIONS_USER, commandModel->get_username().c_str());
        ssh_options_set(sshSession, SSH_OPTIONS_ADD_IDENTITY, commandModel->get_priv_key_path().c_str());

        // Connect to the server
        if (ssh_connect(sshSession) != SSH_OK) {
            cout << "Something went wrong while trying to connect via ssh..." << endl;
            ssh_free(sshSession);
            return nullptr;
        }

        // Try direct authentication
        int auth_result = ssh_userauth_publickey_auto(sshSession, nullptr, nullptr);
        
        // If direct authentication fails, try with passphrase
        if (auth_result != SSH_AUTH_SUCCESS) {
            string passphrase = this->prompt_hidden("Enter passphrase for key '" + commandModel->get_priv_key_path() + "':");
            auth_result = ssh_userauth_publickey_auto(sshSession, nullptr, passphrase.c_str());
            
            // If still not successful, clean up and return nullptr
            if (auth_result != SSH_AUTH_SUCCESS) {
                ssh_disconnect(sshSession);
                ssh_free(sshSession);
                cout << "Authentication failed..." << endl;
                return nullptr;
            }
        }

        // Create the SFTP session model - within try block to catch exceptions
        sftpSession = new SftpSessionModel(sshSession);
        return sftpSession;
    }
    catch (const std::exception& e) {
        cout << "Exception in generate_sftp_session: " << e.what() << endl;
        
        if (sftpSession) {
            delete sftpSession; 
        }
        else if (sshSession) {
            ssh_disconnect(sshSession);
            ssh_free(sshSession);
        }
        
        return nullptr; 
    }
}

// Built from stackoverflow post, not sure where to put this, it is storing a password after all.
string NetworkLogic::prompt_hidden(string const &prompt) {
    cout << prompt;
    termios oldt, newt;
    string password;
    // Save the current state of the terminal
    tcgetattr(STDIN_FILENO, &oldt);
    // The new state is equal to the old state
    newt = oldt;
    // Disable Echo and apply settings to terminal
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getline(cin, password);
    // Restore the backed up state of the terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
    return password;
}
