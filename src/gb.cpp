#include <cstdio>

#include "gb.hpp"
#include "rom.hpp"

gb::gb() {
	gb_cpu.reset();
	context = nullptr;
}

// TODO: Complete
// Loads two banks from the ROM into memory
void gb::load_rom(const rom& r) {
	gb_cpu.load_bank(r);
}
void gb::start() {
	int clock_max = gb_cpu.clock_m + (FRAME_CYCLES);
	do {
		gb_cpu.execute();
	} while (gb_cpu.clock_m < clock_max);
}

void gb::set_context(unsigned char **screen) {
	*screen = gb_cpu.gb_gpu.screen;
}
