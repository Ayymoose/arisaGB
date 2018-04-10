#include <cstring>

#include "z80.hpp"
#include "gpu.hpp"

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



// Read and write 1 byte (8-bits)
void z80::write_byte(int address, int byte) {
	// TODO: Complete
	switch (address & 0xF000) {
	case VRAM:
	case 9000:
		gb_gpu.vram[address & 0x1FFF] = byte;
		gb_gpu.update_tile(address, byte);
		break;
	default: memory[address] = byte;
	}

}

int z80::read_byte(int address) {
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
void z80::write_word(int address, int word) {
	write_byte(address, word & 0x00FF);
	write_byte(address+1, (word & 0xFF00) >> 8);

}

int z80::read_word(int address) {
	return read_byte(address) + (read_byte(address+1) << 8);
}

#define ZERO_FLAG 0x80			// Set if the last operation produced a result of 0;
#define SUBTRACT_FLAG 0x40		// Set if the last operation was a subtraction;
#define HALF_CARRY_FLAG 0x20	// Set if, in the result of the last operation, the lower half of the byte overflowed past 15;
#define CARRY_FLAG 0x10			// Set if the last operation produced a result over 0xFF (for additions) or under 0 (for subtractions).

// NOTE: This loads 64k of the rom into the entire MMU
// FIXME: This should only load 1 bank (16k) in the mmu from 0x0000
// Load the rom into the mmu
void z80::load_rom(const rom &r) {
	memcpy(memory,r.rom_data,MEMORY_MAX);
}

z80::~z80() {

}

z80::z80() {

	in_bios = true;
	memset(memory,0x00,MEMORY_MAX);
	gb_gpu.reset();

	clock_m = 0;
	m_cycle = 0;
	t_cycle = 0;
	ime = true;
	halt = false;
	pc = 0x0000;
	sp = 0x0000;
	for (int i=0; i<REGISTERS; i++) {
		reg8[i] = 0;
	}
				
	opcodes[0x00] = &z80::f_0x00;
	opcodes[0x01] = &z80::f_0x01;
	opcodes[0x02] = &z80::f_0x02;
	opcodes[0x03] = &z80::f_0x03;
	opcodes[0x04] = &z80::f_0x04;
	opcodes[0x05] = &z80::f_0x05;
	opcodes[0x06] = &z80::f_0x06;
	opcodes[0x07] = &z80::f_0x07;
	opcodes[0x08] = &z80::f_0x08;
	opcodes[0x09] = &z80::f_0x09;
	opcodes[0x0A] = &z80::f_0x0A;
	opcodes[0x0B] = &z80::f_0x0B;
	opcodes[0x0C] = &z80::f_0x0C;
	opcodes[0x0D] = &z80::f_0x0D;
	opcodes[0x0E] = &z80::f_0x0E;
	opcodes[0x0F] = &z80::f_0x0F;
	opcodes[0x10] = &z80::f_0x10;
	opcodes[0x11] = &z80::f_0x11;
	opcodes[0x12] = &z80::f_0x12;
	opcodes[0x13] = &z80::f_0x13;
	opcodes[0x14] = &z80::f_0x14;
	opcodes[0x15] = &z80::f_0x15;
	opcodes[0x16] = &z80::f_0x16;
	opcodes[0x17] = &z80::f_0x17;
	opcodes[0x18] = &z80::f_0x18;
	opcodes[0x19] = &z80::f_0x19;
	opcodes[0x1A] = &z80::f_0x1A;
	opcodes[0x1B] = &z80::f_0x1B;
	opcodes[0x1C] = &z80::f_0x1C;
	opcodes[0x1D] = &z80::f_0x1D;
	opcodes[0x1E] = &z80::f_0x1E;
	opcodes[0x1F] = &z80::f_0x1F;
	opcodes[0x20] = &z80::f_0x20;
	opcodes[0x21] = &z80::f_0x21;
	opcodes[0x22] = &z80::f_0x22;
	opcodes[0x23] = &z80::f_0x23;
	opcodes[0x24] = &z80::f_0x24;
	opcodes[0x25] = &z80::f_0x25;
	opcodes[0x26] = &z80::f_0x26;
	opcodes[0x27] = &z80::f_0x27;
	opcodes[0x28] = &z80::f_0x28;
	opcodes[0x29] = &z80::f_0x29;
	opcodes[0x2A] = &z80::f_0x2A;
	opcodes[0x2B] = &z80::f_0x2B;
	opcodes[0x2C] = &z80::f_0x2C;
	opcodes[0x2D] = &z80::f_0x2D;
	opcodes[0x2E] = &z80::f_0x2E;
	opcodes[0x2F] = &z80::f_0x2F;
	opcodes[0x30] = &z80::f_0x30;
	opcodes[0x31] = &z80::f_0x31;
	opcodes[0x32] = &z80::f_0x32;
	opcodes[0x33] = &z80::f_0x33;
	opcodes[0x34] = &z80::f_0x34;
	opcodes[0x35] = &z80::f_0x35;
	opcodes[0x36] = &z80::f_0x36;
	opcodes[0x37] = &z80::f_0x37;
	opcodes[0x38] = &z80::f_0x38;
	opcodes[0x39] = &z80::f_0x39;
	opcodes[0x3A] = &z80::f_0x3A;
	opcodes[0x3B] = &z80::f_0x3B;
	opcodes[0x3C] = &z80::f_0x3C;
	opcodes[0x3D] = &z80::f_0x3D;
	opcodes[0x3E] = &z80::f_0x3E;
	opcodes[0x3F] = &z80::f_0x3F;
	opcodes[0x40] = &z80::f_0x40;
	opcodes[0x41] = &z80::f_0x41;
	opcodes[0x42] = &z80::f_0x42;
	opcodes[0x43] = &z80::f_0x43;
	opcodes[0x44] = &z80::f_0x44;
	opcodes[0x45] = &z80::f_0x45;
	opcodes[0x46] = &z80::f_0x46;
	opcodes[0x47] = &z80::f_0x47;
	opcodes[0x48] = &z80::f_0x48;
	opcodes[0x49] = &z80::f_0x49;
	opcodes[0x4A] = &z80::f_0x4A;
	opcodes[0x4B] = &z80::f_0x4B;
	opcodes[0x4C] = &z80::f_0x4C;
	opcodes[0x4D] = &z80::f_0x4D;
	opcodes[0x4E] = &z80::f_0x4E;
	opcodes[0x4F] = &z80::f_0x4F;
	opcodes[0x50] = &z80::f_0x50;
	opcodes[0x51] = &z80::f_0x51;
	opcodes[0x52] = &z80::f_0x52;
	opcodes[0x53] = &z80::f_0x53;
	opcodes[0x54] = &z80::f_0x54;
	opcodes[0x55] = &z80::f_0x55;
	opcodes[0x56] = &z80::f_0x56;
	opcodes[0x57] = &z80::f_0x57;
	opcodes[0x58] = &z80::f_0x58;
	opcodes[0x59] = &z80::f_0x59;
	opcodes[0x5A] = &z80::f_0x5A;
	opcodes[0x5B] = &z80::f_0x5B;
	opcodes[0x5C] = &z80::f_0x5C;
	opcodes[0x5D] = &z80::f_0x5D;
	opcodes[0x5E] = &z80::f_0x5E;
	opcodes[0x5F] = &z80::f_0x5F;
	opcodes[0x60] = &z80::f_0x60;
	opcodes[0x61] = &z80::f_0x61;
	opcodes[0x62] = &z80::f_0x62;
	opcodes[0x63] = &z80::f_0x63;
	opcodes[0x64] = &z80::f_0x64;
	opcodes[0x65] = &z80::f_0x65;
	opcodes[0x66] = &z80::f_0x66;
	opcodes[0x67] = &z80::f_0x67;
	opcodes[0x68] = &z80::f_0x68;
	opcodes[0x69] = &z80::f_0x69;
	opcodes[0x6A] = &z80::f_0x6A;
	opcodes[0x6B] = &z80::f_0x6B;
	opcodes[0x6C] = &z80::f_0x6C;
	opcodes[0x6D] = &z80::f_0x6D;
	opcodes[0x6E] = &z80::f_0x6E;
	opcodes[0x6F] = &z80::f_0x6F;
	opcodes[0x70] = &z80::f_0x70;
	opcodes[0x71] = &z80::f_0x71;
	opcodes[0x72] = &z80::f_0x72;
	opcodes[0x73] = &z80::f_0x73;
	opcodes[0x74] = &z80::f_0x74;
	opcodes[0x75] = &z80::f_0x75;
	opcodes[0x76] = &z80::f_0x76;
	opcodes[0x77] = &z80::f_0x77;
	opcodes[0x78] = &z80::f_0x78;
	opcodes[0x79] = &z80::f_0x79;
	opcodes[0x7A] = &z80::f_0x7A;
	opcodes[0x7B] = &z80::f_0x7B;
	opcodes[0x7C] = &z80::f_0x7C;
	opcodes[0x7D] = &z80::f_0x7D;
	opcodes[0x7E] = &z80::f_0x7E;
	opcodes[0x7F] = &z80::f_0x7F;
	opcodes[0x80] = &z80::f_0x80;
	opcodes[0x81] = &z80::f_0x81;
	opcodes[0x82] = &z80::f_0x82;
	opcodes[0x83] = &z80::f_0x83;
	opcodes[0x84] = &z80::f_0x84;
	opcodes[0x85] = &z80::f_0x85;
	opcodes[0x86] = &z80::f_0x86;
	opcodes[0x87] = &z80::f_0x87;
	opcodes[0x88] = &z80::f_0x88;
	opcodes[0x89] = &z80::f_0x89;
	opcodes[0x8A] = &z80::f_0x8A;
	opcodes[0x8B] = &z80::f_0x8B;
	opcodes[0x8C] = &z80::f_0x8C;
	opcodes[0x8D] = &z80::f_0x8D;
	opcodes[0x8E] = &z80::f_0x8E;
	opcodes[0x8F] = &z80::f_0x8F;
	opcodes[0x90] = &z80::f_0x90;
	opcodes[0x91] = &z80::f_0x91;
	opcodes[0x92] = &z80::f_0x92;
	opcodes[0x93] = &z80::f_0x93;
	opcodes[0x94] = &z80::f_0x94;
	opcodes[0x95] = &z80::f_0x95;
	opcodes[0x96] = &z80::f_0x96;
	opcodes[0x97] = &z80::f_0x97;
	opcodes[0x98] = &z80::f_0x98;
	opcodes[0x99] = &z80::f_0x99;
	opcodes[0x9A] = &z80::f_0x9A;
	opcodes[0x9B] = &z80::f_0x9B;
	opcodes[0x9C] = &z80::f_0x9C;
	opcodes[0x9D] = &z80::f_0x9D;
	opcodes[0x9E] = &z80::f_0x9E;
	opcodes[0x9F] = &z80::f_0x9F;
	opcodes[0xA0] = &z80::f_0xA0;
	opcodes[0xA1] = &z80::f_0xA1;
	opcodes[0xA2] = &z80::f_0xA2;
	opcodes[0xA3] = &z80::f_0xA3;
	opcodes[0xA4] = &z80::f_0xA4;
	opcodes[0xA5] = &z80::f_0xA5;
	opcodes[0xA6] = &z80::f_0xA6;
	opcodes[0xA7] = &z80::f_0xA7;
	opcodes[0xA8] = &z80::f_0xA8;
	opcodes[0xA9] = &z80::f_0xA9;
	opcodes[0xAA] = &z80::f_0xAA;
	opcodes[0xAB] = &z80::f_0xAB;
	opcodes[0xAC] = &z80::f_0xAC;
	opcodes[0xAD] = &z80::f_0xAD;
	opcodes[0xAE] = &z80::f_0xAE;
	opcodes[0xAF] = &z80::f_0xAF;
	opcodes[0xB0] = &z80::f_0xB0;
	opcodes[0xB1] = &z80::f_0xB1;
	opcodes[0xB2] = &z80::f_0xB2;
	opcodes[0xB3] = &z80::f_0xB3;
	opcodes[0xB4] = &z80::f_0xB4;
	opcodes[0xB5] = &z80::f_0xB5;
	opcodes[0xB6] = &z80::f_0xB6;
	opcodes[0xB7] = &z80::f_0xB7;
	opcodes[0xB8] = &z80::f_0xB8;
	opcodes[0xB9] = &z80::f_0xB9;
	opcodes[0xBA] = &z80::f_0xBA;
	opcodes[0xBB] = &z80::f_0xBB;
	opcodes[0xBC] = &z80::f_0xBC;
	opcodes[0xBD] = &z80::f_0xBD;
	opcodes[0xBE] = &z80::f_0xBE;
	opcodes[0xBF] = &z80::f_0xBF;
	opcodes[0xC0] = &z80::f_0xC0;
	opcodes[0xC1] = &z80::f_0xC1;
	opcodes[0xC2] = &z80::f_0xC2;
	opcodes[0xC3] = &z80::f_0xC3;
	opcodes[0xC4] = &z80::f_0xC4;
	opcodes[0xC5] = &z80::f_0xC5;
	opcodes[0xC6] = &z80::f_0xC6;
	opcodes[0xC7] = &z80::f_0xC7;
	opcodes[0xC8] = &z80::f_0xC8;
	opcodes[0xC9] = &z80::f_0xC9;
	opcodes[0xCA] = &z80::f_0xCA;
	opcodes[0xCB] = &z80::f_0xCB;
	opcodes[0xCC] = &z80::f_0xCC;
	opcodes[0xCD] = &z80::f_0xCD;
	opcodes[0xCE] = &z80::f_0xCE;
	opcodes[0xCF] = &z80::f_0xCF;
	opcodes[0xD0] = &z80::f_0xD0;
	opcodes[0xD1] = &z80::f_0xD1;
	opcodes[0xD2] = &z80::f_0xD2;
	opcodes[0xD3] = &z80::f_0xD3;
	opcodes[0xD4] = &z80::f_0xD4;
	opcodes[0xD5] = &z80::f_0xD5;
	opcodes[0xD6] = &z80::f_0xD6;
	opcodes[0xD7] = &z80::f_0xD7;
	opcodes[0xD8] = &z80::f_0xD8;
	opcodes[0xD9] = &z80::f_0xD9;
	opcodes[0xDA] = &z80::f_0xDA;
	opcodes[0xDB] = &z80::f_0xDB;
	opcodes[0xDC] = &z80::f_0xDC;
	opcodes[0xDD] = &z80::f_0xDD;
	opcodes[0xDE] = &z80::f_0xDE;
	opcodes[0xDF] = &z80::f_0xDF;
	opcodes[0xE0] = &z80::f_0xE0;
	opcodes[0xE1] = &z80::f_0xE1;
	opcodes[0xE2] = &z80::f_0xE2;
	opcodes[0xE3] = &z80::f_0xE3;
	opcodes[0xE4] = &z80::f_0xE4;
	opcodes[0xE5] = &z80::f_0xE5;
	opcodes[0xE6] = &z80::f_0xE6;
	opcodes[0xE7] = &z80::f_0xE7;
	opcodes[0xE8] = &z80::f_0xE8;
	opcodes[0xE9] = &z80::f_0xE9;
	opcodes[0xEA] = &z80::f_0xEA;
	opcodes[0xEB] = &z80::f_0xEB;
	opcodes[0xEC] = &z80::f_0xEC;
	opcodes[0xED] = &z80::f_0xED;
	opcodes[0xEE] = &z80::f_0xEE;
	opcodes[0xEF] = &z80::f_0xEF;
	opcodes[0xF0] = &z80::f_0xF0;
	opcodes[0xF1] = &z80::f_0xF1;
	opcodes[0xF2] = &z80::f_0xF2;
	opcodes[0xF3] = &z80::f_0xF3;
	opcodes[0xF4] = &z80::f_0xF4;
	opcodes[0xF5] = &z80::f_0xF5;
	opcodes[0xF6] = &z80::f_0xF6;
	opcodes[0xF7] = &z80::f_0xF7;
	opcodes[0xF8] = &z80::f_0xF8;
	opcodes[0xF9] = &z80::f_0xF9;
	opcodes[0xFA] = &z80::f_0xFA;
	opcodes[0xFB] = &z80::f_0xFB;
	opcodes[0xFC] = &z80::f_0xFC;
	opcodes[0xFD] = &z80::f_0xFD;
	opcodes[0xFE] = &z80::f_0xFE;
	opcodes[0xFF] = &z80::f_0xFF;

	cb_opcodes[0x00] = &z80::cb_0x00;
	cb_opcodes[0x01] = &z80::cb_0x01;
	cb_opcodes[0x02] = &z80::cb_0x02;
	cb_opcodes[0x03] = &z80::cb_0x03;
	cb_opcodes[0x04] = &z80::cb_0x04;
	cb_opcodes[0x05] = &z80::cb_0x05;
	cb_opcodes[0x06] = &z80::cb_0x06;
	cb_opcodes[0x07] = &z80::cb_0x07;
	cb_opcodes[0x08] = &z80::cb_0x08;
	cb_opcodes[0x09] = &z80::cb_0x09;
	cb_opcodes[0x0A] = &z80::cb_0x0A;
	cb_opcodes[0x0B] = &z80::cb_0x0B;
	cb_opcodes[0x0C] = &z80::cb_0x0C;
	cb_opcodes[0x0D] = &z80::cb_0x0D;
	cb_opcodes[0x0E] = &z80::cb_0x0E;
	cb_opcodes[0x0F] = &z80::cb_0x0F;
	cb_opcodes[0x10] = &z80::cb_0x10;
	cb_opcodes[0x11] = &z80::cb_0x11;
	cb_opcodes[0x12] = &z80::cb_0x12;
	cb_opcodes[0x13] = &z80::cb_0x13;
	cb_opcodes[0x14] = &z80::cb_0x14;
	cb_opcodes[0x15] = &z80::cb_0x15;
	cb_opcodes[0x16] = &z80::cb_0x16;
	cb_opcodes[0x17] = &z80::cb_0x17;
	cb_opcodes[0x18] = &z80::cb_0x18;
	cb_opcodes[0x19] = &z80::cb_0x19;
	cb_opcodes[0x1A] = &z80::cb_0x1A;
	cb_opcodes[0x1B] = &z80::cb_0x1B;
	cb_opcodes[0x1C] = &z80::cb_0x1C;
	cb_opcodes[0x1D] = &z80::cb_0x1D;
	cb_opcodes[0x1E] = &z80::cb_0x1E;
	cb_opcodes[0x1F] = &z80::cb_0x1F;
	cb_opcodes[0x20] = &z80::cb_0x20;
	cb_opcodes[0x21] = &z80::cb_0x21;
	cb_opcodes[0x22] = &z80::cb_0x22;
	cb_opcodes[0x23] = &z80::cb_0x23;
	cb_opcodes[0x24] = &z80::cb_0x24;
	cb_opcodes[0x25] = &z80::cb_0x25;
	cb_opcodes[0x26] = &z80::cb_0x26;
	cb_opcodes[0x27] = &z80::cb_0x27;
	cb_opcodes[0x28] = &z80::cb_0x28;
	cb_opcodes[0x29] = &z80::cb_0x29;
	cb_opcodes[0x2A] = &z80::cb_0x2A;
	cb_opcodes[0x2B] = &z80::cb_0x2B;
	cb_opcodes[0x2C] = &z80::cb_0x2C;
	cb_opcodes[0x2D] = &z80::cb_0x2D;
	cb_opcodes[0x2E] = &z80::cb_0x2E;
	cb_opcodes[0x2F] = &z80::cb_0x2F;
	cb_opcodes[0x30] = &z80::cb_0x30;
	cb_opcodes[0x31] = &z80::cb_0x31;
	cb_opcodes[0x32] = &z80::cb_0x32;
	cb_opcodes[0x33] = &z80::cb_0x33;
	cb_opcodes[0x34] = &z80::cb_0x34;
	cb_opcodes[0x35] = &z80::cb_0x35;
	cb_opcodes[0x36] = &z80::cb_0x36;
	cb_opcodes[0x37] = &z80::cb_0x37;
	cb_opcodes[0x38] = &z80::cb_0x38;
	cb_opcodes[0x39] = &z80::cb_0x39;
	cb_opcodes[0x3A] = &z80::cb_0x3A;
	cb_opcodes[0x3B] = &z80::cb_0x3B;
	cb_opcodes[0x3C] = &z80::cb_0x3C;
	cb_opcodes[0x3D] = &z80::cb_0x3D;
	cb_opcodes[0x3E] = &z80::cb_0x3E;
	cb_opcodes[0x3F] = &z80::cb_0x3F;
	cb_opcodes[0x40] = &z80::cb_0x40;
	cb_opcodes[0x41] = &z80::cb_0x41;
	cb_opcodes[0x42] = &z80::cb_0x42;
	cb_opcodes[0x43] = &z80::cb_0x43;
	cb_opcodes[0x44] = &z80::cb_0x44;
	cb_opcodes[0x45] = &z80::cb_0x45;
	cb_opcodes[0x46] = &z80::cb_0x46;
	cb_opcodes[0x47] = &z80::cb_0x47;
	cb_opcodes[0x48] = &z80::cb_0x48;
	cb_opcodes[0x49] = &z80::cb_0x49;
	cb_opcodes[0x4A] = &z80::cb_0x4A;
	cb_opcodes[0x4B] = &z80::cb_0x4B;
	cb_opcodes[0x4C] = &z80::cb_0x4C;
	cb_opcodes[0x4D] = &z80::cb_0x4D;
	cb_opcodes[0x4E] = &z80::cb_0x4E;
	cb_opcodes[0x4F] = &z80::cb_0x4F;
	cb_opcodes[0x50] = &z80::cb_0x50;
	cb_opcodes[0x51] = &z80::cb_0x51;
	cb_opcodes[0x52] = &z80::cb_0x52;
	cb_opcodes[0x53] = &z80::cb_0x53;
	cb_opcodes[0x54] = &z80::cb_0x54;
	cb_opcodes[0x55] = &z80::cb_0x55;
	cb_opcodes[0x56] = &z80::cb_0x56;
	cb_opcodes[0x57] = &z80::cb_0x57;
	cb_opcodes[0x58] = &z80::cb_0x58;
	cb_opcodes[0x59] = &z80::cb_0x59;
	cb_opcodes[0x5A] = &z80::cb_0x5A;
	cb_opcodes[0x5B] = &z80::cb_0x5B;
	cb_opcodes[0x5C] = &z80::cb_0x5C;
	cb_opcodes[0x5D] = &z80::cb_0x5D;
	cb_opcodes[0x5E] = &z80::cb_0x5E;
	cb_opcodes[0x5F] = &z80::cb_0x5F;
	cb_opcodes[0x60] = &z80::cb_0x60;
	cb_opcodes[0x61] = &z80::cb_0x61;
	cb_opcodes[0x62] = &z80::cb_0x62;
	cb_opcodes[0x63] = &z80::cb_0x63;
	cb_opcodes[0x64] = &z80::cb_0x64;
	cb_opcodes[0x65] = &z80::cb_0x65;
	cb_opcodes[0x66] = &z80::cb_0x66;
	cb_opcodes[0x67] = &z80::cb_0x67;
	cb_opcodes[0x68] = &z80::cb_0x68;
	cb_opcodes[0x69] = &z80::cb_0x69;
	cb_opcodes[0x6A] = &z80::cb_0x6A;
	cb_opcodes[0x6B] = &z80::cb_0x6B;
	cb_opcodes[0x6C] = &z80::cb_0x6C;
	cb_opcodes[0x6D] = &z80::cb_0x6D;
	cb_opcodes[0x6E] = &z80::cb_0x6E;
	cb_opcodes[0x6F] = &z80::cb_0x6F;
	cb_opcodes[0x70] = &z80::cb_0x70;
	cb_opcodes[0x71] = &z80::cb_0x71;
	cb_opcodes[0x72] = &z80::cb_0x72;
	cb_opcodes[0x73] = &z80::cb_0x73;
	cb_opcodes[0x74] = &z80::cb_0x74;
	cb_opcodes[0x75] = &z80::cb_0x75;
	cb_opcodes[0x76] = &z80::cb_0x76;
	cb_opcodes[0x77] = &z80::cb_0x77;
	cb_opcodes[0x78] = &z80::cb_0x78;
	cb_opcodes[0x79] = &z80::cb_0x79;
	cb_opcodes[0x7A] = &z80::cb_0x7A;
	cb_opcodes[0x7B] = &z80::cb_0x7B;
	cb_opcodes[0x7C] = &z80::cb_0x7C;
	cb_opcodes[0x7D] = &z80::cb_0x7D;
	cb_opcodes[0x7E] = &z80::cb_0x7E;
	cb_opcodes[0x7F] = &z80::cb_0x7F;
	cb_opcodes[0x80] = &z80::cb_0x80;
	cb_opcodes[0x81] = &z80::cb_0x81;
	cb_opcodes[0x82] = &z80::cb_0x82;
	cb_opcodes[0x83] = &z80::cb_0x83;
	cb_opcodes[0x84] = &z80::cb_0x84;
	cb_opcodes[0x85] = &z80::cb_0x85;
	cb_opcodes[0x86] = &z80::cb_0x86;
	cb_opcodes[0x87] = &z80::cb_0x87;
	cb_opcodes[0x88] = &z80::cb_0x88;
	cb_opcodes[0x89] = &z80::cb_0x89;
	cb_opcodes[0x8A] = &z80::cb_0x8A;
	cb_opcodes[0x8B] = &z80::cb_0x8B;
	cb_opcodes[0x8C] = &z80::cb_0x8C;
	cb_opcodes[0x8D] = &z80::cb_0x8D;
	cb_opcodes[0x8E] = &z80::cb_0x8E;
	cb_opcodes[0x8F] = &z80::cb_0x8F;
	cb_opcodes[0x90] = &z80::cb_0x90;
	cb_opcodes[0x91] = &z80::cb_0x91;
	cb_opcodes[0x92] = &z80::cb_0x92;
	cb_opcodes[0x93] = &z80::cb_0x93;
	cb_opcodes[0x94] = &z80::cb_0x94;
	cb_opcodes[0x95] = &z80::cb_0x95;
	cb_opcodes[0x96] = &z80::cb_0x96;
	cb_opcodes[0x97] = &z80::cb_0x97;
	cb_opcodes[0x98] = &z80::cb_0x98;
	cb_opcodes[0x99] = &z80::cb_0x99;
	cb_opcodes[0x9A] = &z80::cb_0x9A;
	cb_opcodes[0x9B] = &z80::cb_0x9B;
	cb_opcodes[0x9C] = &z80::cb_0x9C;
	cb_opcodes[0x9D] = &z80::cb_0x9D;
	cb_opcodes[0x9E] = &z80::cb_0x9E;
	cb_opcodes[0x9F] = &z80::cb_0x9F;
	cb_opcodes[0xA0] = &z80::cb_0xA0;
	cb_opcodes[0xA1] = &z80::cb_0xA1;
	cb_opcodes[0xA2] = &z80::cb_0xA2;
	cb_opcodes[0xA3] = &z80::cb_0xA3;
	cb_opcodes[0xA4] = &z80::cb_0xA4;
	cb_opcodes[0xA5] = &z80::cb_0xA5;
	cb_opcodes[0xA6] = &z80::cb_0xA6;
	cb_opcodes[0xA7] = &z80::cb_0xA7;
	cb_opcodes[0xA8] = &z80::cb_0xA8;
	cb_opcodes[0xA9] = &z80::cb_0xA9;
	cb_opcodes[0xAA] = &z80::cb_0xAA;
	cb_opcodes[0xAB] = &z80::cb_0xAB;
	cb_opcodes[0xAC] = &z80::cb_0xAC;
	cb_opcodes[0xAD] = &z80::cb_0xAD;
	cb_opcodes[0xAE] = &z80::cb_0xAE;
	cb_opcodes[0xAF] = &z80::cb_0xAF;
	cb_opcodes[0xB0] = &z80::cb_0xB0;
	cb_opcodes[0xB1] = &z80::cb_0xB1;
	cb_opcodes[0xB2] = &z80::cb_0xB2;
	cb_opcodes[0xB3] = &z80::cb_0xB3;
	cb_opcodes[0xB4] = &z80::cb_0xB4;
	cb_opcodes[0xB5] = &z80::cb_0xB5;
	cb_opcodes[0xB6] = &z80::cb_0xB6;
	cb_opcodes[0xB7] = &z80::cb_0xB7;
	cb_opcodes[0xB8] = &z80::cb_0xB8;
	cb_opcodes[0xB9] = &z80::cb_0xB9;
	cb_opcodes[0xBA] = &z80::cb_0xBA;
	cb_opcodes[0xBB] = &z80::cb_0xBB;
	cb_opcodes[0xBC] = &z80::cb_0xBC;
	cb_opcodes[0xBD] = &z80::cb_0xBD;
	cb_opcodes[0xBE] = &z80::cb_0xBE;
	cb_opcodes[0xBF] = &z80::cb_0xBF;
	cb_opcodes[0xC0] = &z80::cb_0xC0;
	cb_opcodes[0xC1] = &z80::cb_0xC1;
	cb_opcodes[0xC2] = &z80::cb_0xC2;
	cb_opcodes[0xC3] = &z80::cb_0xC3;
	cb_opcodes[0xC4] = &z80::cb_0xC4;
	cb_opcodes[0xC5] = &z80::cb_0xC5;
	cb_opcodes[0xC6] = &z80::cb_0xC6;
	cb_opcodes[0xC7] = &z80::cb_0xC7;
	cb_opcodes[0xC8] = &z80::cb_0xC8;
	cb_opcodes[0xC9] = &z80::cb_0xC9;
	cb_opcodes[0xCA] = &z80::cb_0xCA;
	cb_opcodes[0xCB] = &z80::cb_0xCB;
	cb_opcodes[0xCC] = &z80::cb_0xCC;
	cb_opcodes[0xCD] = &z80::cb_0xCD;
	cb_opcodes[0xCE] = &z80::cb_0xCE;
	cb_opcodes[0xCF] = &z80::cb_0xCF;
	cb_opcodes[0xD0] = &z80::cb_0xD0;
	cb_opcodes[0xD1] = &z80::cb_0xD1;
	cb_opcodes[0xD2] = &z80::cb_0xD2;
	cb_opcodes[0xD3] = &z80::cb_0xD3;
	cb_opcodes[0xD4] = &z80::cb_0xD4;
	cb_opcodes[0xD5] = &z80::cb_0xD5;
	cb_opcodes[0xD6] = &z80::cb_0xD6;
	cb_opcodes[0xD7] = &z80::cb_0xD7;
	cb_opcodes[0xD8] = &z80::cb_0xD8;
	cb_opcodes[0xD9] = &z80::cb_0xD9;
	cb_opcodes[0xDA] = &z80::cb_0xDA;
	cb_opcodes[0xDB] = &z80::cb_0xDB;
	cb_opcodes[0xDC] = &z80::cb_0xDC;
	cb_opcodes[0xDD] = &z80::cb_0xDD;
	cb_opcodes[0xDE] = &z80::cb_0xDE;
	cb_opcodes[0xDF] = &z80::cb_0xDF;
	cb_opcodes[0xE0] = &z80::cb_0xE0;
	cb_opcodes[0xE1] = &z80::cb_0xE1;
	cb_opcodes[0xE2] = &z80::cb_0xE2;
	cb_opcodes[0xE3] = &z80::cb_0xE3;
	cb_opcodes[0xE4] = &z80::cb_0xE4;
	cb_opcodes[0xE5] = &z80::cb_0xE5;
	cb_opcodes[0xE6] = &z80::cb_0xE6;
	cb_opcodes[0xE7] = &z80::cb_0xE7;
	cb_opcodes[0xE8] = &z80::cb_0xE8;
	cb_opcodes[0xE9] = &z80::cb_0xE9;
	cb_opcodes[0xEA] = &z80::cb_0xEA;
	cb_opcodes[0xEB] = &z80::cb_0xEB;
	cb_opcodes[0xEC] = &z80::cb_0xEC;
	cb_opcodes[0xED] = &z80::cb_0xED;
	cb_opcodes[0xEE] = &z80::cb_0xEE;
	cb_opcodes[0xEF] = &z80::cb_0xEF;
	cb_opcodes[0xF0] = &z80::cb_0xF0;
	cb_opcodes[0xF1] = &z80::cb_0xF1;
	cb_opcodes[0xF2] = &z80::cb_0xF2;
	cb_opcodes[0xF3] = &z80::cb_0xF3;
	cb_opcodes[0xF4] = &z80::cb_0xF4;
	cb_opcodes[0xF5] = &z80::cb_0xF5;
	cb_opcodes[0xF6] = &z80::cb_0xF6;
	cb_opcodes[0xF7] = &z80::cb_0xF7;
	cb_opcodes[0xF8] = &z80::cb_0xF8;
	cb_opcodes[0xF9] = &z80::cb_0xF9;
	cb_opcodes[0xFA] = &z80::cb_0xFA;
	cb_opcodes[0xFB] = &z80::cb_0xFB;
	cb_opcodes[0xFC] = &z80::cb_0xFC;
	cb_opcodes[0xFD] = &z80::cb_0xFD;
	cb_opcodes[0xFE] = &z80::cb_0xFE;
	cb_opcodes[0xFF] = &z80::cb_0xFF;


	pc=ENTRY_POINT;
	in_bios = false;
	sp=0xFFFE;
	reg8[H] = 0x01;
	reg8[L]= 0x4D;
	reg8[C]=0x13;
	reg8[E]=0xD8;
	reg8[A]=0x01;

}



// TODO: Check contents of each function are correct
// TODO: Fix t_cycle
// TODO: Add tests for each instruction
// TODO: Verify cycles and opcode mappings

void z80::f_UNMAPPED(int opcode) {
	fprintf(stderr,"Unmapped entry 0x%02X\n",opcode);
	throw "Exception triggered on unmapped entry";
}

// Normal opcodes

// NOP
void z80::f_0x00() {
	printf("[0x%04X] NOP\n",pc-1);
	m_cycle = 1;
	//t_cycle = 4;
}

// LD BC,d16
void z80::f_0x01() {
	reg8[C] = read_byte(pc);
	reg8[B] = read_byte(pc+1);
	pc+=2;
	m_cycle = 3;
	//t_cycle = 12;
	printf("[0x%04X] LD BC,0x%04X\n",pc-1,(reg8[B] << 8)+reg8[C]);
}

// LD (BC),A
void z80::f_0x02() {
	write_byte((reg8[B] << 8)+reg8[C],reg8[A]);
	m_cycle = 2;
	//t_cycle = 8;
	printf("[0x%04X] LD (BC),A\n",pc-1);
}

// INC BC
void z80::f_0x03() {
    reg8[C] = (reg8[C] + 1) & 0xFF;
    if (!reg8[C]) {
    	reg8[B] = (reg8[B] + 1) & 0xFF;
    }
    m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] INC BC\n",pc-1);
}

