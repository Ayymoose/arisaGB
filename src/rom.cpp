#include "rom.hpp"

#include <string>
#include <fstream>
#include <iostream>

rom::rom() {
	rom_data = nullptr;	
	title.resize(TITLE_LENGTH);
	license_high = 0;
	license_low = 0;
	feature = 0;
	cartridge_type = 0;
	rom_size = 0;
	ram_size = 0;
	dest_code = 0;
	licensee_code = 0;
}

rom::~rom() {
	delete[] rom_data;
}

void rom::load_rom(std::string file_name) {

	// Open the rom
	std::ifstream rom_file;
	rom_file.open(file_name.c_str(), std::ifstream::ate | std::ifstream::binary);
	if (!rom_file.is_open()) {
		throw "File couldn't be opened!";
	}
	int rom_size_file = rom_file.tellg();
	if (rom_size_file == 0) {
		throw "File size is 0!";
	}
	rom_file.clear();
	rom_file.seekg(0, std::ios::beg);
	
	// Read entire rom to memory
	rom_data = new unsigned char[rom_size_file];
	// Read rom_size bytes!
	// Read MAX size file
	if (rom_size_file > ROM_MAX_SIZE) {
		std::cerr << rom_size_file << " > " << ROM_MAX_SIZE << std::endl;
		throw "File size too big!";
	}
	// TODO: Complete bytes read from here
	rom_file.read(reinterpret_cast<char*>(rom_data), rom_size_file);
	rom_file.close();

	// Check for Nintendo LOGO in ROM file
	const int nintendo_logo[NINTENDO_LOGO_LEN] =
	{
		0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,
		0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,
		0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E
	};

	int cartridge_ptr = NINTENDO_LOGO_ADDR;
	for (int i=cartridge_ptr,j=0; i<NINTENDO_LOGO_ADDR+NINTENDO_LOGO_LEN; i++,j++) {
		if (rom_data[i] != nintendo_logo[j]) {
			std::cerr << "Invalid ROM!" << std::endl;
			std::cerr << (int)rom_data[i] << " != " << (nintendo_logo[j]) << std::endl;
			throw "Invalid ROM!";
		}
	}



	//TODO: Compute checksum
	//x=0:FOR i=0134h TO 014Ch:x=x-MEM[i]-1:NEXT
	/*int x=0;
	for (int i=CARTRIDGE_HEADER; i<0x14C; i++) {
		x = x - rom_data[i] - 1;
		printf("0x%02X ",rom_data[i]);
	}
	printf("0x%04X\n",x);*/

	// Parse the ROM
	cartridge_ptr = CARTRIDGE_HEADER;

	// Copy the ROM title (Null is interpreted as the end of the title)
	for (int i=cartridge_ptr, j=0; i<cartridge_ptr+TITLE_LENGTH; i++,j++) {
		if (rom_data[i] == '\0') {
			break;
		} else {
			title[j] = rom_data[i];
		}
	}

	cartridge_ptr += TITLE_LENGTH;

    // Licensee code (high & low)
	license_high = rom_data[++cartridge_ptr];
	license_low = rom_data[++cartridge_ptr];
 
    // Feature
	feature = rom_data[++cartridge_ptr];

    // Cartridge type
	cartridge_type = rom_data[++cartridge_ptr];

    // ROM size
    rom_size = rom_data[++cartridge_ptr];

    // RAM size
    ram_size = rom_data[++cartridge_ptr];

    // Destination code
    dest_code = rom_data[++cartridge_ptr];
    
    // Licensee code (old)
	licensee_code = rom_data[++cartridge_ptr];	

}
