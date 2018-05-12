#include "rom.hpp"
#include "gb.hpp"

int main()
{
	rom gb_rom;
	gb gameboy;
	try {
		gb_rom.load_rom("/home/ayman/Desktop/arisaGB/rom/opus5.gb");
		gameboy.load_rom(gb_rom);

	} catch (const char *msg) {
		std::cout << msg << std::endl;
	}

	gameboy.start();

    return 0;
}
