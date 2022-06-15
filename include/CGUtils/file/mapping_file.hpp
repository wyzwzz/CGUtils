#pragma once
#include <string>
namespace wzz::file{

	enum class FileAccess{
		read,
		write,
		read_write
	};
	enum class MapAccess{
		read_only,
		read_write
	};

	class mapping_file_t{
	public:
		virtual ~mapping_file_t() = default;

		virtual bool open(const std::string& filename,size_t filesize,FileAccess file_access,MapAccess map_access);

		virtual void* map(size_t offset,size_t size);

		virtual void unmap(void* p);

		virtual bool flush();

		virtual bool flush(void* p,size_t size);

		virtual bool close();
	};


}