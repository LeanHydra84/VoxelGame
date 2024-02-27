#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <chrono>
#include <unordered_map>

#include "game.hpp"
#include "event.hpp"

#include "../render/render.h"
#include "../render/text_render.hpp"

#include "../world/world.hpp"

namespace game
{
	
	static GLFWwindow* window;
	static glm::vec3 worldUp;
	
	bool isWindowActive;

	std::unordered_map<int, bool> keystates;
	cube::camTransform camera;

	world::World* terrain;

	int init()
	{

		srand(time(NULL));

		if (!glfwInit())
		{
			return -1;
		}

		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "VoxelGame", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		set_window_active(true);

		glfwSetKeyCallback(window, keypressCallback);
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);

		return 0;
	}

	void generate_world()
	{
		camera.position = glm::vec3(0.0f, 50.0f, 0.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		terrain = new world::World(abs(rand()));
		terrain->load({ 0, 0 });
		terrain->load({ -1, 0 });
		terrain->load({ 1, 0 });

		terrain->load({ 1,  -1 });
		terrain->load({ 0, -1 });
		terrain->load({ 0,  1 });

		terrain->load({ -1, 1 });
		terrain->load({ 1, 1 });
		terrain->load({ -1, -1 });
	}

	void cleanup()
	{
		glfwTerminate();
		delete terrain;
	}

	void set_window_active(bool active)
	{
		glfwSetInputMode(window, GLFW_CURSOR, active ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
		isWindowActive = active;
	}

	void processInput(float deltaTime)
	{
		
		glm::vec3 cameraForward = glm::vec3(sin(camera.rotation.x), -sin(camera.rotation.y), -cos(camera.rotation.x));
		glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraForward));

		if (keystates[GLFW_KEY_A])
			camera.position += MOV_SPD * deltaTime * cameraRight;
		else if (keystates[GLFW_KEY_D])
			camera.position -= MOV_SPD * deltaTime * cameraRight;
		if (keystates[GLFW_KEY_W])
			camera.position += MOV_SPD * deltaTime * cameraForward;
		else if (keystates[GLFW_KEY_S])
			camera.position -= MOV_SPD * deltaTime * cameraForward;

		if (keystates[GLFW_KEY_SPACE])
			camera.position.y += MOV_SPD * deltaTime;
		else if (keystates[GLFW_KEY_LEFT_SHIFT])
			camera.position.y -= MOV_SPD * deltaTime;
	}

	void check_current_chunk(world::LocalChunkLocation& oldChunk)
	{
		world::LocalChunkLocation currentChunk = terrain->get_chunk((int)camera.position.x, (int)camera.position.z);

		if (currentChunk != oldChunk)
		{
			if(!terrain->is_chunk_loaded(currentChunk))
				terrain->load(currentChunk);
		}
		oldChunk = currentChunk;
	}

	void game_loop()
	{

		auto alphaTime = std::chrono::high_resolution_clock::now();
		
		world::LocalChunkLocation oldChunkLoc;

		while (!glfwWindowShouldClose(window))
		{

			auto betaTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(betaTime - alphaTime).count() / 1000000000.0f;
			alphaTime = betaTime;

			//std::cout << (1.0f / deltaTime) << std::endl;

			// Render
			cube::render_scene(camera, terrain);

			// Flip Screen
			glfwSwapBuffers(window);

			// Poll Events
			glfwPollEvents();

			// Check world position
			check_current_chunk(oldChunkLoc);

			// Input
			if (isWindowActive)
			{
				processInput(deltaTime);
				glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			}

		}

	}

}