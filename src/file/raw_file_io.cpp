//
// Created by wyz on 2022/6/15.
//
#include <CGUtils/file/raw_file_io.hpp>
#include <fstream>
#include <sstream>
namespace wzz::file{

std::vector<unsigned char> read_raw_file_bytes(const std::string& filename){
	std::ifstream in(filename,std::ios::in | std::ios::binary | std::ios::ate);
	if(!in.is_open()){
		//todo replace with file open exception
		throw std::runtime_error("open file failed: " + filename);
	}
	size_t file_size = in.tellg();
	std::vector<unsigned char> buffer(file_size);
	in.seekg(0,std::ios::beg);
	in.read(reinterpret_cast<char*>(buffer.data()),file_size);
	in.close();
	return buffer;
}

std::string read_txt_file(const std::string& filename){
	std::ifstream in(filename,std::ios::in);
	if(!in.is_open()){
		//todo replace with file open exception
		throw std::runtime_error("open file failed: " + filename);
	}
	std::stringstream ss;
	ss << in.rdbuf();
	return ss.str();
}

void write_raw_file_bytes(const std::string& filename,const void* data,size_t bytes_count){
	std::ofstream out(filename,std::ios::binary|std::ios::beg);
	if(!out.is_open()){
		//todo replace with file open exception
		throw std::runtime_error("write file failed: " + filename);
	}
	out.write(reinterpret_cast<const char*>(data),bytes_count);
	out.close();
}


}