#ifndef _WORLD_H_
#define _WORLD_H_

#include "chunk.hpp"
#include <vector>

namespace world
{

	struct World
	{

		World(unsigned int seed);
		~World();

		signed long long seed;
		unsigned int renderDistance;

		std::vector<LocalChunkLocation> loaded;

		unsigned int loadedChunks;
		Chunk** chunks; // currently loaded chunks

		void changeRenderDistance(unsigned int rd);
		LocalChunkLocation get_chunk(int x, int z);
		
		void set_center_loadchunk(LocalChunkLocation chunk);
		void load(LocalChunkLocation lc);
		bool is_chunk_loaded(LocalChunkLocation lc);

	};

}

#endif