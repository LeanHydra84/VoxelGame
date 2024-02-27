#include "world.hpp"
#include "chunk.hpp"

#include "../render/render.h"
#include "generation/generation.hpp"

#include <algorithm>

namespace world
{

	// World methods
	World::World(unsigned int seed) : seed(seed), renderDistance(8), loadedChunks(0)
	{
		chunks = new Chunk*[renderDistance * renderDistance * 4 + 1];
	}

	World::~World()
	{
		for (size_t i = 0; i < loadedChunks; i++)
		{
			chunk_free(chunks[i]);
		}
		delete[] chunks;
	}

	void World::changeRenderDistance(unsigned int rd)
	{
		renderDistance = rd;
		
		loadedChunks = 0;
		delete[] chunks;

		chunks = new Chunk * [renderDistance * renderDistance * 4 + 1];
	}
	
	LocalChunkLocation World::get_chunk(int x, int z)
	{
		LocalChunkLocation loc = { x / 16, z / 16 };
		if (x < 0) loc.x--;
		if (z < 0) loc.z--;
		return loc;
	}

	void World::set_center_loadchunk(LocalChunkLocation cl)
	{
		
	}

	void World::load(LocalChunkLocation lc)
	{
		chunks[loadedChunks] = generate_chunk(seed, lc);
		loadedChunks++;
		loaded.push_back(lc);
	}

	bool World::is_chunk_loaded(LocalChunkLocation lc)
	{
		if (std::find(loaded.begin(), loaded.end(), lc) != loaded.end()) return true;
		return false;
	}

}