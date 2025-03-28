#ifndef SFTP_SESSION_MODEL_H
#define SFTP_SESSION_MODEL_H

#include <memory>  // for shared_ptr
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <sys/stat.h>  // for mode_t
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <memory>
#include <string>

class SshSessionModel {
private:
    ssh_session session;
    
public:
    explicit SshSessionModel();
    ~SshSessionModel();
    
    ssh_session get() const { return session; }
    bool is_connected() const;
    std::string get_last_error() const;
};

class SftpSessionModel {
private:
    sftp_session sftp_sesh;
    std::shared_ptr<SshSessionModel> ssh_session;
    
public:
    explicit SftpSessionModel(const std::shared_ptr<SshSessionModel>& ssh);
    ~SftpSessionModel();
    
    sftp_session get() const { return sftp_sesh; }
    bool is_valid() const;
    std::string get_last_error() const;
};

#endif // SFTP_SESSION_MODEL_H