// INC B
void z80::f_0x04() {
    reg8[B]++;
    reg8[B] &= 0xFF;
    reg8[F] = reg8[B] ? 0 : ZERO_FLAG;
    m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] INC B\n",pc-1);
}

// DEC B
void z80::f_0x05() {
    reg8[B]--;
    reg8[B] &= 0xFF;
    reg8[F] = reg8[B] ? 0 : ZERO_FLAG;
    m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] DEC B\n",pc-1);
}

// LD B, d8
void z80::f_0x06() {
	reg8[B] = read_byte(pc);
	printf("[0x%04X] LD B, 0x%02X\n",pc-1,reg8[B]);
	pc++;
	m_cycle = 2;
	//t_cycle = 8;
}

// RLCA
void z80::f_0x07() {
    int ci = reg8[A] & ZERO_FLAG ? 1 : 0;
    int co = reg8[A] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] << 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[F] & 0xEF) + co;
	m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] RCLA\n",pc-1);
}

// LD a16 SP
void z80::f_0x08() {
	sp = read_word(pc);
	printf("[0x%04X] LD 0x%04X, SP\n",pc-1,sp);
	pc+=2;
	m_cycle = 3;
	//t_cycle = 20;
}

// ADD HL,BC
void z80::f_0x09() {
    int hl = (reg8[H] << 8) + reg8[L];
    hl += (reg8[B] << 8) + reg8[C];
    if (hl > 0xFFFF) {
    	reg8[F] |= CARRY_FLAG;
    } else {
    	reg8[F] &= 0xEF;
    }
    reg8[H] = (hl >> 8) & 0xFF;
    reg8[L] = hl & 0xFF;
	m_cycle = 1;
	//t_cycle = 8;
	printf("[0x%04X] ADD HL BC\n",pc-1);
}

