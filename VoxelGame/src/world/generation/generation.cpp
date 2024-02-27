#include "generation.hpp"
#include "generation_details.hpp"

Chunk* generate_chunk(unsigned int seed, LocalChunkLocation loc)
{
	
	Chunk* newChunk = new Chunk();
	newChunk->localPos = loc;

	generate_terrain(newChunk, seed);

	recalculate_chunk_mesh(newChunk);

	return newChunk;

}