#ifndef _GENERATION_HPP_
#define _GENERATION_HPP_

#include "../chunk.hpp"

world::Chunk *generate_chunk(unsigned int seed, world::LocalChunkLocation loc);

#endif