// LD A,(BC)
void z80::f_0x0A() {
    reg8[A] = read_byte((reg8[B] << 8) + reg8[C]);
	m_cycle = 1;
	//t_cycle = 8;
	printf("[0x%04X] LD A, (BC)\n",pc-1);
}

// DEC BC
void z80::f_0x0B() {
    reg8[C] = (reg8[C] - 1) & 0xFF;
    if (reg8[C] == 0xFF) {
    	reg8[B] = (reg8[B] - 1) & 0xFF;
    }
    m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] DEC BC\n",pc-1);
}

// INC C
void z80::f_0x0C() {
    reg8[C]++;
    reg8[C] &= 0xFF;
    reg8[F] = reg8[C] ? 0 : ZERO_FLAG;
    m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] INC C\n",pc-1);
}

// DEC C
void z80::f_0x0D() {
    reg8[C]--;
    reg8[C] &= 0xFF;
    reg8[F] = reg8[C] ? 0 : ZERO_FLAG;
    m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] DEC C\n",pc-1);
}

// LD C, d8
void z80::f_0x0E() {
    reg8[C] = read_byte(pc);
	printf("[0x%04X] LD C, 0x%02X\n",pc-1,reg8[C]);
    pc++;
    m_cycle = 2;
	//t_cycle = 8;
}

// RRCA
void z80::f_0x0F() {
    int ci = reg8[A] & 1 ? ZERO_FLAG : 0;
    int co = reg8[A] & 1 ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] >> 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] RRCA\n",pc-1);
}

// DJNZ
void z80::f_0x10() {
	printf("[0x%04X] DJNZn\n",pc-1);
    int i = read_byte(pc);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    }
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
    reg8[B]--;
    if (reg8[B]) {
        pc += i;
        m_cycle++;
    //    t_cycle+= 4;
    }
}

//LD DE, d16
void z80::f_0x11() {
    reg8[E] = read_byte(pc);
    reg8[D] = read_byte(pc + 1);
    pc += 2;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] LD DE, 0x%04X\n",pc-1,(reg8[D] << 8)+reg8[E]);
}

// LD (DE),A
void z80::f_0x12() {
    write_byte((reg8[D] << 8) + reg8[E], reg8[A]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (DE),A\n",pc-1);
}

// INC DE
void z80::f_0x13() {
    reg8[E] = (reg8[E] + 1) & 0xFF;
    if (!reg8[E]) {
    	reg8[D] = (reg8[D] + 1) & 0xFF;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC DE\n",pc-1);
}

// INC D
void z80::f_0x14() {
    reg8[D]++;
    reg8[D] &= 0xFF;
    reg8[F] = reg8[D] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC D\n",pc-1);
}

// DEC D
void z80::f_0x15() {
    reg8[D]--;
    reg8[D] &= 0xFF;
    reg8[F] = reg8[D] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC D\n",pc-1);
}

// LD D,d8
void z80::f_0x16() {
    reg8[D] = read_byte(pc);
	printf("[0x%04X] LD D, 0x%02X\n",pc-1,reg8[D]);
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
}

// RLA
void z80::f_0x17() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[A] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] << 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] RLA\n",pc-1);
}
// JR r8
void z80::f_0x18() {
    int i = read_byte(pc);
	printf("[0x%04X] JR 0x%02X\n",pc-1,i);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    	std::cerr << i << std::endl;
    	throw "";
    }
    pc++;
    m_cycle = 2;
    pc += i;
    m_cycle++;
    //t_cycle += 4;
}

// ADD HL,DE
void z80::f_0x19() {
    int hl = (reg8[H] << 8) + reg8[L];
    hl += (reg8[D] << 8) + reg8[E];
    if (hl > 0xFFFF) {
    	reg8[F] |= CARRY_FLAG;
    } else {
    	reg8[F] &= 0xEF;
    }
    reg8[H] = (hl >> 8) & 0xFF;
    reg8[L] = hl & 0xFF;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] ADD HL,DE\n",pc-1);
}

// LD A,(DE)
void z80::f_0x1A() {
    reg8[A] = read_byte((reg8[D] << 8) + reg8[E]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD A,(DE)\n",pc-1);
}

// DEC DE
void z80::f_0x1B() {
    reg8[E] = (reg8[E] - 1) & 0xFF;
    if (reg8[E] == 0xFF) {
    	reg8[D] = (reg8[D] - 1) & 0xFF;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC DE\n",pc-1);
}

// INC E
void z80::f_0x1C() {
    reg8[E]++;
    reg8[E] &= 0xFF;
    reg8[F] = reg8[E] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC E\n",pc-1);
}

// DEC E
void z80::f_0x1D() {
    reg8[E]--;
    reg8[E] &= 0xFF;
    reg8[F] = reg8[E] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC E\n",pc-1);
}

// LD E,d8
void z80::f_0x1E() {
	printf("[0x%04X] LD E,0x%02X\n",pc-1,read_byte(pc));
    reg8[E] = read_byte(pc);
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
}

// RRA
void z80::f_0x1F() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[A] & 1 ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] >> 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] RRA\n",pc-1);
}

// JR NZ,r8
void z80::f_0x20() {
    int i = read_byte(pc);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    }
    printf("[0x%04X] JR NZ,0x%02X\n",pc-1,i);
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
    if ((reg8[F] & ZERO_FLAG) == 0x00) {
        pc += i;
        m_cycle++;
        //t_cycle += 4;
    }
}

// LD HL,d16
void z80::f_0x21() {
	printf("[0x%04X] LD HL,0x%04X\n",pc-1,(reg8[H] << 8)+reg8[L]);
    reg8[L] = read_byte(pc);
    reg8[H] = read_byte(pc + 1);
    pc += 2;
    m_cycle = 3;
    //t_cycle = 12;
}

// LD (HL+),A
void z80::f_0x22() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[A]);
    reg8[L] = (reg8[L] + 1) & 0xFF;
    if (!reg8[L]) {
    	reg8[H] = (reg8[H] + 1) & 0xFF;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL+),A\n",pc-1);
}

// INC HL
void z80::f_0x23() {
    reg8[L] = (reg8[L] + 1) & 0xFF;
    if (!reg8[L]) {
    	reg8[H] = (reg8[H] + 1) & 0xFF;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC HL\n",pc-1);
}

// INC H
void z80::f_0x24() {
    reg8[H]++;
    reg8[H] &= 0xFF;
    reg8[F] = reg8[H] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC H\n",pc-1);
}

// DEC H
void z80::f_0x25() {
    reg8[H]--;
    reg8[H] &= 0xFF;
    reg8[F] = reg8[H] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC H\n",pc-1);
}

// LD H,d8
void z80::f_0x26() {
	printf("[0x%04X] LD H,0x%02X\n",pc-1,read_byte(pc));
    reg8[H] = read_byte(pc);
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
}

// DAA
void z80::f_0x27() {
    int a = reg8[A];
    if ((reg8[F] & HALF_CARRY_FLAG) || ((reg8[A] & 15) > 9)) {
    	reg8[A] += 6;
    }
    reg8[F] &= 0xEF;
    if ((reg8[F] & HALF_CARRY_FLAG) || (a > 0x99)) {
        reg8[A] += 0x60;
        reg8[F] |= CARRY_FLAG;
    }
    m_cycle = 1;
    printf("[0x%04X] DAA\n",pc-1);
}

// JR Z,r8
void z80::f_0x28() {
    int i = read_byte(pc);
    printf("[0x%04X] JR Z,0x%02X\n",pc-1,i);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    }
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
    if ((reg8[F] & ZERO_FLAG) == ZERO_FLAG) {
        pc += i;
        m_cycle++;
        //t_cycle += 4;
    }
}

// ADD HL,HL
void z80::f_0x29() {
    int hl = (reg8[H] << 8) + reg8[L];
    hl += (reg8[H] << 8) + reg8[L];
    if (hl > 0xFFFF) {
    	reg8[F] |= CARRY_FLAG;
    } else {
    	reg8[F] &= 0xEF;
    }
    reg8[H] = (hl >> 8) & 0xFF;
    reg8[L] = hl & 0xFF;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] ADD HL,HL\n",pc-1);
}

// LD A,(HL+)
void z80::f_0x2A() {
    reg8[A] = read_byte((reg8[H] << 8) + reg8[L]);
    reg8[L] = (reg8[L] + 1) & 0xFF;
    if (!reg8[L]) reg8[H] = (reg8[H] + 1) & 0xFF;
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD A,(HL+)\n",pc-1);
}

// DEC HL
void z80::f_0x2B() {
    reg8[L] = (reg8[L] - 1) & 0xFF;
    if (reg8[L] == 0xFF) {
    	reg8[H] = (reg8[H] - 1) & 0xFF;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC HL\n",pc-1);
}

// INC L
void z80::f_0x2C() {
    reg8[L]++;
    reg8[L] &= 0xFF;
    reg8[F] = reg8[L] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC L\n",pc-1);
}

// DEC L
void z80::f_0x2D() {
    reg8[L]--;
    reg8[L] &= 0xFF;
    reg8[F] = reg8[L] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC L\n",pc-1);
}

// LD L,d8
void z80::f_0x2E() {
	printf("[0x%04X] LD L,0x%02X\n",pc-1,read_byte(pc));
    reg8[L] = read_byte(pc);
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
}

// CPL
void z80::f_0x2F() {
    reg8[A] ^= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CPL\n",pc-1);
}

// JR NC,r8
void z80::f_0x30() {
    int i = read_byte(pc);
    printf("[0x%04X] JR NC,0x%02X\n",pc-1,i);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    }
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
    if ((reg8[F] & CARRY_FLAG) == 0x00) {
        pc += i;
        m_cycle++;
        //t_cycle += 4;
    }
}

// LD SP,d16
void z80::f_0x31() {
	printf("[0x%04X] LD SP,0x%04X\n",pc-1,read_word(pc));
    sp = read_word(pc);
    pc += 2;
    m_cycle = 3;
    //t_cycle = 12;
}

// LD (HL-),A
void z80::f_0x32() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[A]);
    reg8[L] = (reg8[L] - 1) & 0xFF;
    if (reg8[L] == 0xFF) {
    	reg8[H] = (reg8[H] - 1) & 0xFF;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL-),A\n",pc-1);
}

// INC SP
void z80::f_0x33() {
    sp = (sp + 1) & 0xFFFF;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC SP\n",pc-1);
}

// INC (HL)
void z80::f_0x34() {
    int i = read_byte((reg8[H] << 8) + reg8[L]) + 1;
    i &= 0xFF;
    write_byte((reg8[H] << 8) + reg8[L], i);
    reg8[F] = i ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] INC (HL)\n",pc-1);
}

// DEC HL
void z80::f_0x35() {
    int i = read_byte((reg8[H] << 8) + reg8[L]) - 1;
    i &= 0xFF;
    write_byte((reg8[H] << 8) + reg8[L], i);
    reg8[F] = i ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] DEC HL\n",pc-1);
}

// LD (HL),d8
void z80::f_0x36() {
	printf("[0x%04X] LD (HL),0x%02X\n",pc-1,read_byte(pc));
    write_byte((reg8[H] << 8) + reg8[L], read_byte(pc));
    pc++;
    m_cycle = 3;
    //t_cycle = 12;
}

// SCF
void z80::f_0x37() {
    reg8[F] |= CARRY_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SCF\n",pc-1);
}

//JR C,r8
void z80::f_0x38() {
    int i = read_byte(pc);
    printf("[0x%04X] JR C,0x%02X\n",pc-1,i);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    }
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
    if ((reg8[F] & CARRY_FLAG) == CARRY_FLAG) {
        pc += i;
        m_cycle++;
        //t_cycle += 4;
    }
}

