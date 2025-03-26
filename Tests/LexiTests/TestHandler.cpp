#include "TestHandler.h"
#include <fstream>
const string TEST_PATH = "Tests/LexiTests/";
void TestHandler::test_local_file_ops(){
	string path = filesystem::current_path();
	    path+="/";
	    path+=TEST_PATH;
	string test_filename = "bjarni_elskar_sultu.txt";
   	size_t test_size = filesystem::file_size(path + test_filename);

    	FileModel fileModel(path, test_filename, test_size);

    	FileLogic logic;
    	
    	try {
    	    	logic.read_local_data(&fileModel, 16); // Read 16 bytes
    	    	logic.write_local_data(&fileModel);    // Write buffer to "test_write_to.txt"
    	    	logic.read_local_data(&fileModel, 8); // Read 16 bytes
    	    	logic.write_local_data(&fileModel);    // Write buffer to "test_write_to.txt"
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

