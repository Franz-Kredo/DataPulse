#include "TestHandler.h"
#include <fstream>
#include <iostream>
#include "../../Models/CommandModel.h"
#include "../../Models/SftpSessionModel.h"
#include "../../LogicLayer/NetworkLogic.h"
const string TEST_PATH = "Tests/LexiTests";


void TestHandler::test_local_file_ops(){
	string path = filesystem::current_path();
	    path+="/";
	    path+=TEST_PATH;
	string test_filename = "bjarni_elskar_sultu.txt";
   	size_t test_size = filesystem::file_size(path + test_filename);

    	FileModel fileModel(path, test_filename, test_size);

    	FileLogic logic;
    	
    	try {
    	    	logic.read_local_data(&fileModel, 160); 
    	    	logic.write_local_data(&fileModel);    
    	    	cout << "Read and write completed successfully, catting outp" << endl;
    	} catch (const exception& ex) {
    	    	cerr << "Exception: " << ex.what() << endl;
    	}
	    cout << "READ FILE CONTENTS:" << endl;
	    ifstream file(path+test_filename, ios::binary);
	    string line;
	    while (getline(file, line)) {
    	        cout << line << endl;
    	    }

    	    file.close();
}


void TestHandler::test_remote_file_read(SftpSessionModel *sftpSessionModel, CommandModel *commandModel) {
    string remote_path = commandModel->get_remote_dir_path();
    string remote_filename = "super_duper_secret_password_storage.txt";
    string dummy_size_reference = remote_path + "/" +remote_filename;
	cout << dummy_size_reference << endl;
    sftp_attributes attr = sftp_stat(sftpSessionModel->get(), dummy_size_reference.c_str());
    if (!attr) {
        cerr << "Failed to stat remote file." << endl;
        return;
    }

    size_t test_size = attr->size;
    sftp_attributes_free(attr);

    FileModel fileModel("/home/lexman/ru_s25/forc/DataPulse", remote_filename, test_size);
    fileModel.set_remote_dir_path(remote_path);
    FileLogic logic;

    try {
        logic.read_remote_data(&fileModel, sftpSessionModel, 64); 
    	logic.write_local_data(&fileModel);    
        cout << "Remote read succeeded. Dumping buffer contents:" << endl;

        const vector<byte>& buffer = fileModel.get_buffer();
        for (byte b : buffer) cout << static_cast<char>(b);
        cout << endl;
        logic.read_remote_data(&fileModel, sftpSessionModel, 64); 
    	logic.write_local_data(&fileModel);    

    } catch (const exception& ex) {
        cerr << "Exception during remote read: " << ex.what() << endl;
    }
}


void TestHandler::test_remote_file_write(SftpSessionModel* sftpSessionModel, CommandModel* commandModel) {
    string local_path = "/home/lexman/ru_s25/forc/DataPulse/Tests/LexiTests";
    string local_filename = "test.txt";
    string remote_path = commandModel->get_remote_dir_path();
    string remote_filename = "test.txt";
    size_t test_size = filesystem::file_size(local_path + "/" + local_filename);
    FileModel fileModel(local_path, local_filename, test_size);
    fileModel.set_remote_dir_path(remote_path);
    fileModel.set_name(remote_filename); 
    FileLogic logic;
    try {
        logic.read_local_data(&fileModel, 150); 
        logic.write_remote_data(&fileModel, sftpSessionModel); 
        cout << "Write to remote succeeded. Verifying contents..." << endl;

	fileModel.set_fully_read(false);
        logic.write_remote_data(&fileModel, sftpSessionModel); 
        const vector<byte>& buffer = fileModel.get_buffer();
        for (byte b : buffer) cout << static_cast<char>(b);
        cout << endl;
    } catch (const exception& ex) {
        cerr << "Exception during remote write test: " << ex.what() << endl;
    }
}


void TestHandler::test_network_connection(){
    CommandModel command;
    command.set_host("sty.frostbyte.is");
    command.set_username("alexanderme22");
    command.set_local_path("/home/lexman/");
    command.set_remote_path("/home/alexanderme22");
    command.set_priv_key_path("/home/lexman/.ssh/sty1_server");

    try {
        NetworkLogic *netLogic = new NetworkLogic(&command);
	
	SftpSessionModel *sftpSessionModel = netLogic->sftpSession;

	this->test_remote_file_write(sftpSessionModel, &command);

    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

