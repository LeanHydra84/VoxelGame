#include "texture_atlas.hpp"
#include <GL/glew.h>
#include <GL/GL.h>

#include <iostream>

struct xypair { int x, y; };

static xypair translate_identifier(Blocks block)
{
	switch (block)
	{
	case Blocks::Stone:
		return { 0, 0 };
	case Blocks::Grass:
		return { 1, 0 };
	case Blocks::Wood:
		return { 0, 1 };
	case Blocks::Sand:
		return { 1, 1 };
	default:
		return { 0, 0 };
	}
}

TextureCoord::TextureCoord(float xmin, float xmax, float ymin, float ymax)
{
	Xmin = xmin;
	Xmax = xmax;
	Ymin = ymin;
	Ymax = ymax;
}

TextureAtlas::TextureAtlas(const char* path, unsigned int stride)
{

	Image image(path);

	width = image.width / stride;
	height = image.height / stride;
	
	Xstride = 1.0f / width;
	Ystride = 1.0f / height;

	std::cout << Xstride << ", " << Ystride << std::endl;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glGenerateMipmap(GL_TEXTURE_2D);

}

TextureCoord TextureAtlas::getTexture(unsigned int identifier)
{
	xypair coords = translate_identifier(static_cast<Blocks>(identifier));
	return getTexture( coords.x, coords.y );
}

TextureCoord TextureAtlas::getTexture(int x, int y)
{
	float xm = Xstride * x;
	float ym = Ystride * y;
	return TextureCoord(xm, xm + Xstride, ym, ym + Ystride);
}

void TextureAtlas::use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}