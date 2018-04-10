#ifndef Z80_H
#define Z80_H

#include "rom.hpp"
#include "gpu.hpp"
#include "gtest/gtest_prod.h"

// Registers

#define A 0	// Accumulator
#define B 1
#define C 2
#define D 3
#define E 4
#define H 5
#define L 6
#define F 7	// Flags

#define REGISTERS 8
#define NUM_OPCODES 256
#define CB_PREFIX 256
#define ENTRY_POINT 0x0100

#define MEMORY_MAX 65535
#define BANK_SIZE 16384

// Number of "machine" cycles per frame
// 1 full frame is 70224 clock cycles
// 1 machine cycle = 4 clock cycles
// 70224 / 4 = 17556
#define FRAME_CYCLES 17556

class z80 {

    public:
        z80();
        ~z80();
		void reset();

		// Loads a 16KB bank into slot 0
		void load_bank(const rom &r);
		void execute();

		// Memory related functions
		// Read and write 1 byte (8-bits)
		void write_byte(int address, int byte);
		int read_byte(int address) ;

		// Read and write a word (16-bits)
		void write_word(int address, int word);
		int read_word(int address);

		// Test execute
		void t_execute(int instruction);

    private:
		FRIEND_TEST(Z80, Instructions);
		friend class gb;

		gpu gb_gpu;
        int reg8[REGISTERS];    // 8/16-bit registers
        unsigned short pc;		// 16-bit registers
		unsigned short sp;		// 16-bit registers

		int clock_m;			// Internal clock for Machine cycles

		int m_cycle;			// Machine cycles
		int t_cycle;			// Time cycles FIXME: Apparently not needed?
		bool ime;				// Master interrupt
		bool halt;				// Halting flag

		// Memory
		unsigned char memory[MEMORY_MAX];
		bool in_bios;
		const int bios[256] =
		{
			0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
			0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
			0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
			0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
			0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
			0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
			0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
			0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
			0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
			0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
			0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
			0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
			0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
			0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
			0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
			0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
		};

		// Main opcodes
		void (z80::*opcodes[NUM_OPCODES])();

		// CB prefix opcodes
		void (z80::*cb_opcodes[CB_PREFIX])();

		// Unmapped opcode
		void f_UNMAPPED(int opcode);

