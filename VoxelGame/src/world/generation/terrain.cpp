#include "generation_details.hpp"

void fill_to_bedrock(Chunk* chunk, int x, int z, int height) // very wacky parameter order, but whatever
{
	for (size_t i = 0; i < height; i++)
	{
		chunk->blocks[xyzToIndex(x, i, z)].type = 1;
	}
}

void generate_terrain(Chunk* chunk, unsigned int seed)
{
	const siv::PerlinNoise perlin(seed);

	for (int x = 0; x < 16; x++)
	{
		for (size_t z = 0; z < 16; z++)
		{
			int worldX = chunk->localPos.x * 16 + x;
			int worldZ = chunk->localPos.z * 16 + z;

			const double noise = perlin.octave2D_01(worldX * 0.01, worldZ * 0.01, 4);

			unsigned int height = static_cast<unsigned int>(noise * Y_MAXHEIGHT * 0.5f);
			
			chunk->blocks[ xyzToIndex(x, height, z) ].type = 2;
			fill_to_bedrock(chunk, x, z, height);

		}
	}

}