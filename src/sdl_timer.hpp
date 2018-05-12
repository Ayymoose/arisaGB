#ifndef SDL_TIMER_H
#define SDL_TIMER_H

class sdl_timer {
    public:

		sdl_timer();
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		int get_ticks() const;

		//Checks the status of the timer
		bool is_started() const;
		bool is_paused() const;

    private:
		//The clock time when the timer started
		int start_ticks;

		//The ticks stored when the timer was paused
		int paused_ticks;

		//The timer status
		bool paused;
		bool started;
};

#endif
