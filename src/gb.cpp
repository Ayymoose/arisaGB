#include <cstdio>

#include "gb.hpp"
#include "debug.hpp"
#include "rom.hpp"

gb::gb() {
	gb_cpu.reset();
	gb_screen.init();
	gb_screen.set_title("ArisaGB");
}

// TODO: Complete
// Loads two banks from the ROM into memory
void gb::load_rom(const rom& gb_rom) {
	gb_cpu.load_bank(gb_rom);
}
void gb::start() {
	LOG("GB start!");
	int clock_max = gb_cpu.clock_m + FRAME_CYCLES;
	unsigned char *screen_context = gb_cpu.get_context();

	while (gb_screen.is_running()) {
		gb_screen.event_handler();
		if (gb_cpu.clock_m < clock_max) {			
			gb_cpu.execute();
			if (gb_cpu.frame_ready()) {
				gb_screen.render(screen_context);
				gb_cpu.reset_frame();
			}
		}
	} 

	LOG("GB finished!");
}
