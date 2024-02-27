#define GLEW_STATIC

#include <GL/glew.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "image_handling.hpp"
#include "shader_handler.hpp"

#include "render.h"

#include "../world/chunk.hpp"
#include "../world/world.hpp"

#include <iostream>


namespace cube
{

	static Shader blockshader;
	TextureAtlas* atlas;

	//static GLuint VAO;
	static GLuint texture;

	void init()
	{

		glewInit();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glEnable(GL_DEPTH_TEST);

		blockshader = Shader(
			"C:\\Users\\Augustus\\Desktop\\Projects\\Programming\\VoxelGame\\shaders\\standard.vert",
			"C:\\Users\\Augustus\\Desktop\\Projects\\Programming\\VoxelGame\\shaders\\standard.frag"
		);

		blockshader.use();

		atlas = new TextureAtlas("C:\\Users\\Augustus\\Desktop\\Projects\\Programming\\VoxelGame\\atlas.png", 32);
		atlas->use();

		//glGenTextures(1, &texture);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//
		//Image image("C:\\Users\\Augustus\\Desktop\\Projects\\Programming\\VoxelGame\\stolen resources\\assets\\minecraft\\textures\\block\\stone.png");
		//
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glClearColor(0.427f, 0.772f, 0.996f, 1.0f);

	}

	void cleanup()
	{
		//glDeleteVertexArrays(1, &VAO);
		glDeleteTextures(1, &texture);
	}

	void render_scene(const camTransform& camera, const world::World* world)
	{
		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		blockshader.use();

		// Set Transforms 

		// ORDER MATTERS: 
		//		translate then rotate = rotate in place
		//		rotate then translate = rotate around origin

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 16.f / 9.f, 0.1f, 100.0f);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, camera.rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, camera.rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, -camera.position);

		glUniformMatrix4fv(glGetUniformLocation(blockshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));		// should be updated every frame
		glUniformMatrix4fv(glGetUniformLocation(blockshader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));			// should be updated every frame

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);

		for (size_t i = 0; i < world->loadedChunks; i++)
		{
			draw_chunk(world->chunks[i]);
		}
	}

	void draw_chunk(const world::Chunk* chunk)
	{	
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, world::get_chunk_corner_world_coordinates(chunk));

		glUniformMatrix4fv(glGetUniformLocation(blockshader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(chunk->mesh.vao);
		glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.vbo);

		glDrawArrays(GL_TRIANGLES, 0, chunk->mesh.count);
	}

}