// ADD HL,SP
void z80::f_0x39() {
    int hl = (reg8[H] << 8) + reg8[L];
    hl += sp;
    if (hl > 0xFFFF) {
    	reg8[F] |= CARRY_FLAG;
    } else {
    	reg8[F] &= 0xEF;
    }
    reg8[H] = (hl >> 8) & 0xFF;
    reg8[L] = hl & 0xFF;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] ADD HL,SP\n",pc-1);
}

// LD A,(HL-)
void z80::f_0x3A() {
    reg8[A] = read_byte((reg8[H] << 8) + reg8[L]);
    reg8[L] = (reg8[L] - 1) & 0xFF;
    if (reg8[L] == 0xFF) {
    	reg8[H] = (reg8[H] - 1) & 0xFF;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD A,(HL-)\n",pc-1);
}

// DEC SP
void z80::f_0x3B() {
    sp = (sp - 1) & 0xFFFF;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC SP\n",pc-1);
}

// INC A
void z80::f_0x3C() {
    reg8[A]++;
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] INC A\n",pc-1);
}

// DEC A
void z80::f_0x3D() {
    reg8[A]--;
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DEC A\n",pc-1);
}

// LD A,d8
void z80::f_0x3E() {
	printf("[0x%04X] LD A,0x%02X\n",pc-1,read_byte(pc));
    reg8[A] = read_byte(pc);
    pc++;
    m_cycle = 2;
    //t_cycle = 8;
}

// CCF
void z80::f_0x3F() {
    int ci = reg8[F] & CARRY_FLAG ? 0 : CARRY_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + ci;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CCF\n",pc-1);
}

// LD B,B
void z80::f_0x40() {
    //reg8[B] = reg8[B];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD B,B\n",pc-1);
}

// LD B,C
void z80::f_0x41() {
    reg8[B] = reg8[C];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD B,C\n",pc-1);
}

// LD B,D
void z80::f_0x42() {
    reg8[B] = reg8[D];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD B,D\n",pc-1);
}

// LD B,E
void z80::f_0x43() {
    reg8[B] = reg8[E];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD B,E\n",pc-1);
}

// LD B,H
void z80::f_0x44() {
    reg8[B] = reg8[H];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD B,H\n",pc-1);
}

// LD B,L
void z80::f_0x45() {
    reg8[B] = reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD B,L\n",pc-1);
}

// LD B,(HL)
void z80::f_0x46() {
    reg8[B] = read_byte((reg8[H] << 8) + reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD B,(HL)\n",pc-1);
}

// LD B,A
void z80::f_0x47() {
    reg8[B] = reg8[A];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD B,A\n",pc-1);
}

// LD C,B
void z80::f_0x48() {
    reg8[C] = reg8[B];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD C,B\n",pc-1);
}

// LD C,C
void z80::f_0x49() {
    //reg8[C] = reg8[C];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD C,C\n",pc-1);
}

// LD C,D
void z80::f_0x4A() {
    reg8[C] = reg8[D];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD C,D\n",pc-1);
}

// LD C,E
void z80::f_0x4B() {
    reg8[C] = reg8[E];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD C,E\n",pc-1);
}

// LD C,H
void z80::f_0x4C() {
    reg8[C] = reg8[H];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD C,H\n",pc-1);
}

// LD C,L
void z80::f_0x4D() {
    reg8[C] = reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD C,L\n",pc-1);
}

// LD C,(HL)
void z80::f_0x4E() {
    reg8[C] = read_byte((reg8[H] << 8) + reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD C,(HL)\n",pc-1);
}

// LD C,A
void z80::f_0x4F() {
    reg8[C] = reg8[A];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD C,A\n",pc-1);
}

// LD D,B
void z80::f_0x50() {
    reg8[D] = reg8[B];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD D,B\n",pc-1);
}

// LD D,C
void z80::f_0x51() {
    reg8[D] = reg8[C];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD D,C\n",pc-1);
}

// LD D,D
void z80::f_0x52() {
    //reg8[D] = reg8[D];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD D,D\n",pc-1);
}

// LD D,E
void z80::f_0x53() {
    reg8[D] = reg8[E];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD D,E\n",pc-1);
}

// LD D,H
void z80::f_0x54() {
    reg8[D] = reg8[H];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD D,H\n",pc-1);
}

// LD D,L
void z80::f_0x55() {
    reg8[D] = reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD D,L\n",pc-1);
}

// LD D,(HL)
void z80::f_0x56() {
    reg8[D] = read_byte((reg8[H] << 8) + reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD D,(HL)\n",pc-1);
}

// LD D,A
void z80::f_0x57() {
    reg8[D] = reg8[A];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD D,A\n",pc-1);
}

// LD E,B
void z80::f_0x58() {
    reg8[E] = reg8[B];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD E,B\n",pc-1);
}

// LD E,C
void z80::f_0x59() {
    reg8[E] = reg8[C];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD E,C\n",pc-1);
}

// LD E,D
void z80::f_0x5A() {
    reg8[E] = reg8[D];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD E,D\n",pc-1);
}

// LD E,E
void z80::f_0x5B() {
    //reg8[E] = reg8[E];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD E,D\n",pc-1);
}

// LD E,H
void z80::f_0x5C() {
    reg8[E] = reg8[H];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD E,H\n",pc-1);
}

// LD E,L
void z80::f_0x5D() {
    reg8[E] = reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD E,L\n",pc-1);
}

// LD E,(HL)
void z80::f_0x5E() {
    reg8[E] = read_byte((reg8[H] << 8) + reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD E,(HL)\n",pc-1);
}

// LD E,A
void z80::f_0x5F() {
    reg8[E] = reg8[A];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD E,A\n",pc-1);
}

// LD H,B
void z80::f_0x60() {
    reg8[H] = reg8[B];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD H,B\n",pc-1);
}

// LD H,C
void z80::f_0x61() {
    reg8[H] = reg8[C];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD H,C\n",pc-1);
}

// LD H,D
void z80::f_0x62() {
    reg8[H] = reg8[D];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD H,D\n",pc-1);
}

// LD H,E
void z80::f_0x63() {
    reg8[H] = reg8[E];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD H,E\n",pc-1);
}

// LD H,H
void z80::f_0x64() {
    //reg8[H] = reg8[H];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD H,H\n",pc-1);
}

// LD H,L
void z80::f_0x65() {
    reg8[H] = reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD H,L\n",pc-1);
}

// LD H,(HL)
void z80::f_0x66() {
    reg8[H] = read_byte((reg8[H] << 8) + reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD H,(HL)\n",pc-1);
}

// LD H,A
void z80::f_0x67() {
    reg8[H] = reg8[A];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD H,A\n",pc-1);
}

// LD L,B
void z80::f_0x68() {
    reg8[L] = reg8[B];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD L,B\n",pc-1);
}

// LD L,C
void z80::f_0x69() {
    reg8[L] = reg8[C];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD L,C\n",pc-1);
}

// LD L,D
void z80::f_0x6A() {
    reg8[L] = reg8[D];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD L,D\n",pc-1);
}

// LD L,E
void z80::f_0x6B() {
    reg8[L] = reg8[E];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD L,E\n",pc-1);
}

// LD L,H
void z80::f_0x6C() {
    reg8[L] = reg8[H];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD L,H\n",pc-1);
}

// LD L,L
void z80::f_0x6D() {
    //reg8[L] = reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD L,L\n",pc-1);
}

// LD L,(HL)
void z80::f_0x6E() {
    reg8[L] = read_byte((reg8[H] << 8) + reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD L,(HL)\n",pc-1);
}

// LD L,A
void z80::f_0x6F() {
    reg8[L] = reg8[A];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD L,L\n",pc-1);
}

// LD (HL),B
void z80::f_0x70() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[B]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL),B\n",pc-1);
}

// LD (HL),C
void z80::f_0x71() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[C]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL),C\n",pc-1);
}

// LD (HL),D
void z80::f_0x72() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[D]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL),D\n",pc-1);
}

// LD (HL),E
void z80::f_0x73() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[E]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL),E\n",pc-1);
}

// LD (HL),H
void z80::f_0x74() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[H]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL),H\n",pc-1);
}

// LD (HL),L
void z80::f_0x75() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL),L\n",pc-1);
}

// HALT
void z80::f_0x76() {
    halt = true;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] HALT\n",pc-1);
}

// LD (HL),A
void z80::f_0x77() {
    write_byte((reg8[H] << 8) + reg8[L], reg8[A]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (HL),A\n",pc-1);
}

// LD A,B
void z80::f_0x78() {
    reg8[A] = reg8[B];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD A,B\n",pc-1);
}

// LD A,C
void z80::f_0x79() {
    reg8[A] = reg8[C];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD A,C\n",pc-1);
}

// LD A,D
void z80::f_0x7A() {
    reg8[A] = reg8[D];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD A,D\n",pc-1);
}

// LD A,E
void z80::f_0x7B() {
    reg8[A] = reg8[E];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD A,E\n",pc-1);
}

// LD A,H
void z80::f_0x7C() {
    reg8[A] = reg8[H];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD A,H\n",pc-1);
}

// LD A,L
void z80::f_0x7D() {
    reg8[A] = reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD A,L\n",pc-1);
}

// LD A,(HL)
void z80::f_0x7E() {
    reg8[A] = read_byte((reg8[H] << 8) + reg8[L]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD A,(HL)\n",pc-1);
}

// LD A,A
void z80::f_0x7F() {
    //reg8[A] = reg8[A];
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] LD A,A\n",pc-1);
}

// ADD A,B
void z80::f_0x80() {
    int a = reg8[A];
    reg8[A] += reg8[B];
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[B] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADD A,B\n",pc-1);
}

// ADD A,C
void z80::f_0x81() {
    int a = reg8[A];
    reg8[A] += reg8[C];
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[C] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADD A,C\n",pc-1);
}

// ADD A,D
void z80::f_0x82() {
    int a = reg8[A];
    reg8[A] += reg8[D];
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[D] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADD A,D\n",pc-1);
}

// ADD A,E
void z80::f_0x83() {
    int a = reg8[A];
    reg8[A] += reg8[E];
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[E] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADD A,E\n",pc-1);
}

// ADD A,H
void z80::f_0x84() {
    int a = reg8[A];
    reg8[A] += reg8[H];
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[H] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADD A,H\n",pc-1);
}

// ADD A,L
void z80::f_0x85() {
    int a = reg8[A];
    reg8[A] += reg8[L];
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[L] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADD A,L\n",pc-1);
}

// ADD A,(HL)
void z80::f_0x86() {
    int a = reg8[A];
    int m = read_byte((reg8[H] << 8) + reg8[L]);
    reg8[A] += m;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ a ^ m) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] ADD A,(HL)\n",pc-1);
}

// ADD A,A
void z80::f_0x87() {
    int a = reg8[A];
    reg8[A] += reg8[A];
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[A] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADD A,A\n",pc-1);
}

// ADC A,B
void z80::f_0x88() {
    int a = reg8[A];
    reg8[A] += reg8[B];
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[B] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADC A,B\n",pc-1);
}

// ADC A,C
void z80::f_0x89() {
    int a = reg8[A];
    reg8[A] += reg8[C];
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[C] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADC A,C\n",pc-1);
}

// ADC A,D
void z80::f_0x8A() {
    int a = reg8[A];
    reg8[A] += reg8[D];
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) reg8[F] |= ZERO_FLAG;
    if ((reg8[A] ^ reg8[D] ^ a) & CARRY_FLAG) reg8[F] |= HALF_CARRY_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADC A,D\n",pc-1);
}

// ADC A,E
void z80::f_0x8B() {
    int a = reg8[A];
    reg8[A] += reg8[E];
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[E] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADC A,E\n",pc-1);
}

// ADC A,H
void z80::f_0x8C() {
    int a = reg8[A];
    reg8[A] += reg8[H];
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[H] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADC A,H\n",pc-1);
}

// ADC A,L
void z80::f_0x8D() {
    int a = reg8[A];
    reg8[A] += reg8[L];
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[L] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADC A,L\n",pc-1);
}

// ADC A,(HL)
void z80::f_0x8E() {
    int a = reg8[A];
    int m = read_byte((reg8[H] << 8) + reg8[L]);
    reg8[A] += m;
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ m ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] ADC A,(HL)\n",pc-1);
}

// ADC A,A
void z80::f_0x8F() {
    int a = reg8[A];
    reg8[A] += reg8[A];
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[A] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] ADC A,A\n",pc-1);
}

// SUB B
void z80::f_0x90() {
    int a = reg8[A];
    reg8[A] -= reg8[B];
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[B] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SUB B\n",pc-1);
}

// SUB C
void z80::f_0x91() {
    int a = reg8[A];
    reg8[A] -= reg8[C];
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[C] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SUB C\n",pc-1);
}

// SUB D
void z80::f_0x92() {
    int a = reg8[A];
    reg8[A] -= reg8[D];
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[D] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SUB D\n",pc-1);
}

// SUB E
void z80::f_0x93() {
    int a = reg8[A];
    reg8[A] -= reg8[E];
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[E] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SUB E\n",pc-1);
}

// SUB H
void z80::f_0x94() {
    int a = reg8[A];
    reg8[A] -= reg8[H];
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[H] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SUB H\n",pc-1);
}

// SUB L
void z80::f_0x95() {
    int a = reg8[A];
    reg8[A] -= reg8[L];
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[L] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SUB L\n",pc-1);
}

// SUB (HL)
void z80::f_0x96() {
    int a = reg8[A];
    int m = read_byte((reg8[H] << 8) + reg8[L]);
    reg8[A] -= m;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ m ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] SUB (HL)\n",pc-1);
}

// SUB A
void z80::f_0x97() {
    int a = reg8[A];
    reg8[A] -= reg8[A];
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[A] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SUB A\n",pc-1);
}

// SBC A,B
void z80::f_0x98() {
    int a = reg8[A];
    reg8[A] -= reg8[B];
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[B] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SBC A,B\n",pc-1);
}

// SBC A,C
void z80::f_0x99() {
    int a = reg8[A];
    reg8[A] -= reg8[C];
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[C] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SBC A,C\n",pc-1);
}

// SBC A,D
void z80::f_0x9A() {
    int a = reg8[A];
    reg8[A] -= reg8[D];
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[D] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SBC A,D\n",pc-1);
}

// SBC A,E
void z80::f_0x9B() {
    int a = reg8[A];
    reg8[A] -= reg8[E];
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[E] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SBC A,E\n",pc-1);
}

// SBC A,H
void z80::f_0x9C() {
    int a = reg8[A];
    reg8[A] -= reg8[H];
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[H] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SBC A,H\n",pc-1);
}

// SBC A,L
void z80::f_0x9D() {
    int a = reg8[A];
    reg8[A] -= reg8[L];
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[L] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SBC A,L\n",pc-1);
}

// SBC A,(HL)
void z80::f_0x9E() {
    int a = reg8[A];
    int m = read_byte((reg8[H] << 8) + reg8[L]);
    reg8[A] -= m;
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ m ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] SBC A,(HL)\n",pc-1);
}

// SBC A,A
void z80::f_0x9F() {
    int a = reg8[A];
    reg8[A] -= reg8[A];
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[A] ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] SBC A,A\n",pc-1);
}

// AND B
void z80::f_0xA0() {
    reg8[A] &= reg8[B];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] AND B\n",pc-1);
}

// AND C
void z80::f_0xA1() {
    reg8[A] &= reg8[C];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] AND C\n",pc-1);
}

// AND D
void z80::f_0xA2() {
    reg8[A] &= reg8[D];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] AND D\n",pc-1);
}

// AND E
void z80::f_0xA3() {
    reg8[A] &= reg8[E];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] AND E\n",pc-1);
}

// AND H
void z80::f_0xA4() {
    reg8[A] &= reg8[H];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] AND H\n",pc-1);
}

// AND L
void z80::f_0xA5() {
    reg8[A] &= reg8[L];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] AND L\n",pc-1);
}

// AND (HL)
void z80::f_0xA6() {
    reg8[A] &= read_byte((reg8[H] << 8) + reg8[L]);
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] AND (HL)\n",pc-1);
}

// AND A
void z80::f_0xA7() {
    reg8[A] &= reg8[A];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] AND A\n",pc-1);
}

// XOR B
void z80::f_0xA8() {
    reg8[A] ^= reg8[B];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] XOR B\n",pc-1);
}