		void f_0x00();
		void f_0x01();
		void f_0x02();
		void f_0x03();
		void f_0x04();
		void f_0x05();
		void f_0x06();
		void f_0x07();
		void f_0x08();
		void f_0x09();
		void f_0x0A();
		void f_0x0B();
		void f_0x0C();
		void f_0x0D();
		void f_0x0E();
		void f_0x0F();
		void f_0x10();
		void f_0x11();
		void f_0x12();
		void f_0x13();
		void f_0x14();
		void f_0x15();
		void f_0x16();
		void f_0x17();
		void f_0x18();
		void f_0x19();
		void f_0x1A();
		void f_0x1B();
		void f_0x1C();
		void f_0x1D();
		void f_0x1E();
		void f_0x1F();
		void f_0x20();
		void f_0x21();
		void f_0x22();
		void f_0x23();
		void f_0x24();
		void f_0x25();
		void f_0x26();
		void f_0x27();
		void f_0x28();
		void f_0x29();
		void f_0x2A();
		void f_0x2B();
		void f_0x2C();
		void f_0x2D();
		void f_0x2E();
		void f_0x2F();
		void f_0x30();
		void f_0x31();
		void f_0x32();
		void f_0x33();
		void f_0x34();
		void f_0x35();
		void f_0x36();
		void f_0x37();
		void f_0x38();
		void f_0x39();
		void f_0x3A();
		void f_0x3B();
		void f_0x3C();
		void f_0x3D();
		void f_0x3E();
		void f_0x3F();
		void f_0x40();
		void f_0x41();
		void f_0x42();
		void f_0x43();
		void f_0x44();
		void f_0x45();
		void f_0x46();
		void f_0x47();
		void f_0x48();
		void f_0x49();
		void f_0x4A();
		void f_0x4B();
		void f_0x4C();
		void f_0x4D();
		void f_0x4E();
		void f_0x4F();
		void f_0x50();
		void f_0x51();
		void f_0x52();
		void f_0x53();
		void f_0x54();
		void f_0x55();
		void f_0x56();
		void f_0x57();
		void f_0x58();
		void f_0x59();
		void f_0x5A();
		void f_0x5B();
		void f_0x5C();
		void f_0x5D();
		void f_0x5E();
		void f_0x5F();
		void f_0x60();
		void f_0x61();
		void f_0x62();
		void f_0x63();
		void f_0x64();
		void f_0x65();
		void f_0x66();
		void f_0x67();
		void f_0x68();
		void f_0x69();
		void f_0x6A();
		void f_0x6B();
		void f_0x6C();
		void f_0x6D();
		void f_0x6E();
		void f_0x6F();
		void f_0x70();
		void f_0x71();
		void f_0x72();
		void f_0x73();
		void f_0x74();
		void f_0x75();
		void f_0x76();
		void f_0x77();
		void f_0x78();
		void f_0x79();
		void f_0x7A();
		void f_0x7B();
		void f_0x7C();
		void f_0x7D();
		void f_0x7E();
		void f_0x7F();
		void f_0x80();
		void f_0x81();
		void f_0x82();
		void f_0x83();
		void f_0x84();
		void f_0x85();
		void f_0x86();
		void f_0x87();
		void f_0x88();
		void f_0x89();
		void f_0x8A();
		void f_0x8B();
		void f_0x8C();
		void f_0x8D();
		void f_0x8E();
		void f_0x8F();
		void f_0x90();
		void f_0x91();
		void f_0x92();
		void f_0x93();
		void f_0x94();
		void f_0x95();
		void f_0x96();
		void f_0x97();
		void f_0x98();
		void f_0x99();
		void f_0x9A();
		void f_0x9B();
		void f_0x9C();
		void f_0x9D();
		void f_0x9E();
		void f_0x9F();
		void f_0xA0();
		void f_0xA1();
		void f_0xA2();
		void f_0xA3();
		void f_0xA4();
		void f_0xA5();
		void f_0xA6();
		void f_0xA7();
		void f_0xA8();
		void f_0xA9();
		void f_0xAA();
		void f_0xAB();
		void f_0xAC();
		void f_0xAD();
		void f_0xAE();
		void f_0xAF();
		void f_0xB0();
		void f_0xB1();
		void f_0xB2();
		void f_0xB3();
		void f_0xB4();
		void f_0xB5();
		void f_0xB6();
		void f_0xB7();
		void f_0xB8();
		void f_0xB9();
		void f_0xBA();
		void f_0xBB();
		void f_0xBC();
		void f_0xBD();
		void f_0xBE();
		void f_0xBF();
		void f_0xC0();
		void f_0xC1();
		void f_0xC2();
		void f_0xC3();
		void f_0xC4();
		void f_0xC5();
		void f_0xC6();
		void f_0xC7();
		void f_0xC8();
		void f_0xC9();
		void f_0xCA();
		void f_0xCB();
		void f_0xCC();
		void f_0xCD();
		void f_0xCE();
		void f_0xCF();
		void f_0xD0();
		void f_0xD1();
		void f_0xD2();
		void f_0xD3();
		void f_0xD4();
		void f_0xD5();
		void f_0xD6();
		void f_0xD7();
		void f_0xD8();
		void f_0xD9();
		void f_0xDA();
		void f_0xDB();
		void f_0xDC();
		void f_0xDD();
		void f_0xDE();
		void f_0xDF();
		void f_0xE0();
		void f_0xE1();
		void f_0xE2();
		void f_0xE3();
		void f_0xE4();
		void f_0xE5();
		void f_0xE6();
		void f_0xE7();
		void f_0xE8();
		void f_0xE9();
		void f_0xEA();
		void f_0xEB();
		void f_0xEC();
		void f_0xED();
		void f_0xEE();
		void f_0xEF();
		void f_0xF0();
		void f_0xF1();
		void f_0xF2();
		void f_0xF3();
		void f_0xF4();
		void f_0xF5();
		void f_0xF6();
		void f_0xF7();
		void f_0xF8();
		void f_0xF9();
		void f_0xFA();
		void f_0xFB();
		void f_0xFC();
		void f_0xFD();
		void f_0xFE();
		void f_0xFF();

