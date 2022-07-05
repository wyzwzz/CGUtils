#include <CGUtils/image/save_image.hpp>
namespace {
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../ext/stb_image_write.h"
}
namespace wzz::image
{

void save_rgb_to_png_file( const std::string &filename, const math::tensor_t<math::color3b, 2> &data )
{
	stbi_write_png( filename.c_str(), data.shape()[ 0 ], data.shape()[ 1 ], 3, data.raw_data(), 0 );
}

}