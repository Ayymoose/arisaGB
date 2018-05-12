#include <cstdio>

#include "gb.hpp"
#include "debug.hpp"
#include "rom.hpp"

gb::gb() {
	gb_cpu.reset();
	gb_screen.init();
}

// TODO: Complete
// Loads two banks from the ROM into memory
void gb::load_rom(const rom& gb_rom) {
	gb_cpu.load_bank(gb_rom);
}
void gb::start() {
	LOG("GB start!");
	int clock_max = gb_cpu.clock_m + FRAME_CYCLES;

	while (gb_screen.is_open()) {
		gb_screen.events();
		if (gb_cpu.clock_m < clock_max) {			
			gb_cpu.execute();
			if (gb_cpu.frame_ready()) {
				gb_screen.render();
				gb_cpu.reset_frame();
			}
		}
	} 

	LOG("GB finished!");
}
