#include "NetworkLogic.h"
#include <string>
#include <sys/stat.h>
#include <termios.h>


NetworkLogic::NetworkLogic(CommandModel *commandModel)
{
    this->commandModel = commandModel;
    this->sftpSession = this->generate_sftp_session(this->commandModel);
}


// SftpSessionModel *NetworkLogic::generate_sftp_session(CommandModel *commandModel){
//     ssh_session sshSession = ssh_new();
//     if (!sshSession) throw runtime_error("!Failed to create SSH session");

//     try{
//         // Migh need to be cast to c_str()
//         ssh_options_set(sshSession, SSH_OPTIONS_HOST, commandModel->get_host().c_str());
//         ssh_options_set(sshSession, SSH_OPTIONS_USER, commandModel->get_username().c_str());
//         ssh_options_set(sshSession, SSH_OPTIONS_ADD_IDENTITY, commandModel->get_priv_key_path().c_str());
    
//         if (ssh_connect(sshSession) != SSH_OK){
//             ssh_free(sshSession);
//             cout << "Something went wrong while trying to connect via ssh..." << endl;
//             return nullptr;
//         }
//         // if (ssh_userauth_publickey_auto(sshSession, nullptr, nullptr) != SSH_AUTH_SUCCESS) {
//         //     // Maybe add logic to check if the key is encrypted to store in model?
//         //     // That way we only prompt for password if the body of the key is encrypted?
//         //     string passphrase = this->prompt_hidden("Enter passphrase for key '" + commandModel->get_priv_key_path() + "':");
            
//         //     if (ssh_userauth_publickey_auto(sshSession, nullptr, passphrase.c_str()) != SSH_AUTH_SUCCESS) {
//         //         cout << "Something went wrong while trying to connect via ssh..." << endl;
//         //         return nullptr;
//         //     }
//         // }
//         int auth_result = ssh_userauth_publickey_auto(sshSession, nullptr, nullptr);//TODO TESTING MEMORY LEAK 3
            
//             // If direct authentication fails, try with passphrase
//             if (auth_result != SSH_AUTH_SUCCESS) {
//                 string passphrase = this->prompt_hidden("Enter passphrase for key '" + commandModel->get_priv_key_path() + "':");
//                 auth_result = ssh_userauth_publickey_auto(sshSession, nullptr, passphrase.c_str());
                
//                 // If still not successful, clean up and return nullptr
//                 if (auth_result != SSH_AUTH_SUCCESS) {
//                     ssh_disconnect(sshSession);
//                     ssh_free(sshSession);
//                     cout << "Authentication failed..." << endl;
//                     return nullptr;
//                 }
//             }
//     } 
//     catch (...) {
//         // Clean up SSH session in case of any exception
//         ssh_disconnect(sshSession);
//         ssh_free(sshSession);
//         throw;
//     }   

//     return new SftpSessionModel(sshSession);
// }


// Fixed method in NetworkLogic.cpp
//TODO TESTING MEMORY LEAK 4
// Fixed method in NetworkLogic.cpp - Key fix for SftpSessionModel leak
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
        // Clean up resources in case of any exception
        cout << "Exception in generate_sftp_session: " << e.what() << endl;
        
        if (sftpSession) {
            delete sftpSession; // Will handle SSH session cleanup internally
        }
        else if (sshSession) {
            ssh_disconnect(sshSession);
            ssh_free(sshSession);
        }
        
        return nullptr; // Return nullptr instead of re-throwing
    }
}

// Testing func
// void NetworkLogic::list_remote_directory(CommandModel *commandModel) {
//     sftp_dir dir = sftp_opendir(this->sftpSession->get(), commandModel->get_remote_dir_path().c_str());
//     if (!dir) throw runtime_error("Unable to open remote directory: " + commandModel->get_remote_dir_path());

//     sftp_attributes attrs;
//     while ((attrs = sftp_readdir(this->sftpSession->get(), dir)) != nullptr) {
//         if (attrs->type == SSH_FILEXFER_TYPE_REGULAR) {
//             cout << "File: " << attrs->name << "\t\t\t";
//             cout << "  Size: " << attrs->size << " bytes\t";
//             cout << "  Permissions: "
//                       << ((attrs->permissions & S_IRUSR) ? 'r' : '-')
//                       << ((attrs->permissions & S_IWUSR) ? 'w' : '-')
//                       << ((attrs->permissions & S_IXUSR) ? 'x' : '-')
//                       << endl;
//         }
//         sftp_attributes_free(attrs);
//     }
//     sftp_closedir(dir);
// }

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
