#include "image_handling.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const char* path)
{
	data = stbi_load(path, &width, &height, &channels, 0);
}

Image::~Image()
{
	stbi_image_free(data);
}