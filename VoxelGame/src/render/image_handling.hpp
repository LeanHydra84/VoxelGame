#ifndef _IMAGE_HANDLING_HPP_
#define _IMAGE_HANDLING_HPP_

struct Image
{
	int width, height, channels;
	unsigned char* data;

	Image(const char* path);
	~Image();
};

#endif