// XOR C
void z80::f_0xA9() {
    reg8[A] ^= reg8[C];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] XOR C\n",pc-1);
}

// XOR D
void z80::f_0xAA() {
    reg8[A] ^= reg8[D];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] XOR D\n",pc-1);
}

// XOR E
void z80::f_0xAB() {
    reg8[A] ^= reg8[E];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
	m_cycle = 1;
	//t_cycle = 4;
	printf("[0x%04X] XOR E\n",pc-1);
}

// XOR H
void z80::f_0xAC() {
    reg8[A] ^= reg8[H];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] XOR H\n",pc-1);
}

// XOR L
void z80::f_0xAD() {
    reg8[A] ^= reg8[L];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] XOR L\n",pc-1);
}

// XOR (HL)
void z80::f_0xAE() {
    reg8[A] ^= read_byte((reg8[H] << 8) + reg8[L]);
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] XOR (HL)\n",pc-1);
}

// XOR A
void z80::f_0xAF() {
    reg8[A] ^= reg8[A];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] XOR A\n",pc-1);
}

// OR B
void z80::f_0xB0() {
    reg8[A] |= reg8[B];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] OR B\n",pc-1);
}

// OR C
void z80::f_0xB1() {
    reg8[A] |= reg8[C];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] OR C\n",pc-1);
}

// OR D
void z80::f_0xB2() {
    reg8[A] |= reg8[D];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] OR D\n",pc-1);
}

// OR E
void z80::f_0xB3() {
    reg8[A] |= reg8[E];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] OR E\n",pc-1);
}

// OR H
void z80::f_0xB4() {
    reg8[A] |= reg8[H];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] OR H\n",pc-1);
}

// OR L
void z80::f_0xB5() {
    reg8[A] |= reg8[L];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] OR L\n",pc-1);
}

// OR (HL)
void z80::f_0xB6() {
    reg8[A] |= read_byte((reg8[H] << 8) + reg8[L]);
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] OR (HL)\n",pc-1);
}

// OR A
void z80::f_0xB7() {
    reg8[A] |= reg8[A];
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] OR A\n",pc-1);
}

// CP B
void z80::f_0xB8() {
    int i = reg8[A];
    i -= reg8[B];
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[B] ^ i) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CP B\n",pc-1);
}

// CP C
void z80::f_0xB9() {
    int i = reg8[A];
    i -= reg8[C];
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[C] ^ i) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CP C\n",pc-1);
}

// CP D
void z80::f_0xBA() {
    int i = reg8[A];
    i -= reg8[D];
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[D] ^ i) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CP D\n",pc-1);
}

// CP E
void z80::f_0xBB() {
    int i = reg8[A];
    i -= reg8[E];
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[E] ^ i) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CP E\n",pc-1);
}

// CP H
void z80::f_0xBC() {
    int i = reg8[A];
    i -= reg8[H];
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[H] ^ i) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CP H\n",pc-1);
}

// CP L
void z80::f_0xBD() {
    int i = reg8[A];
    i -= reg8[L];
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[L] ^ i) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CP L\n",pc-1);
}

// CP (HL)
void z80::f_0xBE() {
    int i = reg8[A];
    int m = read_byte((reg8[H] << 8) + reg8[L]);
    i -= m;
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ i ^ m) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] CP (HL)\n",pc-1);
}

// CP A
void z80::f_0xBF() {
    int i = reg8[A];
    i -= reg8[A];
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ reg8[A] ^ i) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] CP A\n",pc-1);
}

// RET NZ
void z80::f_0xC0() {
    m_cycle = 1;
    //t_cycle = 4;
    if ((reg8[F] & ZERO_FLAG) == 0x00) {
        pc = read_word(sp);
        sp += 2;
        m_cycle += 2;
    }
	printf("[0x%04X] RET NZ\n",pc-1);
}

// POP BC
void z80::f_0xC1() {
    reg8[C] = read_byte(sp);
    sp++;
    reg8[B] = read_byte(sp);
    sp++;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] POP BC\n",pc-1);
}

// JP NZ,a16
void z80::f_0xC2() {
	printf("[0x%04X] JP NZ, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & ZERO_FLAG) == 0x00) {
        pc = read_word(pc);
        m_cycle++;
    } else pc += 2;
}

// JP a16
void z80::f_0xC3() {
	printf("[0x%04X] JP 0x%04X\n",pc-1,read_word(pc));
    pc = read_word(pc);
    m_cycle = 3;
    //t_cycle = 12;
}

// CALL NZ,a16
void z80::f_0xC4() {
	printf("[0x%04X] CALL NZ, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & ZERO_FLAG) == 0x00) {
        sp -= 2;
        write_word(sp, pc + 2);
        pc = read_word(pc);
        m_cycle += 2;
    } else pc += 2;
}

// PUSH BC
void z80::f_0xC5() {
    sp--;
    write_byte(sp, reg8[B]);
    sp--;
    write_byte(sp, reg8[C]);
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] PUSH BC\n",pc-1);
}

// ADD A,d8
void z80::f_0xC6() {
	printf("[0x%04X] ADD A,0x%02X\n",pc-1,read_byte(pc));
    int a = reg8[A];
    int m = read_byte(pc);
    reg8[A] += m;
    pc++;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ a ^ m) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
}

// RST 00H
void z80::f_0xC7() {
	printf("[0x%04X] RST 00H\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = 0x00;
    m_cycle = 3;
    //t_cycle = 12;
}

// RET Z
void z80::f_0xC8() {
	printf("[0x%04X] RET Z\n",pc-1);
    m_cycle = 1;
    //t_cycle = 4;
    if ((reg8[F] & ZERO_FLAG) == ZERO_FLAG) {
        pc = read_word(sp);
        sp += 2;
        m_cycle += 2;
    }
}

// RET
void z80::f_0xC9() {
	printf("[0x%04X] RET\n",pc-1);
    pc = read_word(sp);
    sp += 2;
    m_cycle = 3;
    //t_cycle = 12;
}

// JP Z,a16
void z80::f_0xCA() {
	printf("[0x%04X] JP Z, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & ZERO_FLAG) == ZERO_FLAG) {
        pc = read_word(pc);
        m_cycle++;
        //t_cycle += 4;
    } else pc += 2;
}

// MAPCb
void z80::f_0xCB() {
    int i = read_byte(pc);
    pc++;
    (this->*cb_opcodes[i])();
    //std::cerr << "CB instruction not implemented: " << i << std::endl;
    //throw "Exception thrown on unimplemented CB instruction";
}

// CALL Z,a16
void z80::f_0xCC() {
	printf("[0x%04X] CALL Z, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & ZERO_FLAG) == ZERO_FLAG) {
        sp -= 2;
        write_word(sp, pc + 2);
        pc = read_word(pc);
        m_cycle += 2;
    } else pc += 2;
}

// CALL a16
void z80::f_0xCD() {
	printf("[0x%04X] CALL 0x%04X\n",pc-1,read_word(pc));
    sp -= 2;
    write_word(sp, pc + 2);
    pc = read_word(pc);
    m_cycle = 5;
    //t_cycle = 20;
}

// ADC A,d8
void z80::f_0xCE() {
	printf("[0x%04X] ADC A, 0x%02X\n",pc-1,read_byte(pc));
    int a = reg8[A];
    int m = read_byte(pc);
    reg8[A] += m;
    pc++;
    reg8[A] += (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] > 0xFF) ? CARRY_FLAG : 0;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ m ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
}

// RST 08H
void z80::f_0xCF() {
	printf("[0x%04X] RST 08H\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = 0x08;
    m_cycle = 3;
    //t_cycle = 12;
}

// RET NC
void z80::f_0xD0() {
	printf("[0x%04X] RET NC\n",pc-1);
    m_cycle = 1;
    //t_cycle = 4;
    if ((reg8[F] & CARRY_FLAG) == 0x00) {
        pc = read_word(sp);
        sp += 2;
        m_cycle += 2;
    }
}

// POP DE
void z80::f_0xD1() {
    reg8[E] = read_byte(sp);
    sp++;
    reg8[D] = read_byte(sp);
    sp++;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] POP DE\n",pc-1);
}

// JP NC,a16
void z80::f_0xD2() {
	printf("[0x%04X] JP NC, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & CARRY_FLAG) == 0x00) {
        pc = read_word(pc);
        m_cycle++;
    } else pc += 2;
}

void z80::f_0xD3() {
	f_UNMAPPED(0xD3);
}

// CALL NC,a16
void z80::f_0xD4() {
	printf("[0x%04X] CALL NC, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & CARRY_FLAG) == 0x00) {
        sp -= 2;
        write_word(sp, pc + 2);
        pc = read_word(pc);
        m_cycle += 2;
    } else pc += 2;
}

// PUSH DE
void z80::f_0xD5() {
    sp--;
    write_byte(sp, reg8[D]);
    sp--;
    write_byte(sp, reg8[E]);
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] PUSH DE\n",pc-1);
}

// SUB d8
void z80::f_0xD6() {
	printf("[0x%04X] SUB 0x%02X\n",pc-1,read_byte(pc));
    int a = reg8[A];
    int m = read_byte(pc);
    reg8[A] -= m;
    pc++;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ m ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
}

// RST 10H
void z80::f_0xD7() {
	printf("[0x%04X] RST 10H\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = CARRY_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
}

// RET C
void z80::f_0xD8() {
	printf("[0x%04X] RET C\n",pc-1);
    m_cycle = 1;
    //t_cycle = 4;
    if ((reg8[F] & CARRY_FLAG) == CARRY_FLAG) {
        pc = read_word(sp);
        sp += 2;
        m_cycle += 2;
    }
}

// RETI
void z80::f_0xD9() {
	printf("[0x%04X] RETI\n",pc-1);
    ime = true;
    //Z80._ops.rrs();
    pc = read_word(sp);
    sp += 2;
    m_cycle = 3;
    //t_cycle = 12;
}

// JP C,a16
void z80::f_0xDA() {
	printf("[0x%04X] JP C, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & CARRY_FLAG) == CARRY_FLAG) {
        pc = read_word(pc);
        m_cycle++;
    } else pc += 2;
}

void z80::f_0xDB() {
	f_UNMAPPED(0xDB);
}

// CALL C,a16
void z80::f_0xDC() {
	printf("[0x%04X] CALL C, 0x%04X\n",pc-1,read_word(pc));
    m_cycle = 3;
    //t_cycle = 12;
    if ((reg8[F] & CARRY_FLAG) == CARRY_FLAG) {
        sp -= 2;
        write_word(sp, pc + 2);
        pc = read_word(pc);
        m_cycle += 2;
    } else pc += 2;
}

void z80::f_0xDD() {
	f_UNMAPPED(0xDD);
}

// SBC A,d8
void z80::f_0xDE() {
	printf("[0x%04X] SBC A, 0x%02X\n",pc-1,read_byte(pc));
    int a = reg8[A];
    int m = read_byte(pc);
    reg8[A] -= m;
    pc++;
    reg8[A] -= (reg8[F] & CARRY_FLAG) ? 1 : 0;
    reg8[F] = (reg8[A] < 0) ? 0x50 : SUBTRACT_FLAG;
    reg8[A] &= 0xFF;
    if (!reg8[A]) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ m ^ a) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
}

// RST 18H
void z80::f_0xDF() {
	printf("[0x%04X] RST 18H\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = 0x18;
    m_cycle = 3;
    //t_cycle = 12;
}

// LDH (a8),A
void z80::f_0xE0() {
	printf("[0x%04X] LDH (0x%02X),A\n",pc-1,read_byte(pc));
    write_byte(0xFF00 + read_byte(pc), reg8[A]);
    pc++;
    m_cycle = 3;
    //t_cycle = 12;
}

// POP HL
void z80::f_0xE1() {
    reg8[L] = read_byte(sp);
    sp++;
    reg8[H] = read_byte(sp);
    sp++;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] POP HL\n",pc-1);
}

// LD (C),A
void z80::f_0xE2() {
    write_byte(0xFF00 + reg8[C], reg8[A]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD (C),A\n",pc-1);
}

void z80::f_0xE3() {
	f_UNMAPPED(0xE3);
}

void z80::f_0xE4() {
	f_UNMAPPED(0xE4);
}

// PUSH HL
void z80::f_0xE5() {
    sp--;
    write_byte(sp, reg8[H]);
    sp--;
    write_byte(sp, reg8[L]);
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] PUSH HL\n",pc-1);
}

// AND d8
void z80::f_0xE6() {
	printf("[0x%04X] AND 0x%02X\n",pc-1,read_byte(pc));
    reg8[A] &= read_byte(pc);
    pc++;
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
}

// RST 20H
void z80::f_0xE7() {
	printf("[0x%04X] RST 20H\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = HALF_CARRY_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
}

// ADD SP,r8
void z80::f_0xE8() {
    int i = read_byte(pc);
    printf("[0x%04X] ADD SP,0x%02X\n",pc-1,i);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    }
    pc++;
    sp += i;
    m_cycle = 4;
    //t_cycle = 16;
}

// JP (HL)
void z80::f_0xE9() {
	printf("[0x%04X] JP (HL)\n",pc-1);
	pc = (reg8[H] << 8) + reg8[L];
    m_cycle = 1;
    //t_cycle = 4;
}

// LD (a16),A
void z80::f_0xEA() {
	printf("[0x%04X] LD (0x%04X),A\n",pc-1,read_word(pc));
    write_byte(read_word(pc), reg8[A]);
    pc += 2;
    m_cycle = 4;
    //t_cycle = 16;
}

void z80::f_0xEB() {
	f_UNMAPPED(0xEB);
}

void z80::f_0xEC() {
	f_UNMAPPED(0xEC);
}

void z80::f_0xED() {
	f_UNMAPPED(0xED);
}

// XOR d8
void z80::f_0xEE() {
	printf("[0x%04X] XOR 0x%02X\n",pc-1,read_byte(pc));
    reg8[A] ^= read_byte(pc);
    pc++;
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
}

// RST28
void z80::f_0xEF() {
	printf("[0x%04X] RST28\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = 0x28;
    m_cycle = 3;
    //t_cycle = 12;
}

// LDH A,(a8)
void z80::f_0xF0() {
	printf("[0x%04X] LDH A,(0x%02X)\n",pc-1,read_byte(pc));
    reg8[A] = read_byte(0xFF00 + read_byte(pc));
    pc++;
    m_cycle = 3;
    //t_cycle = 12;
}

// POP AF
void z80::f_0xF1() {
    reg8[F] = read_byte(sp);
    sp++;
    reg8[A] = read_byte(sp);
    sp++;
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] POP AF\n",pc-1);
}

// LD A,(C)
void z80::f_0xF2() {
    reg8[A] = read_byte(0xFF00 + reg8[C]);
    m_cycle = 2;
    //t_cycle = 8;
	printf("[0x%04X] LD A,(C)\n",pc-1);
}

// DI
void z80::f_0xF3() {
    ime = false;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] DI\n",pc-1);
}

void z80::f_0xF4() {
	f_UNMAPPED(0xF4);
}

// PUSH AF
void z80::f_0xF5() {
    sp--;
    write_byte(sp, reg8[A]);
    sp--;
    write_byte(sp, reg8[F]);
    m_cycle = 3;
    //t_cycle = 12;
	printf("[0x%04X] PUSH AF\n",pc-1);
}

// OR d8
void z80::f_0xF6() {
	printf("[0x%04X] OR 0x%02X\n",pc-1,read_byte(pc));
    reg8[A] |= read_byte(pc);
    pc++;
    reg8[A] &= 0xFF;
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
}

