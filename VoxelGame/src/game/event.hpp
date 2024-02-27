#include <unordered_map>
#include "../render/render.h"

namespace game
{

	extern std::unordered_map<int, bool> keystates;
	extern cube::camTransform camera;

	void keypressCallback(GLFWwindow* window, int key, int scannode, int action, int mods);
	void mouseCallback(GLFWwindow* window, double xval, double yval);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

}