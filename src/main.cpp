#include "rom.hpp"
#include "gb.hpp"

#include <iostream>

int main() {
	rom r;
	gb gameboy;
	try {

		r.load_rom("/home/ayman/Desktop/arisaGB/rom/ttt.gb");
		gameboy.load_rom(r);
		gameboy.start();

	} catch (const char *msg) {
		std::cout << msg << std::endl;
	}
	return 0;
}
