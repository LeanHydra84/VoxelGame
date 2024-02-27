#include <gl/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>

#include "game.hpp"
#include "event.hpp"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "../render/render.h"

#include <iostream>

#define M_PI 3.1415926535897932384626433832795

namespace game
{

	bool wireframe = false;
	void toggle_wireframe()
	{
		if (!wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		wireframe = !wireframe;
	}

	void keypressCallback(GLFWwindow* window, int key, int scannode, int action, int mods)
	{
		if (key != GLFW_KEY_UNKNOWN)
		{
			if (action == GLFW_PRESS)
			{
				switch (key)
				{
				case GLFW_KEY_Q:
					toggle_wireframe();
					break;
				case GLFW_KEY_ESCAPE:
					set_window_active(false);
					break;
				case GLFW_KEY_R:
					std::cout << "(" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")" << std::endl;
					break;
				}
			}

			if (action == GLFW_PRESS)
				keystates[key] = true;
			else if (action == GLFW_RELEASE)
				keystates[key] = false;
		}
	}

	float sensititivy = 0.25f;

	float clampf(float n, float low, float high)
	{
		if (n < low) return low;
		if (n > high) return high;
		return n;
	}

	void mouseCallback(GLFWwindow* window, double xval, double yval)
	{
		if (isWindowActive)
		{
			camera.rotation.x -= static_cast<float>(((SCREEN_WIDTH >> 1) - xval) / SCREEN_WIDTH * (2 * M_PI) * sensititivy);
			camera.rotation.y -= static_cast<float>(((SCREEN_HEIGHT >> 1) - yval) / SCREEN_HEIGHT * (2 * M_PI) * sensititivy);

			camera.rotation.y = clampf(camera.rotation.y, -M_PI / 2, M_PI / 2);
		}
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		set_window_active(true);
	}

}