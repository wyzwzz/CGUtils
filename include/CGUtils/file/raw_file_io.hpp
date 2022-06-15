#pragma once

#include <string>
#include <vector>

namespace wzz::file{

	std::vector<unsigned char> read_raw_file_bytes(const std::string& filename);

	std::string read_txt_file(const std::string& filename);

	void write_raw_file_bytes(const std::string& filename,const void* data,size_t bytes_count);
}
