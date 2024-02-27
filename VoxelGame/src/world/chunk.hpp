#ifndef _CHUNK_H_
#define _CHUNK_H_

#define VERT_SIZE 5
#define Y_MAXHEIGHT 128
#define xyzToIndex(x, y, z) (((x) + ((z) * 16) ) + (y) * (16 * 16))

#include <vector>
#include <glm/vec3.hpp>


namespace world
{

	typedef unsigned int face_t;

	struct LocalChunkLocation
	{
		int x, z;

		inline bool operator==(const LocalChunkLocation& other)
		{
			return x == other.x && z == other.z;
		}

		inline bool operator!=(const LocalChunkLocation& other)
		{
			return x != other.x || z != other.z;
		}

	};

	struct Block
	{
		unsigned short type : 12;
		unsigned int  light : 4;
	};

	struct ChunkMesh
	{
		unsigned int vao;
		unsigned int vbo;
		float* mesh;
		unsigned int count;
	};

	struct Chunk
	{

		// X, Y index of the current chunk 
		// (localX, localY) * 16 = (0, 0) local chunk block in world space 
		LocalChunkLocation localPos;

		struct {
			bool empty;
		} flags;

		Block blocks[16 * 16 * Y_MAXHEIGHT];
		ChunkMesh mesh;

		inline glm::vec3 offset() const
		{
			return glm::vec3(localPos.x * 16, 0, localPos.z * 16);
		}

	};

	inline glm::vec3 get_chunk_corner_world_coordinates(const Chunk* ch)
	{
		return glm::vec3(ch->localPos.x * 16, 0, ch->localPos.z * 16);
	}

	void chunk_free(Chunk* ch);

	int chunk_get_face(Chunk* chunk, int x, int y, int z);

	bool is_block_air(const Block& block);

	Chunk* getTestChunk();

	void recalculate_chunk_mesh(Chunk* chunk);

}

#endif