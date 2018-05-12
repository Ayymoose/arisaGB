#include <iostream>
#include <SDL2/SDL.h>
#include "sdl_screen.hpp"

sdl_screen::sdl_screen(int width, int height, std::string window_title, int bpp, int fps_limit) :
	width(width),
	height(height),
	window_title(window_title),
	bpp(bpp),
	fps_limit(fps_limit) 
{
		window = nullptr;
		renderer = nullptr;
		current_frame = 0;
		quit = false;
}

sdl_screen::~sdl_screen() {
	SDL_Quit();
}

bool sdl_screen::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow(window_title.c_str(),
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          width, height,
                          SDL_WINDOW_OPENGL);
    if (window == nullptr) {
		std::cerr << SDL_GetError() << std::endl;
        return false;
    }
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cerr << SDL_GetError() << std::endl;
		return false;
	}
    return true;
}

void sdl_screen::render() {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//While the user hasn't quit
    while (!quit) {

        //Start the frame timer
        fps.start();

        //While there's events to handle
        while (SDL_PollEvent(&event)) {

			switch (event.type) {
				case SDL_KEYDOWN: std::cout << "A key is being pressed" << std::endl;	break;
				case SDL_QUIT:	quit = true; break;
				default:	break;
			}
        }

		// Drawing code goes here


		// Flip buffers
   		SDL_RenderPresent(renderer);

        //Increment the frame counter
        current_frame++;

        //If we want to cap the frame rate
        if (fps.get_ticks() < 1000 / fps_limit) {
            //Sleep the remaining frame time
            SDL_Delay((1000 / fps_limit) - fps.get_ticks());
        }
    }
}
