#ifndef SFTP_SESSION_MODEL_H
#define SFTP_SESSION_MODEL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <libssh/libssh.h>
#include <libssh/sftp.h>


using namespace std;



class SftpSessionModel {

public:
	explicit SftpSessionModel(ssh_session ssh);
    	~SftpSessionModel();
    	sftp_session get() const { return sftp_sesh; }

private:
	sftp_session sftp_sesh;
	ssh_session ssh_sesh;
};

#endif // SFTP_SESSION_MODEL 



