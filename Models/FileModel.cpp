#include "FileModel.h"
#include <filesystem>

// #if __has_include(<filesystem>)
//     #include <filesystem>
//     namespace fs = std::filesystem;
// #elif __has_include(<experimental/filesystem>)
//     #include <experimental/filesystem>
//     namespace fs = std::experimental::filesystem;
// #else
//     #error "Missing the <filesystem> header."
// #endif


FileModel::FileModel(const string& path, const std::string& name, std::size_t size)
        : path(path), name(name), size(size)
	{

	}

void FileModel::populate_buffer(vector<byte> data){ 
    this->buffer = data;
}	
void FileModel::clear_buffer() {
    fill(this->buffer.begin(), this->buffer.end(), static_cast<byte>(0));
    this->buffer.clear();
    this->buffer.resize(0);
}


FileModel *FileModel::populate_file_model(CommandModel *commandModel, string filename){
    string local_path = commandModel->get_local_path();
    string remote_path = commandModel->get_remote_path();
    // size_t file_size = filesystem::file_size(local_path + filename);
    size_t file_size = filesystem::file_size(local_path + "/" + filename);

    FileModel *fileModel = new FileModel(local_path, filename, file_size);
    fileModel->set_remote_path(remote_path);

    cout << "I------I  file_size is " << file_size <<"  I------I" << endl;


    return fileModel;
}