#include "FileModel.h"


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
