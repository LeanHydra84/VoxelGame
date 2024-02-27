#ifndef _GAME_HPP_
#define _GAME_HPP_

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MOV_SPD 40.0f

namespace game
{

	extern bool isWindowActive;

	int init();
	void generate_world();
	void game_loop();
	void cleanup();

	void set_window_active(bool active);

}

#endif