// RST 30H
void z80::f_0xF7() {
	printf("[0x%04X] RST 30H\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = 0x30;
    m_cycle = 3;
    //t_cycle = 12;
}

// LD HL,SP+r8
void z80::f_0xF8() {
    int i = read_byte(pc);
    printf("[0x%04X] LD HL,SP+0x%02X\n",pc-1,i);
    if (i > 127) {
    	i = -((~i + 1) & 0xFF);
    }
    pc++;
    i += sp;
    reg8[H] = (i >> 8) & 0xFF;
    reg8[L] = i & 0xFF;
    m_cycle = 3;
    //t_cycle = 12;
}

void z80::f_0xF9() {
	f_UNMAPPED(0xF9);
}

// LD A,(a16)
void z80::f_0xFA() {
	printf("[0x%04X] LD A,(0x%04X)\n",pc-1,read_word(pc));
    reg8[A] = read_byte(read_word(pc));
    pc += 2;
    m_cycle = 4;
    //t_cycle = 16;
}

// EI
void z80::f_0xFB() {
    ime = true;
    m_cycle = 1;
    //t_cycle = 4;
	printf("[0x%04X] EI\n",pc-1);
}

void z80::f_0xFC() {
	f_UNMAPPED(0xFC);
}

void z80::f_0xFD() {
	f_UNMAPPED(0xFD);
}

// CP d8
void z80::f_0xFE() {
	printf("[0x%04X] CP 0x%02X\n",pc-1,read_byte(pc));
    int i = reg8[A];
    int m = read_byte(pc);
    i -= m;
    pc++;
    reg8[F] = (i < 0) ? 0x50 : SUBTRACT_FLAG;
    i &= 0xFF;
    if (!i) {
    	reg8[F] |= ZERO_FLAG;
    }
    if ((reg8[A] ^ i ^ m) & CARRY_FLAG) {
    	reg8[F] |= HALF_CARRY_FLAG;
    }
    m_cycle = 2;
    //t_cycle = 8;
}

// RST 38H
void z80::f_0xFF() {
	printf("[0x%04X] RST 38H\n",pc-1);
    //Z80._ops.rsv();
    sp -= 2;
    write_word(sp, pc);
    pc = 0x38;
    m_cycle = 3;
    //t_cycle = 12;
}


// CB prefix opcodes

// RLC B
void z80::cb_0x00() {
    int ci = reg8[B] & ZERO_FLAG ? 1 : 0;
    int co = reg8[B] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[B] = (reg8[B] << 1) + ci;
    reg8[B] &= 0xFF;
    reg8[F] = (reg8[B]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RLC B\n",pc-1);
}

// RLC C
void z80::cb_0x01() {
    int ci = reg8[C] & ZERO_FLAG ? 1 : 0;
    int co = reg8[C] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[C] = (reg8[C] << 1) + ci;
    reg8[C] &= 0xFF;
    reg8[F] = (reg8[C]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RLC C\n",pc-1);
}

// RLC D
void z80::cb_0x02() {
    int ci = reg8[D] & ZERO_FLAG ? 1 : 0;
    int co = reg8[D] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[D] = (reg8[D] << 1) + ci;
    reg8[D] &= 0xFF;
    reg8[F] = (reg8[D]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RLC D\n",pc-1);
}

// RLC E
void z80::cb_0x03() {
    int ci = reg8[E] & ZERO_FLAG ? 1 : 0;
    int co = reg8[E] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[E] = (reg8[E] << 1) + ci;
    reg8[E] &= 0xFF;
    reg8[F] = (reg8[E]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RLC E\n",pc-1);
}

// RLC H
void z80::cb_0x04() {
    int ci = reg8[H] & ZERO_FLAG ? 1 : 0;
    int co = reg8[H] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[H] = (reg8[H] << 1) + ci;
    reg8[H] &= 0xFF;
    reg8[F] = (reg8[H]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RLC H\n",pc-1);
}

// RLC L
void z80::cb_0x05() {
    int ci = reg8[L] & ZERO_FLAG ? 1 : 0;
    int co = reg8[L] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[L] = (reg8[L] << 1) + ci;
    reg8[L] &= 0xFF;
    reg8[F] = (reg8[L]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RLC L\n",pc-1);
}

// RLC (HL)
void z80::cb_0x06() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    int ci = i & ZERO_FLAG ? 1 : 0;
    int co = i & ZERO_FLAG ? CARRY_FLAG : 0;
    i = (i << 1) + ci;
    i &= 0xFF;
    reg8[F] = (i) ? 0 : ZERO_FLAG;
    write_byte((reg8[H] << 8) + reg8[L], i);
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 4;
    //t_cycle = 16;
    printf("[0x%04X] RLC HL\n",pc-1);
}

// RLC A
void z80::cb_0x07() {
    int ci = reg8[A] & ZERO_FLAG ? 1 : 0;
    int co = reg8[A] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] << 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[A]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RLC A\n",pc-1);
}

// RRC B
void z80::cb_0x08() {
    int ci = reg8[B] & 1 ? ZERO_FLAG : 0;
    int co = reg8[B] & 1 ? CARRY_FLAG : 0;
    reg8[B] = (reg8[B] >> 1) + ci;
    reg8[B] &= 0xFF;
    reg8[F] = (reg8[B]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RRC B\n",pc-1);
}

// RRC C
void z80::cb_0x09() {
    int ci = reg8[C] & 1 ? ZERO_FLAG : 0;
    int co = reg8[C] & 1 ? CARRY_FLAG : 0;
    reg8[C] = (reg8[C] >> 1) + ci;
    reg8[C] &= 0xFF;
    reg8[F] = (reg8[C]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RRC C\n",pc-1);
}

// RRC D
void z80::cb_0x0A() {
    int ci = reg8[D] & 1 ? ZERO_FLAG : 0;
    int co = reg8[D] & 1 ? CARRY_FLAG : 0;
    reg8[D] = (reg8[D] >> 1) + ci;
    reg8[D] &= 0xFF;
    reg8[F] = (reg8[D]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RRC D\n",pc-1);
}

// RRC E
void z80::cb_0x0B() {
    int ci = reg8[E] & 1 ? ZERO_FLAG : 0;
    int co = reg8[E] & 1 ? CARRY_FLAG : 0;
    reg8[E] = (reg8[E] >> 1) + ci;
    reg8[E] &= 0xFF;
    reg8[F] = (reg8[E]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RRC E\n",pc-1);
}

// RRC H
void z80::cb_0x0C() {
    int ci = reg8[H] & 1 ? ZERO_FLAG : 0;
    int co = reg8[H] & 1 ? CARRY_FLAG : 0;
    reg8[H] = (reg8[H] >> 1) + ci;
    reg8[H] &= 0xFF;
    reg8[F] = (reg8[H]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RRC H\n",pc-1);
}

// RRC L
void z80::cb_0x0D() {
    int ci = reg8[L] & 1 ? ZERO_FLAG : 0;
    int co = reg8[L] & 1 ? CARRY_FLAG : 0;
    reg8[L] = (reg8[L] >> 1) + ci;
    reg8[L] &= 0xFF;
    reg8[F] = (reg8[L]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RRC L\n",pc-1);
}

// RRC (HL)
void z80::cb_0x0E() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    int ci = i & 1 ? ZERO_FLAG : 0;
    int co = i & 1 ? CARRY_FLAG : 0;
    i = (i >> 1) + ci;
    i &= 0xFF;
    write_byte((reg8[H] << 8) + reg8[L], i);
    reg8[F] = (i) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 4;
    //t_cycle = 16;
    printf("[0x%04X] RRC (HL)\n",pc-1);
}

// RRC A
void z80::cb_0x0F() {
    int ci = reg8[A] & 1 ? ZERO_FLAG : 0;
    int co = reg8[A] & 1 ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] >> 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[A]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RRC A\n",pc-1);
}

// RL B
void z80::cb_0x10() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[B] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[B] = (reg8[B] << 1) + ci;
    reg8[B] &= 0xFF;
    reg8[F] = (reg8[B]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RL B\n",pc-1);
}

// RL C
void z80::cb_0x11() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[C] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[C] = (reg8[C] << 1) + ci;
    reg8[C] &= 0xFF;
    reg8[F] = (reg8[C]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RL C\n",pc-1);
}

// RL D
void z80::cb_0x12() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[D] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[D] = (reg8[D] << 1) + ci;
    reg8[D] &= 0xFF;
    reg8[F] = (reg8[D]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RL D\n",pc-1);
}

// RL E
void z80::cb_0x13() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[E] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[E] = (reg8[E] << 1) + ci;
    reg8[E] &= 0xFF;
    reg8[F] = (reg8[E]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RL E\n",pc-1);
}

// RL H
void z80::cb_0x14() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[H] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[H] = (reg8[H] << 1) + ci;
    reg8[H] &= 0xFF;
    reg8[F] = (reg8[H]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RL H\n",pc-1);
}

// RL L
void z80::cb_0x15() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[L] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[L] = (reg8[L] << 1) + ci;
    reg8[L] &= 0xFF;
    reg8[F] = (reg8[L]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RL L\n",pc-1);
}

// RL (HL)
void z80::cb_0x16() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = i & ZERO_FLAG ? CARRY_FLAG : 0;
    i = (i << 1) + ci;
    i &= 0xFF;
    reg8[F] = (i) ? 0 : ZERO_FLAG;
    write_byte((reg8[H] << 8) + reg8[L], i);
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 4;
    //t_cycle = 16;
    printf("[0x%04X] RL (HL)\n",pc-1);
}

// RL A
void z80::cb_0x17() {
    int ci = reg8[F] & CARRY_FLAG ? 1 : 0;
    int co = reg8[A] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] << 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[A]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RL A\n",pc-1);
}

// RR B
void z80::cb_0x18() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[B] & 1 ? CARRY_FLAG : 0;
    reg8[B] = (reg8[B] >> 1) + ci;
    reg8[B] &= 0xFF;
    reg8[F] = (reg8[B]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RR B\n",pc-1);
}

// RR C
void z80::cb_0x19() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[C] & 1 ? CARRY_FLAG : 0;
    reg8[C] = (reg8[C] >> 1) + ci;
    reg8[C] &= 0xFF;
    reg8[F] = (reg8[C]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RR C\n",pc-1);
}

// RR D
void z80::cb_0x1A() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[D] & 1 ? CARRY_FLAG : 0;
    reg8[D] = (reg8[D] >> 1) + ci;
    reg8[D] &= 0xFF;
    reg8[F] = (reg8[D]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RR D\n",pc-1);
}

// RR E
void z80::cb_0x1B() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[E] & 1 ? CARRY_FLAG : 0;
    reg8[E] = (reg8[E] >> 1) + ci;
    reg8[E] &= 0xFF;
    reg8[F] = (reg8[E]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RR E\n",pc-1);
}

// RR H
void z80::cb_0x1C() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[H] & 1 ? CARRY_FLAG : 0;
    reg8[H] = (reg8[H] >> 1) + ci;
    reg8[H] &= 0xFF;
    reg8[F] = (reg8[H]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RR H\n",pc-1);
}

// RR L
void z80::cb_0x1D() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[L] & 1 ? CARRY_FLAG : 0;
    reg8[L] = (reg8[L] >> 1) + ci;
    reg8[L] &= 0xFF;
    reg8[F] = (reg8[L]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RR L\n",pc-1);
}

// RR (HL)
void z80::cb_0x1E() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = i & 1 ? CARRY_FLAG : 0;
    i = (i >> 1) + ci;
    i &= 0xFF;
    write_byte((reg8[H] << 8) + reg8[L], i);
    reg8[F] = (i) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 4;
    //t_cycle = 16;
    printf("[0x%04X] RR (HL)\n",pc-1);
}

// RR A
void z80::cb_0x1F() {
    int ci = reg8[F] & CARRY_FLAG ? ZERO_FLAG : 0;
    int co = reg8[A] & 1 ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] >> 1) + ci;
    reg8[A] &= 0xFF;
    reg8[F] = (reg8[A]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] RR A\n",pc-1);
}

// SLA B
void z80::cb_0x20() {
    int co = reg8[B] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[B] = (reg8[B] << 1) & 0xFF;
    reg8[F] = (reg8[B]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SLA B\n",pc-1);
}

// SLA C
void z80::cb_0x21() {
    int co = reg8[C] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[C] = (reg8[C] << 1) & 0xFF;
    reg8[F] = (reg8[C]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SLA C\n",pc-1);
}

// SLA D
void z80::cb_0x22() {
    int co = reg8[D] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[D] = (reg8[D] << 1) & 0xFF;
    reg8[F] = (reg8[D]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SLA D\n",pc-1);
}

// SLA E
void z80::cb_0x23() {
    int co = reg8[E] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[E] = (reg8[E] << 1) & 0xFF;
    reg8[F] = (reg8[E]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SLA E\n",pc-1);
}

// SLA H
void z80::cb_0x24() {
    int co = reg8[H] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[H] = (reg8[H] << 1) & 0xFF;
    reg8[F] = (reg8[H]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SLA H\n",pc-1);
}

// SLA L
void z80::cb_0x25() {
    int co = reg8[L] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[L] = (reg8[L] << 1) & 0xFF;
    reg8[F] = (reg8[L]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SLA L\n",pc-1);
}

void z80::cb_0x26() {
	f_UNMAPPED(0x26);
}

// SLA A
void z80::cb_0x27() {
    int co = reg8[A] & ZERO_FLAG ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] << 1) & 0xFF;
    reg8[F] = (reg8[A]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SLA A\n",pc-1);
}

// SRA B
void z80::cb_0x28() {
    int ci = reg8[B] & ZERO_FLAG;
    int co = reg8[B] & 1 ? CARRY_FLAG : 0;
    reg8[B] = ((reg8[B] >> 1) + ci) & 0xFF;
    reg8[F] = (reg8[B]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRA B\n",pc-1);
}

// SRA C
void z80::cb_0x29() {
    int ci = reg8[C] & ZERO_FLAG;
    int co = reg8[C] & 1 ? CARRY_FLAG : 0;
    reg8[C] = ((reg8[C] >> 1) + ci) & 0xFF;
    reg8[F] = (reg8[C]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRA C\n",pc-1);
}

// SRA D
void z80::cb_0x2A() {
    int ci = reg8[D] & ZERO_FLAG;
    int co = reg8[D] & 1 ? CARRY_FLAG : 0;
    reg8[D] = ((reg8[D] >> 1) + ci) & 0xFF;
    reg8[F] = (reg8[D]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRA D\n",pc-1);
}

// SRA E
void z80::cb_0x2B() {
    int ci = reg8[E] & ZERO_FLAG;
    int co = reg8[E] & 1 ? CARRY_FLAG : 0;
    reg8[E] = ((reg8[E] >> 1) + ci) & 0xFF;
    reg8[F] = (reg8[E]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRA E\n",pc-1);
}

// SRA H
void z80::cb_0x2C() {
    int ci = reg8[H] & ZERO_FLAG;
    int co = reg8[H] & 1 ? CARRY_FLAG : 0;
    reg8[H] = ((reg8[H] >> 1) + ci) & 0xFF;
    reg8[F] = (reg8[H]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRA H\n",pc-1);
}

// SRA L
void z80::cb_0x2D() {
    int ci = reg8[L] & ZERO_FLAG;
    int co = reg8[L] & 1 ? CARRY_FLAG : 0;
    reg8[L] = ((reg8[L] >> 1) + ci) & 0xFF;
    reg8[F] = (reg8[L]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRA L\n",pc-1);
}

void z80::cb_0x2E() {
	f_UNMAPPED(0x2E);
}

// SRA A
void z80::cb_0x2F() {
    int ci = reg8[A] & ZERO_FLAG;
    int co = reg8[A] & 1 ? CARRY_FLAG : 0;
    reg8[A] = ((reg8[A] >> 1) + ci) & 0xFF;
    reg8[F] = (reg8[A]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRA A\n",pc-1);
}

// SWAP B
void z80::cb_0x30() {
    int tr = reg8[B];
    reg8[B] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
    reg8[F] = reg8[B] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 16;
    printf("[0x%04X] SWAP B\n",pc-1);
}

// SWAP C
void z80::cb_0x31() {
    int tr = reg8[C];
    reg8[C] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
    reg8[F] = reg8[C] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 16;
    printf("[0x%04X] SWAP C\n",pc-1);
}

// SWAP D
void z80::cb_0x32() {
    int tr = reg8[D];
    reg8[D] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
    reg8[F] = reg8[D] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 16;
    printf("[0x%04X] SWAP D\n",pc-1);
}

// SWAP E
void z80::cb_0x33() {
    int tr = reg8[E];
    reg8[E] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
    reg8[F] = reg8[E] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 16;
    printf("[0x%04X] SWAP E\n",pc-1);
}

// SWAP H
void z80::cb_0x34() {
    int tr = reg8[H];
    reg8[H] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
    reg8[F] = reg8[H] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 16;
    printf("[0x%04X] SWAP H\n",pc-1);
}

// SWAP L
void z80::cb_0x35() {
    int tr = reg8[L];
    reg8[L] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
    reg8[F] = reg8[L] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 16;
}

void z80::cb_0x36() {
	f_UNMAPPED(0x36);
}

// SWAP A
void z80::cb_0x37() {
    int tr = reg8[A];
    reg8[A] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
    reg8[F] = reg8[A] ? 0 : ZERO_FLAG;
    m_cycle = 1;
    //t_cycle = 16;
    printf("[0x%04X] SWAP A\n",pc-1);
}

// SRL B
void z80::cb_0x38() {
    int co = reg8[B] & 1 ? CARRY_FLAG : 0;
    reg8[B] = (reg8[B] >> 1) & 0xFF;
    reg8[F] = (reg8[B]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRL B\n",pc-1);
}

// SRL C
void z80::cb_0x39() {
    int co = reg8[C] & 1 ? CARRY_FLAG : 0;
    reg8[C] = (reg8[C] >> 1) & 0xFF;
    reg8[F] = (reg8[C]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
     //t_cycle = 8;
    printf("[0x%04X] SRL C\n",pc-1);
}

// SRL D
void z80::cb_0x3A() {
    int co = reg8[D] & 1 ? CARRY_FLAG : 0;
    reg8[D] = (reg8[D] >> 1) & 0xFF;
    reg8[F] = (reg8[D]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRL D\n",pc-1);
}

// SRL E
void z80::cb_0x3B() {
    int co = reg8[E] & 1 ? CARRY_FLAG : 0;
    reg8[E] = (reg8[E] >> 1) & 0xFF;
    reg8[F] = (reg8[E]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRL E\n",pc-1);
}

// SRL H
void z80::cb_0x3C() {
    int co = reg8[H] & 1 ? CARRY_FLAG : 0;
    reg8[H] = (reg8[H] >> 1) & 0xFF;
    reg8[F] = (reg8[H]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRL H\n",pc-1);
}

// SRL L
void z80::cb_0x3D() {
    int co = reg8[L] & 1 ? CARRY_FLAG : 0;
    reg8[L] = (reg8[L] >> 1) & 0xFF;
    reg8[F] = (reg8[L]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRL L\n",pc-1);
}

void z80::cb_0x3E() {
	f_UNMAPPED(0x3E);
}

// SRL A
void z80::cb_0x3F() {
    int co = reg8[A] & 1 ? CARRY_FLAG : 0;
    reg8[A] = (reg8[A] >> 1) & 0xFF;
    reg8[F] = (reg8[A]) ? 0 : ZERO_FLAG;
    reg8[F] = (reg8[F] & 0xEF) + co;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] SRL A\n",pc-1);
}

// BIT 0,B
void z80::cb_0x40() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 0,B\n",pc-1);
}

// BIT 0,C
void z80::cb_0x41() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 0,C\n",pc-1);
}

// BIT 0,D
void z80::cb_0x42() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 0,D\n",pc-1);
}

// BIT 0,E
void z80::cb_0x43() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 0,E\n",pc-1);
}

// BIT 0,H
void z80::cb_0x44() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 0,H\n",pc-1);
}

// BIT 0,L
void z80::cb_0x45() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 0,L\n",pc-1);
}

// BIT 0,(HL)
void z80::cb_0x46() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 0,(HL)\n",pc-1);
}

// BIT 0,A
void z80::cb_0x47() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & 0x01) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 0,A\n",pc-1);
}

// BIT 1,B
void z80::cb_0x48() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 1,B\n",pc-1);
}

// BIT 1,C
void z80::cb_0x49() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 1,C\n",pc-1);
}

// BIT 1,D
void z80::cb_0x4A() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 1,D\n",pc-1);
}

// BIT 1,E
void z80::cb_0x4B() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 1,E\n",pc-1);
}

// BIT 1,H
void z80::cb_0x4C() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 1,H\n",pc-1);
}

// BIT 1,L
void z80::cb_0x4D() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 1,L\n",pc-1);
}

// BIT 1,(HL)
void z80::cb_0x4E() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 1,(HL)\n",pc-1);
}

// BIT 1,A
void z80::cb_0x4F() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & 0x02) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 1,A\n",pc-1);
}

// BIT 2,B
void z80::cb_0x50() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 2,B\n",pc-1);
}

// BIT 2,C
void z80::cb_0x51() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 2,C\n",pc-1);
}

// BIT 2,D
void z80::cb_0x52() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 2,D\n",pc-1);
}

