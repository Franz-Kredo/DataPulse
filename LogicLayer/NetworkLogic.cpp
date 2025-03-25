#include "NetworkLogic.h"


NetworkLogic::NetworkLogic(CommandModel *commandModel)
{
    ssh_session sshSession = ssh_new();
    if (!sshSession) throw std::runtime_error("Failed to create SSH session");

    // Migh need to be cast to c_str()
    ssh_options_set(sshSession, SSH_OPTIONS_HOST, commandModel->host);
    ssh_options_set(sshSession, SSH_OPTIONS_USER, commandModel->user);
    ssh_options_set(sshSession, SSH_OPTIONS_ADD_IDENTITY, commandModel->key_path);

    if (ssh_connect(sshSession) != SSH_OK)
        throw std::runtime_error("SSH connection failed: " + std::string(ssh_get_error(sshSession)));

    //if (ssh_userauth_publickey_auto(sshSession, nullptr, passphrase.c_str()) != SSH_AUTH_SUCCESS)
    //    throw std::runtime_error("SSH authentication failed: " + std::string(ssh_get_error(sshSession)));
    if (ssh_userauth_publickey_auto(sshSession, nullptr, nullptr) != SSH_AUTH_SUCCESS)
        throw std::runtime_error("SSH authentication failed: " + std::string(ssh_get_error(sshSession)));

    this->sftpSession = new SftpSessionModel(sshSession);
}


// Testing func
void NetworkLogic::list_remote_directory(CommandModel *commandModel) {
    sftp_dir dir = sftp_opendir(this->sftpSession->get(), commandModel->remote_path);
    if (!dir) throw std::runtime_error("Unable to open remote directory: " + commandModel->remote_path);

    sftp_attributes attrs;
    while ((attrs = sftp_readdir(this->sftpSession->get(), dir)) != nullptr) {
        if (attrs->type == SSH_FILEXFER_TYPE_REGULAR) {
            std::cout << "File: " << attrs->name << "\t\t\t";
            std::cout << "  Size: " << attrs->size << " bytes\t";
            std::cout << "  Permissions: "
                      << ((attrs->permissions & S_IRUSR) ? 'r' : '-')
                      << ((attrs->permissions & S_IWUSR) ? 'w' : '-')
                      << ((attrs->permissions & S_IXUSR) ? 'x' : '-')
                      << std::endl;
        }
        sftp_attributes_free(attrs);
    }
    sftp_closedir(dir);
}
