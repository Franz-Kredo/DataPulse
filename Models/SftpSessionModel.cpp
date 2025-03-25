
#include "SftpSessionModel.h"



// Constructor
SftpSessionModel::SftpSessionModel(ssh_session ssh)
    : ssh_sesh(ssh), sftp_sesh(nullptr)
{
    if (!ssh_sesh) throw std::runtime_error("Invalid SSH session");

    sftp_sesh = sftp_new(ssh_sesh);
    if (!sftp_sesh) throw std::runtime_error("Failed to create SFTP session");

    if (sftp_init(sftp_sesh) != SSH_OK)
        throw std::runtime_error("SFTP initialization failed: " + std::string(ssh_get_error(ssh_sesh)));
}


// Decosntructor
SftpSessionModel::~SftpSessionModel() {
    if (sftp_sesh) sftp_free(sftp_sesh);
    if (ssh_sesh) {
        ssh_disconnect(ssh_sesh);
        ssh_free(ssh_sesh);
    }
}

