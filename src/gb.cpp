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
	int clock_max = gb_cpu.clock_m + FRAME_CYCLES;
	do {
		gb_cpu.execute();
	} while (gb_cpu.clock_m < clock_max);

	/*printf("{\n");
	for (int i=0x9800; i<0x9BFF; i++) {
		printf("0x%02X, ",gb_cpu.memory[i]);
		if (i % 15 == 0) {
			printf("\n");
		}
	}
	printf("}\n");*/

	//gb_cpu.gb_gpu.dump_vram();
	//gb_cpu.gb_gpu.dump_tilemaps();
	//gb_cpu.gb_gpu.dump_screen();
}

void gb::set_context(unsigned char **screen) {
	*screen = gb_cpu.gb_gpu.screen;
}
