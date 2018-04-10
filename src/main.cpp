#include "rom.hpp"
//#include "z80.hpp"
#include "gb.hpp"
#include <iostream>

int main() {
	rom r;
	gb gameboy;
	try {

		r.load_rom("/home/ayman/Desktop/arisaGB/rom/Legend of Zelda, The - Link's Awakening DX (U) (V1.2) [C][!].gbc");
		gameboy.load_rom(r);
		gameboy.start();

	} catch (const char *msg) {
		std::cout << msg << std::endl;
	}
	return 0;
}
