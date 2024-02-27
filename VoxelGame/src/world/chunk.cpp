#include "chunk.hpp"
#include "world.hpp"

#include <GL/glew.h>

#include "../render/render.h"

#include <algorithm>
#include <iostream>

#define FACE_SIZE (VERT_SIZE * 6) // size in floats of a single vertice
#define RF_INDEX (subface * FACE_SIZE + j * VERT_SIZE)

namespace world
{

	static const float RIGHT[] = 
	{
		0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
						   
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f, 1.0f,
	};

	static const float TOP[] =
	{
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
							
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
	};

	static const float FRONT[] =
	{
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
							
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
	};

	static const float LEFT[] =
	{
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
							
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
	};

	static const float BOTTOM[] =
	{
		-0.5f,  -0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f,  -0.5f,  0.5f,	1.0f, 1.0f,
		 0.5f,  -0.5f, -0.5f,	1.0f, 0.0f,
							 
		-0.5f,  -0.5f, -0.5f,	0.0f, 0.0f,
		-0.5f,  -0.5f,  0.5f,	0.0f, 1.0f,
		 0.5f,  -0.5f,  0.5f,	1.0f, 1.0f,
	};

	static const float BACK[] =
	{
		-0.5f, -0.5f, 0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,	1.0f, 0.0f,
						   
		-0.5f, -0.5f, 0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f, 0.5f,	0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f,	1.0f, 1.0f,
	};

	static const int block_get_facecount(int face)
	{
		int ncount = 0;
		for (int i = 0; i < 6; i++)
		{
			if (((face << i) & 0x1) == 0) ncount++;
		}
		return ncount;
	}

	static int block_get_facecount(cube::CullFace face)
	{
		return block_get_facecount(static_cast<int>(face));
	}

	bool is_block_air(const Block& block)
	{
		return block.type == 0;
	}

	void chunk_free(Chunk* ch)
	{
		glDeleteVertexArrays(1, &ch->mesh.vao);
		glDeleteBuffers(1, &ch->mesh.vbo);
		delete[] ch->mesh.mesh;
	}

	int chunk_get_face(Chunk* chunk, int x, int y, int z)
	{
		using f = cube::CullFace;
		int cullface = static_cast<int>(f::None);

		if (x > 0)
		{
			if (!is_block_air(chunk->blocks[xyzToIndex(x - 1, y, z)]))
				cullface |= f::Left;
		}

		if (x < 15)
		{
			if (!is_block_air(chunk->blocks[xyzToIndex(x + 1, y, z)]))
				cullface |= f::Right;
		}

		if (y > 0)
		{
			if (!is_block_air(chunk->blocks[xyzToIndex(x, y - 1, z)]))
				cullface |= f::Bottom;
		}

		if (y < Y_MAXHEIGHT)
		{
			if (!is_block_air(chunk->blocks[xyzToIndex(x, y + 1, z)]))
				cullface |= f::Top;
		}

		if (z > 0)
		{
			if (!is_block_air(chunk->blocks[xyzToIndex(x, y, z - 1)]))
				cullface |= f::Front;
		}

		if (z < 15)
		{
			if (!is_block_air(chunk->blocks[xyzToIndex(x, y, z + 1)]))
				cullface |= f::Back;
		}

		return cullface;

		//blocks[xyzToIndex(x, y, z)]
	}

	Chunk* getTestChunk()
	{
		Chunk* tc = new Chunk();
		tc->localPos = { 0, 0 };

		for (size_t x = 0; x < 16; x++)
		{
			for (size_t z = 0; z < 16; z++)
			{
				tc->blocks[xyzToIndex(x, 0, z)].type = 1;
				tc->blocks[xyzToIndex(x, 1, z)].type = 1;
				tc->blocks[xyzToIndex(x, 3, z)].type = 1;
			}
		}

		recalculate_chunk_mesh(tc);

		return tc;
	}

