#include "image_handling.hpp"

enum class Blocks
{
	Air = 0,
	Stone,
	Grass,
	Wood,
	Sand,
};

struct TextureCoord
{
	TextureCoord(float xmin, float xmax, float ymin, float ymax);
	float Xmin, Xmax;
	float Ymin, Ymax;
};

class TextureAtlas
{

	int width, height; // width / height in TEXTURES, not pixels

	float Ystride;
	float Xstride;

	unsigned int ID;

public:
	TextureAtlas(const char* path, unsigned int stride);

	TextureCoord getTexture(unsigned int identifier);
	TextureCoord getTexture(int x, int y);

	void use();

};