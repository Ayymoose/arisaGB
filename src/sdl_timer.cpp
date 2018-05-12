#include <SDL2/SDL.h>
#include "sdl_timer.hpp"

sdl_timer::sdl_timer() {
    start_ticks = 0;
    paused_ticks = 0;
    paused = false;
    started = false;
}

void sdl_timer::start() {
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    start_ticks = SDL_GetTicks();
}

void sdl_timer::stop() {
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void sdl_timer::pause() {
    //If the timer is running and isn't already paused
    if (started && paused) {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        paused_ticks = SDL_GetTicks() - start_ticks;
    }
}

void sdl_timer::unpause() {
    //If the timer is paused
    if (paused) {

        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        start_ticks = SDL_GetTicks() - paused_ticks;

        //Reset the paused ticks
        paused_ticks = 0;
    }
}

int sdl_timer::get_ticks() const {
    //If the timer is running
    if (started) {
        //If the timer is paused
        if (paused) {
            //Return the number of ticks when the timer was paused
            return paused_ticks;
        } else {
            //Return the current time minus the start time
            return SDL_GetTicks() - start_ticks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool sdl_timer::is_started() const {
    return started;
}

bool sdl_timer::is_paused() const {
    return paused;
}