// BIT 2,E
void z80::cb_0x53() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 2,E\n",pc-1);
}

// BIT 2,H
void z80::cb_0x54() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 2,H\n",pc-1);
}

// BIT 2,L
void z80::cb_0x55() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 2,L\n",pc-1);
}

// BIT 2,(HL)
void z80::cb_0x56() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 2,(HL)\n",pc-1);
}

// BIT 2,A
void z80::cb_0x57() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & 0x04) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 2,A\n",pc-1);
}

// BIT 3,B
void z80::cb_0x58() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 3,B\n",pc-1);
}

// BIT 3,C
void z80::cb_0x59() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 3,C\n",pc-1);
}

// BIT 3,D
void z80::cb_0x5A() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 3,D\n",pc-1);
}

// BIT 3,E
void z80::cb_0x5B() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 3,E\n",pc-1);
}

// BIT 3,H
void z80::cb_0x5C() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 3,H\n",pc-1);
}

// BIT 3,L
void z80::cb_0x5D() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 3,L\n",pc-1);
}

// BIT 3,(HL)
void z80::cb_0x5E() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 3,(HL)\n",pc-1);
}

// BIT 3,A
void z80::cb_0x5F() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & 0x08) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 3,A\n",pc-1);
}

// BIT 4,B
void z80::cb_0x60() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 4,B\n",pc-1);
}

// BIT 4,C
void z80::cb_0x61() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 4,C\n",pc-1);
}

// BIT 4,D
void z80::cb_0x62() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 4,D\n",pc-1);
}

// BIT 4,E
void z80::cb_0x63() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 4,E\n",pc-1);
}

// BIT 4,H
void z80::cb_0x64() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 4,H\n",pc-1);
}

// BIT 4,L
void z80::cb_0x65() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 4,L\n",pc-1);
}

// BIT 4,(HL)
void z80::cb_0x66() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 4,(HL)\n",pc-1);
}

// BIT 4,A
void z80::cb_0x67() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 4,A\n",pc-1);
}

// BIT 5,B
void z80::cb_0x68() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 5,B\n",pc-1);
}

// BIT 5,C
void z80::cb_0x69() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 5,C\n",pc-1);
}

// BIT 5,D
void z80::cb_0x6A() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 5,D\n",pc-1);
}

// BIT 5,E
void z80::cb_0x6B() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 5,E\n",pc-1);
}

// BIT 5,H
void z80::cb_0x6C() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 5,H\n",pc-1);
}

// BIT 5,L
void z80::cb_0x6D() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 5,L\n",pc-1);
}

// BIT 5,(HL)
void z80::cb_0x6E() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 5,(HL)\n",pc-1);
}

// BIT 5,A
void z80::cb_0x6F() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & HALF_CARRY_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 5,A\n",pc-1);
}

// BIT 6,B
void z80::cb_0x70() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 6,B\n",pc-1);
}

// BIT 6,C
void z80::cb_0x71() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 6,C\n",pc-1);
}

// BIT 6,D
void z80::cb_0x72() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 6,D\n",pc-1);
}

// BIT 6,E
void z80::cb_0x73() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 6,E\n",pc-1);
}

// BIT 6,H
void z80::cb_0x74() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 6,H\n",pc-1);
}

// BIT 6,L
void z80::cb_0x75() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 6,LB\n",pc-1);
}

// BIT 6,(HL)
void z80::cb_0x76() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 6,(HL)\n",pc-1);
}

// BIT 6,A
void z80::cb_0x77() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & SUBTRACT_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 6,A\n",pc-1);
}

// BIT 7,B
void z80::cb_0x78() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[B] & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 7,B\n",pc-1);
}

// BIT 7,C
void z80::cb_0x79() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[C] & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 7,C\n",pc-1);
}

// BIT 7,D
void z80::cb_0x7A() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[D] & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 7,D\n",pc-1);
}

// BIT 7,E
void z80::cb_0x7B() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[E] & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 7,E\n",pc-1);
}

// BIT 7,H
void z80::cb_0x7C() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[H] & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 7,H\n",pc-1);
}

// BIT 7,L
void z80::cb_0x7D() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[L] & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 7,L\n",pc-1);
}

// BIT 7,(HL)
void z80::cb_0x7E() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (read_byte((reg8[H] << 8) + reg8[L]) & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 3;
    //t_cycle = 12;
    printf("[0x%04X] BIT 7,(HL)\n",pc-1);
}

// BIT 7,A
void z80::cb_0x7F() {
    reg8[F] &= 0x1F;
    reg8[F] |= HALF_CARRY_FLAG;
    reg8[F] = (reg8[A] & ZERO_FLAG) ? 0 : ZERO_FLAG;
    m_cycle = 2;
    //t_cycle = 8;
    printf("[0x%04X] BIT 7,A\n",pc-1);
}

// RES 0,B
void z80::cb_0x80() {
    reg8[B] &= 0xFE;
    m_cycle = 2;
    printf("[0x%04X] RES 0,B\n",pc-1);
}

// RES 0,C
void z80::cb_0x81() {
    reg8[C] &= 0xFE;
    m_cycle = 2;
    printf("[0x%04X] RES 0,C\n",pc-1);
}

// RES 0,D
void z80::cb_0x82() {
    reg8[D] &= 0xFE;
    m_cycle = 2;
    printf("[0x%04X] RES 0,D\n",pc-1);
}

// RES 0,E
void z80::cb_0x83() {
    reg8[E] &= 0xFE;
    m_cycle = 2;
    printf("[0x%04X] RES 0,E\n",pc-1);
}

// RES 0,H
void z80::cb_0x84() {
    reg8[H] &= 0xFE;
    m_cycle = 2;
    printf("[0x%04X] RES 0,H\n",pc-1);
}

// RES 0,L
void z80::cb_0x85() {
    reg8[L] &= 0xFE;
    m_cycle = 2;
    printf("[0x%04X] RES 0,L\n",pc-1);
}

// RES 0,(HL)
void z80::cb_0x86() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0xFE;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 0,(HL)\n",pc-1);
}

// RES 0,A
void z80::cb_0x87() {
    reg8[A] &= 0xFE;
    m_cycle = 2;
    printf("[0x%04X] RES 0,A\n",pc-1);
}

// RES 1,B
void z80::cb_0x88() {
    reg8[B] &= 0xFD;
    m_cycle = 2;
    printf("[0x%04X] RES 1,B\n",pc-1);
}

// RES 1,C
void z80::cb_0x89() {
    reg8[C] &= 0xFD;
    m_cycle = 2;
    printf("[0x%04X] RES 1,C\n",pc-1);
}

// RES 1,D
void z80::cb_0x8A() {
    reg8[D] &= 0xFD;
    m_cycle = 2;
    printf("[0x%04X] RES 1,D\n",pc-1);
}

// RES 1,E
void z80::cb_0x8B() {
    reg8[E] &= 0xFD;
    m_cycle = 2;
    printf("[0x%04X] RES 1,E\n",pc-1);
}

// RES 1,H
void z80::cb_0x8C() {
    reg8[H] &= 0xFD;
    m_cycle = 2;
    printf("[0x%04X] RES 1,H\n",pc-1);
}

// RES 1,L
void z80::cb_0x8D() {
    reg8[L] &= 0xFD;
    m_cycle = 2;
    printf("[0x%04X] RES 1,L\n",pc-1);
}

// RES 1,(HL)
void z80::cb_0x8E() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0xFD;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 1,(HL)\n",pc-1);
}

// RES 1,A
void z80::cb_0x8F() {
    reg8[A] &= 0xFD;
    m_cycle = 2;
    printf("[0x%04X] RES 1,A\n",pc-1);
}

// RES 2,B
void z80::cb_0x90() {
    reg8[B] &= 0xFB;
    m_cycle = 2;
    printf("[0x%04X] RES 2,B\n",pc-1);
}

// RES 2,C
void z80::cb_0x91() {
    reg8[C] &= 0xFB;
    m_cycle = 2;
    printf("[0x%04X] RES 2,C\n",pc-1);
}

// RES 2,D
void z80::cb_0x92() {
    reg8[D] &= 0xFB;
    m_cycle = 2;
    printf("[0x%04X] RES 2,D\n",pc-1);
}

// RES 2,E
void z80::cb_0x93() {
    reg8[E] &= 0xFB;
    m_cycle = 2;
    printf("[0x%04X] RES 2,E\n",pc-1);
}

// RES 2,H
void z80::cb_0x94() {
    reg8[H] &= 0xFB;
    m_cycle = 2;
    printf("[0x%04X] RES 2,H\n",pc-1);
}

// RES 2,L
void z80::cb_0x95() {
    reg8[L] &= 0xFB;
    m_cycle = 2;
    printf("[0x%04X] RES 2,L\n",pc-1);
}

// RES 2,(HL)
void z80::cb_0x96() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0xFB;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 2,(HL)\n",pc-1);
}

// RES 2,A
void z80::cb_0x97() {
    reg8[A] &= 0xFB;
    m_cycle = 2;
    printf("[0x%04X] RES 2,A\n",pc-1);
}

// RES 3,B
void z80::cb_0x98() {
    reg8[B] &= 0xF7;
    m_cycle = 2;
    printf("[0x%04X] RES 3,B\n",pc-1);
}

// RES 3,C
void z80::cb_0x99() {
    reg8[C] &= 0xF7;
    m_cycle = 2;
    printf("[0x%04X] RES 3,C\n",pc-1);
}

// RES 3,D
void z80::cb_0x9A() {
    reg8[D] &= 0xF7;
    m_cycle = 2;
    printf("[0x%04X] RES 3,D\n",pc-1);
}

// RES 3,E
void z80::cb_0x9B() {
    reg8[E] &= 0xF7;
    m_cycle = 2;
    printf("[0x%04X] RES 3,E\n",pc-1);
}

// RES 3,H
void z80::cb_0x9C() {
    reg8[H] &= 0xF7;
    m_cycle = 2;
    printf("[0x%04X] RES 3,H\n",pc-1);
}

// RES 3,L
void z80::cb_0x9D() {
    reg8[L] &= 0xF7;
    m_cycle = 2;
    printf("[0x%04X] RES 3,L\n",pc-1);
}

// RES 3,(HL)
void z80::cb_0x9E() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0xF7;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 3,(HL)\n",pc-1);
}

// RES 3,A
void z80::cb_0x9F() {
    reg8[A] &= 0xF7;
    m_cycle = 2;
    printf("[0x%04X] RES 3,A\n",pc-1);
}

// RES 4,B
void z80::cb_0xA0() {
    reg8[B] &= 0xEF;
    m_cycle = 2;
    printf("[0x%04X] RES 4,B\n",pc-1);
}

// RES 4,C
void z80::cb_0xA1() {
    reg8[C] &= 0xEF;
    m_cycle = 2;
    printf("[0x%04X] RES 4,C\n",pc-1);
}

// RES 4,D
void z80::cb_0xA2() {
    reg8[D] &= 0xEF;
    m_cycle = 2;
    printf("[0x%04X] RES 4,D\n",pc-1);
}

// RES 4,E
void z80::cb_0xA3() {
    reg8[E] &= 0xEF;
    m_cycle = 2;
    printf("[0x%04X] RES 4,E\n",pc-1);
}

// RES 4,H
void z80::cb_0xA4() {
    reg8[H] &= 0xEF;
    m_cycle = 2;
    printf("[0x%04X] RES 4,H\n",pc-1);
}

// RES 4,L
void z80::cb_0xA5() {
    reg8[L] &= 0xEF;
    m_cycle = 2;
    printf("[0x%04X] RES 4,L\n",pc-1);
}

// RES 4,(HL)
void z80::cb_0xA6() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0xEF;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 4,(HL)\n",pc-1);
}

// RES 4,A
void z80::cb_0xA7() {
    reg8[A] &= 0xEF;
    m_cycle = 2;
    printf("[0x%04X] RES 4,A\n",pc-1);
}