		// CB prefix opcodes
		void cb_0x00();
		void cb_0x01();
		void cb_0x02();
		void cb_0x03();
		void cb_0x04();
		void cb_0x05();
		void cb_0x06();
		void cb_0x07();
		void cb_0x08();
		void cb_0x09();
		void cb_0x0A();
		void cb_0x0B();
		void cb_0x0C();
		void cb_0x0D();
		void cb_0x0E();
		void cb_0x0F();
		void cb_0x10();
		void cb_0x11();
		void cb_0x12();
		void cb_0x13();
		void cb_0x14();
		void cb_0x15();
		void cb_0x16();
		void cb_0x17();
		void cb_0x18();
		void cb_0x19();
		void cb_0x1A();
		void cb_0x1B();
		void cb_0x1C();
		void cb_0x1D();
		void cb_0x1E();
		void cb_0x1F();
		void cb_0x20();
		void cb_0x21();
		void cb_0x22();
		void cb_0x23();
		void cb_0x24();
		void cb_0x25();
		void cb_0x26();
		void cb_0x27();
		void cb_0x28();
		void cb_0x29();
		void cb_0x2A();
		void cb_0x2B();
		void cb_0x2C();
		void cb_0x2D();
		void cb_0x2E();
		void cb_0x2F();
		void cb_0x30();
		void cb_0x31();
		void cb_0x32();
		void cb_0x33();
		void cb_0x34();
		void cb_0x35();
		void cb_0x36();
		void cb_0x37();
		void cb_0x38();
		void cb_0x39();
		void cb_0x3A();
		void cb_0x3B();
		void cb_0x3C();
		void cb_0x3D();
		void cb_0x3E();
		void cb_0x3F();
		void cb_0x40();
		void cb_0x41();
		void cb_0x42();
		void cb_0x43();
		void cb_0x44();
		void cb_0x45();
		void cb_0x46();
		void cb_0x47();
		void cb_0x48();
		void cb_0x49();
		void cb_0x4A();
		void cb_0x4B();
		void cb_0x4C();
		void cb_0x4D();
		void cb_0x4E();
		void cb_0x4F();
		void cb_0x50();
		void cb_0x51();
		void cb_0x52();
		void cb_0x53();
		void cb_0x54();
		void cb_0x55();
		void cb_0x56();
		void cb_0x57();
		void cb_0x58();
		void cb_0x59();
		void cb_0x5A();
		void cb_0x5B();
		void cb_0x5C();
		void cb_0x5D();
		void cb_0x5E();
		void cb_0x5F();
		void cb_0x60();
		void cb_0x61();
		void cb_0x62();
		void cb_0x63();
		void cb_0x64();
		void cb_0x65();
		void cb_0x66();
		void cb_0x67();
		void cb_0x68();
		void cb_0x69();
		void cb_0x6A();
		void cb_0x6B();
		void cb_0x6C();
		void cb_0x6D();
		void cb_0x6E();
		void cb_0x6F();
		void cb_0x70();
		void cb_0x71();
		void cb_0x72();
		void cb_0x73();
		void cb_0x74();
		void cb_0x75();
		void cb_0x76();
		void cb_0x77();
		void cb_0x78();
		void cb_0x79();
		void cb_0x7A();
		void cb_0x7B();
		void cb_0x7C();
		void cb_0x7D();
		void cb_0x7E();
		void cb_0x7F();
		void cb_0x80();
		void cb_0x81();
		void cb_0x82();
		void cb_0x83();
		void cb_0x84();
		void cb_0x85();
		void cb_0x86();
		void cb_0x87();
		void cb_0x88();
		void cb_0x89();
		void cb_0x8A();
		void cb_0x8B();
		void cb_0x8C();
		void cb_0x8D();
		void cb_0x8E();
		void cb_0x8F();
		void cb_0x90();
		void cb_0x91();
		void cb_0x92();
		void cb_0x93();
		void cb_0x94();
		void cb_0x95();
		void cb_0x96();
		void cb_0x97();
		void cb_0x98();
		void cb_0x99();
		void cb_0x9A();
		void cb_0x9B();
		void cb_0x9C();
		void cb_0x9D();
		void cb_0x9E();
		void cb_0x9F();
		void cb_0xA0();
		void cb_0xA1();
		void cb_0xA2();
		void cb_0xA3();
		void cb_0xA4();
		void cb_0xA5();
		void cb_0xA6();
		void cb_0xA7();
		void cb_0xA8();
		void cb_0xA9();
		void cb_0xAA();
		void cb_0xAB();
		void cb_0xAC();
		void cb_0xAD();
		void cb_0xAE();
		void cb_0xAF();
		void cb_0xB0();
		void cb_0xB1();
		void cb_0xB2();
		void cb_0xB3();
		void cb_0xB4();
		void cb_0xB5();
		void cb_0xB6();
		void cb_0xB7();
		void cb_0xB8();
		void cb_0xB9();
		void cb_0xBA();
		void cb_0xBB();
		void cb_0xBC();
		void cb_0xBD();
		void cb_0xBE();
		void cb_0xBF();
		void cb_0xC0();
		void cb_0xC1();
		void cb_0xC2();
		void cb_0xC3();
		void cb_0xC4();
		void cb_0xC5();
		void cb_0xC6();
		void cb_0xC7();
		void cb_0xC8();
		void cb_0xC9();
		void cb_0xCA();
		void cb_0xCB();
		void cb_0xCC();
		void cb_0xCD();
		void cb_0xCE();
		void cb_0xCF();
		void cb_0xD0();
		void cb_0xD1();
		void cb_0xD2();
		void cb_0xD3();
		void cb_0xD4();
		void cb_0xD5();
		void cb_0xD6();
		void cb_0xD7();
		void cb_0xD8();
		void cb_0xD9();
		void cb_0xDA();
		void cb_0xDB();
		void cb_0xDC();
		void cb_0xDD();
		void cb_0xDE();
		void cb_0xDF();
		void cb_0xE0();
		void cb_0xE1();
		void cb_0xE2();
		void cb_0xE3();
		void cb_0xE4();
		void cb_0xE5();
		void cb_0xE6();
		void cb_0xE7();
		void cb_0xE8();
		void cb_0xE9();
		void cb_0xEA();
		void cb_0xEB();
		void cb_0xEC();
		void cb_0xED();
		void cb_0xEE();
		void cb_0xEF();
		void cb_0xF0();
		void cb_0xF1();
		void cb_0xF2();
		void cb_0xF3();
		void cb_0xF4();
		void cb_0xF5();
		void cb_0xF6();
		void cb_0xF7();
		void cb_0xF8();
		void cb_0xF9();
		void cb_0xFA();
		void cb_0xFB();
		void cb_0xFC();
		void cb_0xFD();
		void cb_0xFE();
		void cb_0xFF();


};

#endif
