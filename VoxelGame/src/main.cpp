#include "game/game.hpp"
#include "render/render.h"

int main()
{
	game::init();
	cube::init();

	game::generate_world();
	game::game_loop();

	cube::cleanup();
	game::cleanup();
}