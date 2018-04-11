#include "gb.hpp"
#include "rom.hpp"

gb::gb() {
	gb_cpu.reset();
}

// TODO: Complete
// At present, loads 16kB into Bank 0
void gb::load_rom(const rom& r) {
	gb_cpu.load_bank(r);
}
void gb::start() {
	int clock_max = gb_cpu.clock_m + FRAME_CYCLES;
	do {
		gb_cpu.execute();
	} while (gb_cpu.clock_m < clock_max);
	//gb_cpu.gb_gpu.dump_screen();

}

void gb::stop() {

}
