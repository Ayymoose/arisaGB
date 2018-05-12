#include <string>
#include <SDL2/SDL.h>
#include "sdl_timer.hpp"

#ifndef SDL_SCREEN_H
#define SDL_SCREEN_H

class sdl_screen {

	public:
		sdl_screen();
		sdl_screen(int width, int height, std::string window_title, int fps_limit);
		~sdl_screen();
		bool init();
		void render();
		void events() const;
		bool is_open() const;
	private:
		int width;
		int height;
		std::string window_title;
		int fps_limit;		

		int current_frame;
		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_Texture *texture;
		SDL_Event event;
		bool quit;
		sdl_timer fps;
};

#endif
