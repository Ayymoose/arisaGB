#include <iostream>
#include <SDL2/SDL.h>
#include "sdl_screen.hpp"

sdl_screen::sdl_screen() {
	width = 160;
	height = 144;
	window_title = "";
	fps_limit = 60;
	window = nullptr;
	renderer = nullptr;
	texture = nullptr;
	current_frame = 0;
	quit = false;
}

sdl_screen::~sdl_screen() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void sdl_screen::set_title(std::string title) {
	SDL_SetWindowTitle(window,title.c_str());
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

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	if (texture == nullptr) {
		std::cerr << SDL_GetError() << std::endl; 
		return false;
	}

    return true;
}

void sdl_screen::events() {
	//While there's events to handle
    while (SDL_PollEvent(&event)) {

		switch (event.type) {
			case SDL_KEYDOWN: std::cout << "A key is being pressed" << std::endl;	break;
			case SDL_QUIT:	quit = true; break;
			default:	break;
		}
    }
}

bool sdl_screen::is_open() const {
	return !quit;
}

void sdl_screen::render(unsigned char* screen_context) {

    //Start the frame timer
    fps.start();

	// Drawing code goes here
	SDL_UpdateTexture(texture, nullptr, screen_context,16); 

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