// RES 5,B
void z80::cb_0xA8() {
    reg8[B] &= 0xDF;
    m_cycle = 2;
    printf("[0x%04X] RES 5,B\n",pc-1);
}

// RES 5,C
void z80::cb_0xA9() {
    reg8[C] &= 0xDF;
    m_cycle = 2;
    printf("[0x%04X] RES 5,C\n",pc-1);
}

// RES 5,D
void z80::cb_0xAA() {
    reg8[D] &= 0xDF;
    m_cycle = 2;
    printf("[0x%04X] RES 5,D\n",pc-1);
}

// RES 5,E
void z80::cb_0xAB() {
    reg8[E] &= 0xDF;
    m_cycle = 2;
    printf("[0x%04X] RES 5,E\n",pc-1);
}

// RES 5,H
void z80::cb_0xAC() {
    reg8[H] &= 0xDF;
    m_cycle = 2;
    printf("[0x%04X] RES 5,H\n",pc-1);
}

// RES 5,L
void z80::cb_0xAD() {
    reg8[L] &= 0xDF;
    m_cycle = 2;
    printf("[0x%04X] RES 5,L\n",pc-1);
}

// RES 5,(HL)
void z80::cb_0xAE() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0xDF;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 5,(HL)\n",pc-1);
}

// RES 5,A
void z80::cb_0xAF() {
    reg8[A] &= 0xDF;
    m_cycle = 2;
    printf("[0x%04X] RES 5,A\n",pc-1);
}

// RES 6,B
void z80::cb_0xB0() {
    reg8[B] &= 0xBF;
    m_cycle = 2;
    printf("[0x%04X] RES 6,B\n",pc-1);
}

// RES 6,C
void z80::cb_0xB1() {
    reg8[C] &= 0xBF;
    m_cycle = 2;
    printf("[0x%04X] RES 6,C\n",pc-1);
}

// RES 6,D
void z80::cb_0xB2() {
    reg8[D] &= 0xBF;
    m_cycle = 2;
    printf("[0x%04X] RES 6,D\n",pc-1);
}

// RES 6,E
void z80::cb_0xB3() {
    reg8[E] &= 0xBF;
    m_cycle = 2;
    printf("[0x%04X] RES 6,E\n",pc-1);
}

// RES 6,H
void z80::cb_0xB4() {
    reg8[H] &= 0xBF;
    m_cycle = 2;
    printf("[0x%04X] RES 6,H\n",pc-1);
}

// RES 6,L
void z80::cb_0xB5() {
    reg8[L] &= 0xBF;
    m_cycle = 2;
    printf("[0x%04X] RES 6,L\n",pc-1);
}

// RES 6,(HL)
void z80::cb_0xB6() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0xBF;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 6,(HL)\n",pc-1);
}

// RES 6,A
void z80::cb_0xB7() {
    reg8[A] &= 0xBF;
    m_cycle = 2;
    printf("[0x%04X] RES 6,A\n",pc-1);
}

// RES 7,B
void z80::cb_0xB8() {
    reg8[B] &= 0x7F;
    m_cycle = 2;
    printf("[0x%04X] RES 7,B\n",pc-1);
}

// RES 7,C
void z80::cb_0xB9() {
    reg8[C] &= 0x7F;
    m_cycle = 2;
    printf("[0x%04X] RES 7,C\n",pc-1);
}

// RES 7,D
void z80::cb_0xBA() {
    reg8[D] &= 0x7F;
    m_cycle = 2;
    printf("[0x%04X] RES 7,D\n",pc-1);
}

// RES 7,E
void z80::cb_0xBB() {
    reg8[E] &= 0x7F;
    m_cycle = 2;
    printf("[0x%04X] RES 7,E\n",pc-1);
}

// RES 7,H
void z80::cb_0xBC() {
    reg8[H] &= 0x7F;
    m_cycle = 2;
    printf("[0x%04X] RES 7,H\n",pc-1);
}

// RES 7,L
void z80::cb_0xBD() {
    reg8[L] &= 0x7F;
    m_cycle = 2;
    printf("[0x%04X] RES 7,L\n",pc-1);
}

// RES 7,(HL)
void z80::cb_0xBE() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i &= 0x7F;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] RES 7,(HL)\n",pc-1);
}

// RES 7,A
void z80::cb_0xBF() {
    reg8[A] &= 0x7F;
    m_cycle = 2;
    printf("[0x%04X] RES 7,A\n",pc-1);
}

// SET 0,B
void z80::cb_0xC0() {
    reg8[B] |= 0x01;
    m_cycle = 2;
    printf("[0x%04X] SET 0,B\n",pc-1);
}

// SET 0,C
void z80::cb_0xC1() {
    reg8[B] |= 0x01;
    m_cycle = 2;
    printf("[0x%04X] SET 0,C\n",pc-1);
}

// SET 0,D
void z80::cb_0xC2() {
    reg8[B] |= 0x01;
    m_cycle = 2;
    printf("[0x%04X] SET 0,D\n",pc-1);
}

// SET 0,E
void z80::cb_0xC3() {
    reg8[B] |= 0x01;
    m_cycle = 2;
    printf("[0x%04X] SET 0,E\n",pc-1);
}

// SET 0,H
void z80::cb_0xC4() {
    reg8[B] |= 0x01;
    m_cycle = 2;
    printf("[0x%04X] SET 0,H\n",pc-1);
}

// SET 0,L
void z80::cb_0xC5() {
    reg8[B] |= 0x01;
    m_cycle = 2;
    printf("[0x%04X] SET 0,L\n",pc-1);
}

// SET 0,(HL)
void z80::cb_0xC6() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= 0x01;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 0,(HL)\n",pc-1);
}

// SET 0,A
void z80::cb_0xC7() {
    reg8[B] |= 0x01;
    m_cycle = 2;
    printf("[0x%04X] SET 0,A\n",pc-1);
}

// SET 1,B
void z80::cb_0xC8() {
    reg8[B] |= 0x02;
    m_cycle = 2;
    printf("[0x%04X] SET 1,B\n",pc-1);
}

// SET 1,C
void z80::cb_0xC9() {
    reg8[B] |= 0x02;
    m_cycle = 2;
    printf("[0x%04X] SET 1,C\n",pc-1);
}

// SET 1,D
void z80::cb_0xCA() {
    reg8[B] |= 0x02;
    m_cycle = 2;
    printf("[0x%04X] SET 1,D\n",pc-1);
}

// SET 1,E
void z80::cb_0xCB() {
    reg8[B] |= 0x02;
    m_cycle = 2;
    printf("[0x%04X] SET 1,E\n",pc-1);
}

// SET 1,H
void z80::cb_0xCC() {
    reg8[B] |= 0x02;
    m_cycle = 2;
    printf("[0x%04X] SET 1,H\n",pc-1);
}

// SET 1,L
void z80::cb_0xCD() {
    reg8[B] |= 0x02;
    m_cycle = 2;
    printf("[0x%04X] SET 1,L\n",pc-1);
}

// SET 1,(HL)
void z80::cb_0xCE() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= 0x02;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 1,(HL)\n",pc-1);
}

// SET 1,A
void z80::cb_0xCF() {
    reg8[B] |= 0x02;
    m_cycle = 2;
    printf("[0x%04X] SET 1,A\n",pc-1);
}

// SET 2,B
void z80::cb_0xD0() {
    reg8[B] |= 0x04;
    m_cycle = 2;
    printf("[0x%04X] SET 2,B\n",pc-1);
}

// SET 2,C
void z80::cb_0xD1() {
    reg8[B] |= 0x04;
    m_cycle = 2;
    printf("[0x%04X] SET 2,C\n",pc-1);
}

// SET 2,D
void z80::cb_0xD2() {
    reg8[B] |= 0x04;
    m_cycle = 2;
    printf("[0x%04X] SET 2,D\n",pc-1);
}

// SET 2,E
void z80::cb_0xD3() {
    reg8[B] |= 0x04;
    m_cycle = 2;
    printf("[0x%04X] SET 2,E\n",pc-1);
}

// SET 2,H
void z80::cb_0xD4() {
    reg8[B] |= 0x04;
    m_cycle = 2;
    printf("[0x%04X] SET 2,H\n",pc-1);
}

// SET 2,L
void z80::cb_0xD5() {
    reg8[B] |= 0x04;
    m_cycle = 2;
    printf("[0x%04X] SET 2,L\n",pc-1);
}

// SET 2,(HL)
void z80::cb_0xD6() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= 0x04;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 2,(HL)\n",pc-1);
}

// SET 2,A
void z80::cb_0xD7() {
    reg8[B] |= 0x04;
    m_cycle = 2;
    printf("[0x%04X] SET 2,A\n",pc-1);
}

// SET 3,B
void z80::cb_0xD8() {
    reg8[B] |= 0x08;
    m_cycle = 2;
    printf("[0x%04X] SET 3,B\n",pc-1);
}

// SET 3,C
void z80::cb_0xD9() {
    reg8[B] |= 0x08;
    m_cycle = 2;
    printf("[0x%04X] SET 3,C\n",pc-1);
}

// SET 3,D
void z80::cb_0xDA() {
    reg8[B] |= 0x08;
    m_cycle = 2;
    printf("[0x%04X] SET 3,D\n",pc-1);
}

// SET 3,E
void z80::cb_0xDB() {
    reg8[B] |= 0x08;
    m_cycle = 2;
    printf("[0x%04X] SET 3,E\n",pc-1);
}

// SET 3,H
void z80::cb_0xDC() {
    reg8[B] |= 0x08;
    m_cycle = 2;
    printf("[0x%04X] SET 3,H\n",pc-1);
}

// SET 3,L
void z80::cb_0xDD() {
    reg8[B] |= 0x08;
    m_cycle = 2;
    printf("[0x%04X] SET 3,L\n",pc-1);
}

// SET 3,(HL)
void z80::cb_0xDE() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= 0x08;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 3,(HL)\n",pc-1);
}

// SET 3,A
void z80::cb_0xDF() {
    reg8[B] |= 0x08;
    m_cycle = 2;
    printf("[0x%04X] SET 3,A\n",pc-1);
}

// SET 4,B
void z80::cb_0xE0() {
    reg8[B] |= CARRY_FLAG;
     m_cycle = 2;
     printf("[0x%04X] SET 4,B\n",pc-1);
}

// SET 4,C
void z80::cb_0xE1() {
    reg8[B] |= CARRY_FLAG;
     m_cycle = 2;
     printf("[0x%04X] SET 4,C\n",pc-1);
}

// SET 4,D
void z80::cb_0xE2() {
    reg8[B] |= CARRY_FLAG;
     m_cycle = 2;
     printf("[0x%04X] SET 4,D\n",pc-1);
}

// SET 4,E
void z80::cb_0xE3() {
    reg8[B] |= CARRY_FLAG;
     m_cycle = 2;
     printf("[0x%04X] SET 4,E\n",pc-1);
}

// SET 4,H
void z80::cb_0xE4() {
    reg8[B] |= CARRY_FLAG;
     m_cycle = 2;
     printf("[0x%04X] SET 4,H\n",pc-1);
}

// SET 4,L
void z80::cb_0xE5() {
    reg8[B] |= CARRY_FLAG;
     m_cycle = 2;
     printf("[0x%04X] SET 4,L\n",pc-1);
}

// SET 4,(HL)
void z80::cb_0xE6() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= CARRY_FLAG;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 5,(HL)\n",pc-1);
}

// SET 4,A
void z80::cb_0xE7() {
    reg8[B] |= CARRY_FLAG;
	m_cycle = 2;
    printf("[0x%04X] SET 4,A\n",pc-1);
}

// SET 5,B
void z80::cb_0xE8() {
    reg8[B] |= HALF_CARRY_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 5,B\n",pc-1);
}

// SET 5,C
void z80::cb_0xE9() {
    reg8[B] |= HALF_CARRY_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 5,C\n",pc-1);
}

// SET 5,D
void z80::cb_0xEA() {
    reg8[B] |= HALF_CARRY_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 5,D\n",pc-1);
}

// SET 5,E
void z80::cb_0xEB() {
    reg8[B] |= HALF_CARRY_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 5,E\n",pc-1);
}

// SET 5,H
void z80::cb_0xEC() {
    reg8[B] |= HALF_CARRY_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 5,H\n",pc-1);
}

// SET 5,L
void z80::cb_0xED() {
    reg8[B] |= HALF_CARRY_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 5,L\n",pc-1);
}

// SET 5,(HL)
void z80::cb_0xEE() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= HALF_CARRY_FLAG;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 5,(HL)\n",pc-1);
}

// SET 5,A
void z80::cb_0xEF() {
    reg8[B] |= HALF_CARRY_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 5,A\n",pc-1);
}

// SET 6,B
void z80::cb_0xF0() {
    reg8[B] |= SUBTRACT_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 6,B\n",pc-1);
}

// SET 6,C
void z80::cb_0xF1() {
    reg8[B] |= SUBTRACT_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 6,C\n",pc-1);
}

// SET 6,D
void z80::cb_0xF2() {
    reg8[B] |= SUBTRACT_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 6,D\n",pc-1);
}

// SET 6,E
void z80::cb_0xF3() {
    reg8[B] |= SUBTRACT_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 6,E\n",pc-1);
}

// SET 6,H
void z80::cb_0xF4() {
    reg8[B] |= SUBTRACT_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 6,H\n",pc-1);
}

// SET 6,L
void z80::cb_0xF5() {
    reg8[B] |= SUBTRACT_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 6,L\n",pc-1);
}

// SET 6,(HL)
void z80::cb_0xF6() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= SUBTRACT_FLAG;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 6,(HL)\n",pc-1);
}

// SET 6,A
void z80::cb_0xF7() {
    reg8[B] |= SUBTRACT_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 6,A\n",pc-1);
}

// SET 7,B
void z80::cb_0xF8() {
    reg8[B] |= ZERO_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 7,B\n",pc-1);
}

// SET 7,C
void z80::cb_0xF9() {
    reg8[B] |= ZERO_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 7,C\n",pc-1);
}

// SET 7,D
void z80::cb_0xFA() {
    reg8[B] |= ZERO_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 7,D\n",pc-1);
}

// SET 7,E
void z80::cb_0xFB() {
    reg8[B] |= ZERO_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 7,E\n",pc-1);
}

// SET 7,H
void z80::cb_0xFC() {
    reg8[B] |= ZERO_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 7,H\n",pc-1);
}

// SET 7,L
void z80::cb_0xFD() {
    reg8[B] |= ZERO_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 7,L\n",pc-1);
}

// SET 7,(HL)
void z80::cb_0xFE() {
    int i = read_byte((reg8[H] << 8) + reg8[L]);
    i |= ZERO_FLAG;
    write_byte((reg8[H] << 8) + reg8[L], i);
    m_cycle = 4;
    printf("[0x%04X] SET 7,(HL)\n",pc-1);
}

// SET 7,A
void z80::cb_0xFF() {
    reg8[B] |= ZERO_FLAG;
    m_cycle = 2;
    printf("[0x%04X] SET 7,A\n",pc-1);
}


// Gameboys's reset vector
void z80::reset() {

	/*
	 *
	 * Z80._r.pc=0x100;
	 * MMU._inbios=0;
	 * Z80._r.sp=0xFFFE;
	 * Z80._r.hl=0x014D;
	 * Z80._r.c=0x13;
	 * Z80._r.e=0xD8;
	 * Z80._r.a=1;
	 *
	 */

	//AF=0x01B0;
	reg8[A] = 0x01;
	reg8[F] = 0xB0;
	//BC=0x0013;
	reg8[B] = 0x00;
	reg8[C] = 0x13;
	//DE=0x00D8;
	reg8[D] = 0x00;
	reg8[E] = 0xD8;
	//HL=0x014D;
	reg8[H] = 0x01;
	reg8[L] = 0x4D;
	sp=0xFFFE;

	pc = ENTRY_POINT;
	in_bios = false;
}

void z80::execute() {
	unsigned char instruction = read_byte(pc++);
	(this->*opcodes[static_cast<int>(instruction)])();
	clock_m += m_cycle;
}


void z80::t_execute(int instruction) {
	pc++;
	(this->*opcodes[instruction])();
}



	
