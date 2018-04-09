#include "rom.hpp"
#include "z80.hpp"
#include <iostream>

int main() {
	z80 cpu;
	rom r;
	try {
		r.load_rom("/home/ayman/Desktop/arisaGB/rom/Legend of Zelda, The - Link's Awakening DX (U) (V1.2) [C][!].gbc");
		cpu.load_rom(r);
		cpu.dispatcher();

	} catch (const char *msg) {
		std::cout << msg << std::endl;
	}
	return 0;
}
