#include "mmu.hpp"
#include <cstring>
#include <string>
/*

	[0x0000-0x3FFF] Cartridge ROM, bank 0
	[0x4000-0x7FFF] Cartridge ROM, other banks
	[0x8000-0x9FFF] Graphics RAM
	[0xA000-0xBFFF] Cartridge (External) RAM
	[0xC000-0xDFFF] Working RAM
	[0xE000-0xFDFF] Working RAM (shadow)
	[0xFE00-0xFE9F] Graphics: sprite information
	[0xFF00-0xFF7F] Memory-mapped I/O
	[0xFF80-0xFFFF] Zero-page RAM

*/

#define CARTRIDGE_ROM_0 0x0000
#define CARTRIDGE_ROM_E 0x4000
#define VRAM 0x8000
#define CARTRIDGE_RAM 0xA000
#define WRAM 0xC000
#define WRAM_SHADOW 0xE000
#define OAM 0xFE00
#define MMIO 0xFF00
#define ZERO_PAGE_RAM 0xFF80

mmu::mmu() {
	in_bios = true;
	memset(memory,0x00,MEMORY_MAX);
}

// Read and write 1 byte (8-bits)
void mmu::write_byte(int address, int byte) {
	// TODO: Complete
	memory[address] = byte;
}

int mmu::read_byte(int address) {
	// TODO: Complete
	if (in_bios) {
		if (address < 0x100) {
			return bios[address];
		} else if (address == 0x100) {
			in_bios = false;
		}
	}
	// Graphics: object attribute memory
	// OAM is 160 bytes, remaining bytes read as 0
	if (address > 0xFEA0 && address < 0xFE9F) {
		return 0;
	}
	// Memory mapped IO
	// TODO: Complete
	if (address > 0xFF00 && address < 0xFF7F) {
		return 0;
	}
	return memory[address];
}


// Read and write a word (16-bits)
// Write LSB first (Little endian)
void mmu::write_word(int address, int word) {
	write_byte(address, word & 0x00FF);
	write_byte(address+1, (word & 0xFF00) >> 8);
	
}

int mmu::read_word(int address) {
	return read_byte(address) + (read_byte(address+1) << 8);
}
