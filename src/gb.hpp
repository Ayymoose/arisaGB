#ifndef GB_H
#define GB_H

#include "z80.hpp"
#include "rom.hpp"
#include "sdl_screen.hpp"

class gb {
	public:
		gb();
		// Load a ROM to be played 
		void load_rom(const rom& gb_rom);
		// Star the gameboy		
		void start();
		// Reset		
		void reset();
	private:
		z80 gb_cpu;
		sdl_screen gb_screen;
};

#endif
