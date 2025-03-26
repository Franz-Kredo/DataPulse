#include "FileModel.h"


FileModel::FileModel(const string& path, const std::string& name, std::size_t size)
        : path(path), name(name), size(size), buffer(size)
	{

	}

void FileModel::read_to_buffer(vector<byte> data){ 
    this->bytes_read += data.size();
    this->buffer = data;

}	
