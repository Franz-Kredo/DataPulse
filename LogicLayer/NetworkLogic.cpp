#include "NetworkLogic.h"
#include <string>
#include <sys/stat.h>
#include <termios.h>


NetworkLogic::NetworkLogic(CommandModel *commandModel)
{
    ssh_session sshSession = ssh_new();
    if (!sshSession) throw runtime_error("!Failed to create SSH session");

    // Migh need to be cast to c_str()
    ssh_options_set(sshSession, SSH_OPTIONS_HOST, commandModel->get_host().c_str());
    ssh_options_set(sshSession, SSH_OPTIONS_USER, commandModel->get_username().c_str());
    ssh_options_set(sshSession, SSH_OPTIONS_ADD_IDENTITY, commandModel->get_priv_key_path().c_str());

    if (ssh_connect(sshSession) != SSH_OK)
        throw runtime_error("!SSH connection failed: " + string(ssh_get_error(sshSession)));
    if (ssh_userauth_publickey_auto(sshSession, nullptr, nullptr) != SSH_AUTH_SUCCESS) {
        // Maybe add logic to check if the key is encrypted to store in model?
        // That way we only prompt for password if the body of the key is encrypted?
        string passphrase = this->prompt_hidden("Enter passphrase for key '" + commandModel->get_priv_key_path() + "':");
        if (ssh_userauth_publickey_auto(sshSession, nullptr, passphrase.c_str()) != SSH_AUTH_SUCCESS) {
            throw runtime_error("!SSH authentication failed" + string(ssh_get_error(sshSession)));
    }
}

    this->sftpSession = new SftpSessionModel(sshSession);
    this->commandModel = commandModel;
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