	//_declspec(deprecated) void recalculate_chunk_mesh(Chunk* chunk)
	//{
	//	chunk->mesh.clear();
	//	for (size_t x = 0; x < 16; x++)
	//	{
	//		for (size_t z = 0; z < 16; z++)
	//		{
	//			for (size_t y = 0; y < Y_MAXHEIGHT; y++)
	//			{
	//				if (!is_block_air(chunk->blocks[xyzToIndex(x, y, z)]))
	//				{
	//					int faces = chunk_get_face(chunk, x, y, z);
	//					if (faces == cube::CullFace::All) continue;
	//
	//					ChunkMeshPosition cm;
	//					cm.position = glm::vec3(x, y, z);
	//					cm.type = chunk->blocks[xyzToIndex(x, y, z)].type;
	//					cm.faceMesh = faces;
	//
	//					chunk->mesh.push_back(cm);
	//
	//				}
	//			}
	//		}
	//	}
	//
	//}

	static const float* get_next_face_array(int face, int& offset)
	{
		while (((face >> (offset)) & 0x1) != 0) { if (offset >= 6) return nullptr; offset++; }
		offset++;
		switch (offset - 1)
		{
		case 0:
			return TOP;
		case 1:
			return BOTTOM;
		case 2:
			return FRONT;
		case 3:
			return BACK;
		case 4:
			return LEFT;
		case 5:
			return RIGHT;
		default:
			return nullptr;
		}
	}

	static struct cm_buffer
	{
		glm::ivec3 position;
		char face;
		unsigned short type;
	};

	void recalculate_chunk_mesh(Chunk* chunk)
	{
		std::vector<cm_buffer> blockVec;
		int facecount = 0;
		
		// Getting correct blocks
		for (size_t x = 0; x < 16; x++)
		{
			for (size_t z = 0; z < 16; z++)
			{
				for (size_t y = 0; y < Y_MAXHEIGHT; y++)
				{
					if (!is_block_air(chunk->blocks[xyzToIndex(x, y, z)]))
					{
						char faces = chunk_get_face(chunk, x, y, z);
						if (faces == cube::CullFace::All) continue;

						blockVec.push_back(cm_buffer { glm::ivec3(x, y, z), faces, chunk->blocks[xyzToIndex(x, y, z)].type });
						facecount += block_get_facecount(faces);
					}
				}
			}
		}


		float *renderFaces = new float[facecount * FACE_SIZE];

		unsigned int subface = 0;
		for (size_t i = 0; i < blockVec.size(); i++)
		{
			auto& block = blockVec[i];
			const float* workingMem = nullptr;
			int offset = 0;

			while ((workingMem = get_next_face_array(block.face, offset)) != nullptr)
			{
				TextureCoord crd = cube::atlas->getTexture(block.type);
				for (size_t j = 0; j < 6; j++)
				{
					renderFaces[RF_INDEX + 0] = workingMem[VERT_SIZE * j + 0] + block.position.x;
					renderFaces[RF_INDEX + 1] = workingMem[VERT_SIZE * j + 1] + block.position.y;
					renderFaces[RF_INDEX + 2] = workingMem[VERT_SIZE * j + 2] + block.position.z;
#if VERT_SIZE > 3
					renderFaces[RF_INDEX + 3] = workingMem[VERT_SIZE * j + 3] == 0.0f ? crd.Xmin : crd.Xmax;
					renderFaces[RF_INDEX + 4] = workingMem[VERT_SIZE * j + 4] == 0.0f ? crd.Ymin : crd.Ymax;
#endif
				}
				subface++;
			}
		}

		assert(subface <= facecount);

		chunk->mesh.count = facecount * FACE_SIZE;
		chunk->mesh.mesh = renderFaces;

		glGenVertexArrays(1, &chunk->mesh.vao);
		glBindVertexArray(chunk->mesh.vao);

		glGenBuffers(1, &chunk->mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.vbo);

		glBufferData(GL_ARRAY_BUFFER, chunk->mesh.count * sizeof(float), chunk->mesh.mesh, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERT_SIZE * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERT_SIZE * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}

}