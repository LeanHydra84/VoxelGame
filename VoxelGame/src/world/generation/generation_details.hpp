#ifndef _GENERATION_DETAILS_HPP_
#define _GENERATION_DETAILS_HPP_

#include "../chunk.hpp"
#include "PerlinNoise.hpp"

using namespace world; // this feels extremely sinful

void generate_terrain(Chunk* chunk, unsigned int seed);

#endif