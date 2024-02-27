#ifndef _RENDER_H_
#define _RENDER_H_

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "texture_atlas.hpp"

namespace world { struct Chunk; struct World; }

namespace cube
{

	extern TextureAtlas* atlas;

	enum class CullFace
	{
		None = 0x0,
		Top = 0x1,   
		Bottom = 0x2,
		Front = 0x4,
		Back = 0x8,
		Left = 0x10,
		Right = 0x20,
		All = 63,
	};

	inline bool operator==(int x, CullFace y)
	{
		return x == static_cast<int>(y);
	}

	inline void operator|=(int& x, CullFace y)
	{
		x |= static_cast<int>(y);
	}

	struct camTransform
	{
		glm::vec3 position;
		glm::vec2 rotation;
	};

	void init();
	void cleanup();

	void render_scene(const camTransform& camera, const world::World* world);
	void draw_block(unsigned char blockType, const glm::vec3& pos, int faces);
	void draw_chunk(const world::Chunk *chunk);

}

#endif