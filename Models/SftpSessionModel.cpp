#include "SftpSessionModel.h"
#include <stdexcept>

// SshSessionModel implementation
SshSessionModel::SshSessionModel() {
    session = ssh_new();
    if (!session) {
        throw std::runtime_error("Failed to create SSH session");
    }
}

SshSessionModel::~SshSessionModel() {
    if (session) {
        ssh_disconnect(session);
        ssh_free(session);
    }
}

bool SshSessionModel::is_connected() const {
    return session && ssh_is_connected(session);
}

std::string SshSessionModel::get_last_error() const {
    return session ? ssh_get_error(session) : "No SSH session";
}

// SftpSessionModel implementation
SftpSessionModel::SftpSessionModel(const std::shared_ptr<SshSessionModel>& ssh)
    : ssh_session(ssh) {
    
    if (!ssh_session || !ssh_session->is_connected()) {
        throw std::runtime_error("Invalid SSH session");
    }

    sftp_sesh = sftp_new(ssh_session->get());
    if (!sftp_sesh) {
        throw std::runtime_error("Failed to create SFTP session");
    }

    if (sftp_init(sftp_sesh) != SSH_OK) {
        sftp_free(sftp_sesh);
        throw std::runtime_error("SFTP initialization failed: " + ssh_session->get_last_error());
    }
}

SftpSessionModel::~SftpSessionModel() {
    if (sftp_sesh) {
        sftp_free(sftp_sesh);
    }
}

bool SftpSessionModel::is_valid() const {
    return sftp_sesh != nullptr;
}

std::string SftpSessionModel::get_last_error() const {
    if (ssh_session && sftp_sesh) {
        return ssh_get_error(ssh_session->get());
    }
    return "No active SFTP session";
}