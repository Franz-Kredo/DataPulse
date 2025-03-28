#include "FileModel.h"
#include <filesystem>


FileModel::FileModel(const string& path, const string& relative_file_path, size_t size)
        : path(path), relative_file_path(relative_file_path), size(size)
	{


}

string FileModel::get_remote_file_path(){
    // return this->get_remote_path() + "/" + this->get_name();
    cout << "GETTING FULL REMOTE FILE PATH" << endl;
    cout << this->get_remote_path() << "/" << this->get_relative_path() << endl;
    return this->get_remote_path() + "/" + this->get_relative_path();
}
string FileModel::get_local_file_path(){
    // return this->get_path() + "/" + this->get_name();
    // cout << "GETTING FULL LOCAL FILE PATH" << endl;
    cout << this->get_path() << "/" << this->get_relative_path() << endl;

    return this->get_path() + "/" + this->get_relative_path();
}

void FileModel::populate_buffer(const vector<byte>& data){ 
    this->buffer = data;
}	
void FileModel::clear_buffer() {
    fill(this->buffer.begin(), this->buffer.end(), static_cast<byte>(0));
    this->buffer.clear();
    this->buffer.resize(0);
}


FileModel *FileModel::populate_local_file_model(CommandModel *commandModel, string relative_file_path){
    string local_dir_path = commandModel->get_local_dir_path();
    string remote_dir_path = commandModel->get_remote_dir_path();
    // size_t file_size = filesystem::file_size(local_dir_path + relative_file_path);
    size_t file_size = filesystem::file_size(local_dir_path + "/" + relative_file_path);

    FileModel *fileModel = new FileModel(local_dir_path, relative_file_path, file_size);
    fileModel->set_remote_dir_path(remote_dir_path);

    // cout << "I------I  file_size is " << file_size <<"  I------I" << endl;


    return fileModel;
}

FileModel *FileModel::populate_remote_file_model(CommandModel *commandModel, string relative_file_path, size_t file_size){
    string local_dir_path = commandModel->get_local_dir_path();
    string remote_dir_path = commandModel->get_remote_dir_path();

    FileModel *fileModel = new FileModel(local_dir_path, relative_file_path, file_size);
    fileModel->set_remote_dir_path(remote_dir_path);

    // cout << "I------I  file_size is " << file_size <<"  I------I" << endl;


    return fileModel;
}
