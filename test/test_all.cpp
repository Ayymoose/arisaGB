#include <iostream>
#include <cassert>
#include <cstring>

#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

#include "rom.hpp"
#include "gpu.hpp"
#include "z80.hpp"



	TEST(RomTest,Files) {
		rom r;

		try {
			r.load_rom("mm");
		} catch (const char * e) {
			EXPECT_EQ(e,std::string("File couldn't be opened!"));
		}

		try {
			r.load_rom("/home/ayman/Desktop/arisaGB/test/rom/rom_0.gbc");
		} catch (const char * e) {
			EXPECT_EQ(e,std::string("File size is 0!"));
		}

		try {
			r.load_rom("/home/ayman/Desktop/arisaGB/test/rom/rom_inv.gbc");
		} catch (const char * e) {
			EXPECT_EQ(e,std::string("Invalid ROM!"));
		}

		/*try {
			r.load_rom("/home/ayman/Desktop/arisaGB/test/rom/rom_inv.gbc");
		} catch (const char * e) {
			EXPECT_EQ(e,std::string("File size too big!"));
		}*/
	}

	TEST(Z80,Memory) {
		z80 cpu;

		cpu.write_byte(0x0000,0xAA);
		EXPECT_EQ(cpu.read_byte(0x0000),0xAA);

		cpu.write_word(0x0000,0xCAFE);			//FE CA
		EXPECT_EQ(cpu.read_word(0x0000),0xCAFE);
		EXPECT_EQ(cpu.read_byte(0x0000),0xFE);
		EXPECT_EQ(cpu.read_byte(0x0001),0xCA);

		// TODO: More tests
	}

	TEST(Z80, Instructions) {
		z80 z;

		// LD instructions
		// 1. Load an unsigned value
		// 2. Load a signed value
		// 3. Load an edge case unsigned value
		// 4. Load an edge case signed value

		// TODO: LD BC, d16

		/*z.reg8[B] = 0x10;
		z.reg8[C] = 0x20;
		z.t_execute(0x01);*/

		// LD (BC), A
		z.reg8[B] = 0x10;
		z.reg8[C] = 0x20;
		z.reg8[A] = 0xAC;
		z.t_execute(0x02);
		EXPECT_EQ(z.memory[0x1020],0xAC);
		/*z.reg8[A] = -20;
		z.t_execute(0x02);
		EXPECT_EQ(z.memory[0x1020],-20);*/
		z.reg8[A] = 255;
		z.t_execute(0x02);
		EXPECT_EQ(z.memory[0x1020],255);
		/*z.reg8[A] = -128;
		z.t_execute(0x02);
		EXPECT_EQ(z.memory[0x1020],-128);*/

		// TODO: INC BC

		// INC B
		z.reg8[B] = 0x05;
		z.t_execute(0x04);
		EXPECT_EQ(z.reg8[B],0x06);
		z.reg8[B] = 0xFF;
		z.t_execute(0x04);
		EXPECT_EQ(z.reg8[B],0x00);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[B] = -50;
		z.t_execute(0x04);
		EXPECT_EQ(z.reg8[B],-49);*/

		// DEC B
		z.reg8[B] = 0x05;
		z.t_execute(0x05);
		EXPECT_EQ(z.reg8[B],0x04);
		z.reg8[B] = 0x01;
		z.t_execute(0x05);
		EXPECT_EQ(z.reg8[B],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[B] = -50;
		z.t_execute(0x05);
		EXPECT_EQ(z.reg8[B],-51);*/

		// TODO: LD B,d8
		// TODO: RLCA
		// TODO: LD (a16),SP

		// ADD HL,BC
		z.reg8[B] = 0x10;
		z.reg8[C] = 0x20;
		z.reg8[H] = 0x40;
		z.reg8[L] = 0x00;
		z.t_execute(0x09);
		EXPECT_EQ((z.reg8[H] << 8) + z.reg8[L],0x5020);
		z.reg8[B] = 0xFF;
		z.reg8[C] = 0xFF;
		z.reg8[H] = 0x40;
		z.reg8[L] = 0x00;
		z.t_execute(0x09);
		EXPECT_EQ((z.reg8[H] << 8) + z.reg8[L],0x3FFF);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// LD A,(BC)
		z.reg8[B] = 0x10;
		z.reg8[C] = 0x20;
		z.memory[0x1020] = 0x10;
		z.t_execute(0x0A);
		EXPECT_EQ(z.reg8[A],0x10);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x0A);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.memory[0x1020] = 255;
		z.t_execute(0x0A);
		EXPECT_EQ(z.reg8[A],255);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x0A);
		EXPECT_EQ(z.reg8[A],-128);*/

		// TODO: DEC BC

		// INC C
		z.reg8[C] = 0x05;
		z.t_execute(0x0C);
		EXPECT_EQ(z.reg8[C],0x06);
		z.reg8[C] = 0xFF;
		z.t_execute(0x0C);
		EXPECT_EQ(z.reg8[C],0x00);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[C] = -50;
		z.t_execute(0x0C);
		EXPECT_EQ(z.reg8[C],-49);*/

		// DEC C
		z.reg8[C] = 0x05;
		z.t_execute(0x0D);
		EXPECT_EQ(z.reg8[C],0x04);
		z.reg8[C] = 0x01;
		z.t_execute(0x0D);
		EXPECT_EQ(z.reg8[C],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[C] = -50;
		z.t_execute(0x0D);
		EXPECT_EQ(z.reg8[C],-51);*/

		// TODO: LD C,d8
		// TODO: RRCA
		// TODO: DJNZ
		// TODO: LD DE,d16

		// LD (DE),A
		z.reg8[A] = 0x10;
		z.reg8[D] = 0x10;
		z.reg8[E] = 0x20;
		z.t_execute(0x12);
		EXPECT_EQ(z.memory[0x1020],0x10);
		/*z.reg8[A] = -20;
		z.t_execute(0x12);
		EXPECT_EQ(z.memory[0x1020],-20);
		z.reg8[A] = -128;
		z.t_execute(0x12);
		EXPECT_EQ(z.memory[0x1020],-128);*/
		z.reg8[A] = 255;
		z.t_execute(0x12);
		EXPECT_EQ(z.memory[0x1020],255);

		// TODO: INC DE

		// INC D
		z.reg8[D] = 0x05;
		z.t_execute(0x14);
		EXPECT_EQ(z.reg8[D],0x06);
		z.reg8[D] = 0xFF;
		z.t_execute(0x14);
		EXPECT_EQ(z.reg8[D],0x00);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[D] = -50;
		z.t_execute(0x14);
		EXPECT_EQ(z.reg8[D],-49);*/

		// DEC D
		z.reg8[D] = 0x05;
		z.t_execute(0x15);
		EXPECT_EQ(z.reg8[D],0x04);
		z.reg8[D] = 0x01;
		z.t_execute(0x15);
		EXPECT_EQ(z.reg8[D],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[D] = -50;
		z.t_execute(0x15);
		EXPECT_EQ(z.reg8[D],-51);*/

		// TODO LD D,d8
		// TODO RLA
		// TODO JR r8

		// ADD HL,DE
		z.reg8[D] = 0x10;
		z.reg8[E] = 0x20;
		z.reg8[H] = 0x40;
		z.reg8[L] = 0x00;
		z.t_execute(0x19);
		EXPECT_EQ((z.reg8[H] << 8) + z.reg8[L],0x5020);
		z.reg8[D] = 0xFF;
		z.reg8[E] = 0xFF;
		z.reg8[H] = 0x40;
		z.reg8[L] = 0x00;
		z.t_execute(0x19);
		EXPECT_EQ((z.reg8[H] << 8) + z.reg8[L],0x3FFF);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// LD A,(DE)
		z.reg8[D] = 0x10;
		z.reg8[E] = 0x20;
		z.memory[0x1020] = 0x10;
		z.t_execute(0x1A);
		EXPECT_EQ(z.reg8[A],0x10);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x1A);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.memory[0x1020] = 255;
		z.t_execute(0x1A);
		EXPECT_EQ(z.reg8[A],255);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x1A);
		EXPECT_EQ(z.reg8[A],-128);*/

		// TODO DEC DE

		// INC E
		z.reg8[E] = 0x05;
		z.t_execute(0x1C);
		EXPECT_EQ(z.reg8[E],0x06);
		z.reg8[E] = 0xFF;
		z.t_execute(0x1C);
		EXPECT_EQ(z.reg8[E],0x00);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[E] = -50;
		z.t_execute(0x1C);
		EXPECT_EQ(z.reg8[E],-49);*/

		// DEC E
		z.reg8[E] = 0x05;
		z.t_execute(0x1D);
		EXPECT_EQ(z.reg8[E],0x04);
		z.reg8[E] = 0x01;
		z.t_execute(0x1D);
		EXPECT_EQ(z.reg8[E],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[E] = -50;
		z.t_execute(0x1D);
		EXPECT_EQ(z.reg8[E],-51);*/

		// TODO: LD E,d8
		// TODO: RRA
		// TODO: JR NZ,r8
		// TODO: LD HL,d16

		// TODO: LD (HL+),A
		// TODO: INC HL

		// INC H
		z.reg8[H] = 0x05;
		z.t_execute(0x24);
		EXPECT_EQ(z.reg8[H],0x06);
		z.reg8[H] = 0xFF;
		z.t_execute(0x24);
		EXPECT_EQ(z.reg8[H],0x00);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[H] = -50;
		z.t_execute(0x24);
		EXPECT_EQ(z.reg8[H],-49);*/

		// DEC H
		z.reg8[H] = 0x05;
		z.t_execute(0x25);
		EXPECT_EQ(z.reg8[H],0x04);
		z.reg8[H] = 0x01;
		z.t_execute(0x25);
		EXPECT_EQ(z.reg8[H],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[H] = -50;
		z.t_execute(0x25);
		EXPECT_EQ(z.reg8[H],-51);*/

		// TODO: LD H,d8
		// TODO: DAA
		// TODO: JR Z,r8

		// ADD HL,HL
		z.reg8[H] = 0x20;
		z.reg8[L] = 0x20;
		z.t_execute(0x29);
		EXPECT_EQ((z.reg8[H] << 8) + z.reg8[L],0x4040);
		z.reg8[H] = 0xDD;
		z.reg8[L] = 0xDD;
		z.t_execute(0x29);
		EXPECT_EQ((z.reg8[H] << 8) + z.reg8[L],0xBBBA);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// TODO: LD A,(HL+)
		// TODO: DEC HL

		// INC L
		z.reg8[L] = 0x05;
		z.t_execute(0x2C);
		EXPECT_EQ(z.reg8[L],0x06);
		z.reg8[L] = 0xFF;
		z.t_execute(0x2C);
		EXPECT_EQ(z.reg8[L],0x00);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[L] = -50;
		z.t_execute(0x2C);
		EXPECT_EQ(z.reg8[L],-49);*/

		// DEC L
		z.reg8[L] = 0x05;
		z.t_execute(0x2D);
		EXPECT_EQ(z.reg8[L],0x04);
		z.reg8[L] = 0x01;
		z.t_execute(0x2D);
		EXPECT_EQ(z.reg8[L],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[L] = -50;
		z.t_execute(0x2D);
		EXPECT_EQ(z.reg8[L],-51);*/

		// TODO: LD L,d8
		// TODO: CPL
		// TODO: JR NC,r8
		// TODO: LD SP,d16
		// TODO: LD (HL-),A

		// INC SP
		z.sp = 0x05;
		z.t_execute(0x33);
		EXPECT_EQ(z.sp,0x06);

		// INC (HL)
		// DEC (HL)

		// TODO: LD (HL),d8
		// TODO: SCF
		// TODO: JR C,r8

		// ADD HL,SP

		// TODO: LD A,(HL-)

		// DEC SP
		z.sp = 0x05;
		z.t_execute(0x3B);
		EXPECT_EQ(z.sp,0x04);

		// INC A
		z.reg8[A] = 0x05;
		z.t_execute(0x3C);
		EXPECT_EQ(z.reg8[A],0x06);
		z.reg8[A] = 0xFF;
		z.t_execute(0x3C);
		EXPECT_EQ(z.reg8[A],0x00);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -50;
		z.t_execute(0x3C);
		EXPECT_EQ(z.reg8[A],-49);*/

		// DEC A
		z.reg8[A] = 0x05;
		z.t_execute(0x3D);
		EXPECT_EQ(z.reg8[A],0x04);
		z.reg8[A] = 0x01;
		z.t_execute(0x3D);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -50;
		z.t_execute(0x3D);
		EXPECT_EQ(z.reg8[A],-51);*/

		// TODO: LD A,d8
		// TODO: CCF

		// LD B,B
		z.reg8[B] = 0x20;
		z.t_execute(0x40);
		EXPECT_EQ(z.reg8[B],0x20);
		z.reg8[B] = -20;
		z.t_execute(0x40);
		EXPECT_EQ(z.reg8[B],-20);
		z.reg8[B] = 127;
		z.t_execute(0x40);
		EXPECT_EQ(z.reg8[B],127);
		z.reg8[B] = -128;
		z.t_execute(0x40);
		EXPECT_EQ(z.reg8[B],-128);

		// LD B,C
		z.reg8[C] = 0x20;
		z.t_execute(0x41);
		EXPECT_EQ(z.reg8[B],0x20);
		z.reg8[C] = -20;
		z.t_execute(0x41);
		EXPECT_EQ(z.reg8[B],-20);
		z.reg8[C] = 127;
		z.t_execute(0x41);
		EXPECT_EQ(z.reg8[B],127);
		z.reg8[C] = -128;
		z.t_execute(0x41);
		EXPECT_EQ(z.reg8[B],-128);

		// LD B,D
		z.reg8[D] = 0x20;
		z.t_execute(0x42);
		EXPECT_EQ(z.reg8[B],0x20);
		z.reg8[D] = -20;
		z.t_execute(0x42);
		EXPECT_EQ(z.reg8[B],-20);
		z.reg8[D] = 127;
		z.t_execute(0x42);
		EXPECT_EQ(z.reg8[B],127);
		z.reg8[D] = -128;
		z.t_execute(0x42);
		EXPECT_EQ(z.reg8[B],-128);

		// LD B,E
		z.reg8[E] = 0x20;
		z.t_execute(0x43);
		EXPECT_EQ(z.reg8[B],0x20);
		z.reg8[E] = -20;
		z.t_execute(0x43);
		EXPECT_EQ(z.reg8[B],-20);
		z.reg8[E] = 127;
		z.t_execute(0x43);
		EXPECT_EQ(z.reg8[B],127);
		z.reg8[E] = -128;
		z.t_execute(0x43);
		EXPECT_EQ(z.reg8[B],-128);

		// LD B,H
		z.reg8[H] = 0x20;
		z.t_execute(0x44);
		EXPECT_EQ(z.reg8[B],0x20);
		z.reg8[H] = -20;
		z.t_execute(0x44);
		EXPECT_EQ(z.reg8[B],-20);
		z.reg8[H] = 127;
		z.t_execute(0x44);
		EXPECT_EQ(z.reg8[B],127);
		z.reg8[H] = -128;
		z.t_execute(0x44);
		EXPECT_EQ(z.reg8[B],-128);

		// LD B,L
		z.reg8[L] = 0x20;
		z.t_execute(0x45);
		EXPECT_EQ(z.reg8[B],0x20);
		z.reg8[L] = -20;
		z.t_execute(0x45);
		EXPECT_EQ(z.reg8[B],-20);
		z.reg8[L] = 127;
		z.t_execute(0x45);
		EXPECT_EQ(z.reg8[B],127);
		z.reg8[L] = -128;
		z.t_execute(0x45);
		EXPECT_EQ(z.reg8[B],-128);

		// LD, B,(HL)
		z.memory[0x1020] = 0x20;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x46);
		EXPECT_EQ(z.reg8[B],0x20);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x46);
		EXPECT_EQ(z.reg8[B],-20);*/
		z.memory[0x1020] = 127;
		z.t_execute(0x46);
		EXPECT_EQ(z.reg8[B],127);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x46);
		EXPECT_EQ(z.reg8[B],-128);*/

		// LD B,A
		z.reg8[A] = 0x20;
		z.t_execute(0x47);
		EXPECT_EQ(z.reg8[B],0x20);
		z.reg8[A] = -20;
		z.t_execute(0x47);
		EXPECT_EQ(z.reg8[B],-20);
		z.reg8[A] = 127;
		z.t_execute(0x47);
		EXPECT_EQ(z.reg8[B],127);
		z.reg8[A] = -128;
		z.t_execute(0x47);
		EXPECT_EQ(z.reg8[B],-128);

		////

		// LD C,B
		z.reg8[B] = 0x20;
		z.t_execute(0x48);
		EXPECT_EQ(z.reg8[C],0x20);
		z.reg8[B] = -20;
		z.t_execute(0x48);
		EXPECT_EQ(z.reg8[C],-20);
		z.reg8[B] = 127;
		z.t_execute(0x48);
		EXPECT_EQ(z.reg8[C],127);
		z.reg8[B] = -128;
		z.t_execute(0x48);
		EXPECT_EQ(z.reg8[C],-128);

		// LD C,C
		z.reg8[C] = 0x20;
		z.t_execute(0x49);
		EXPECT_EQ(z.reg8[C],0x20);
		z.reg8[C] = -20;
		z.t_execute(0x49);
		EXPECT_EQ(z.reg8[C],-20);
		z.reg8[C] = 127;
		z.t_execute(0x49);
		EXPECT_EQ(z.reg8[C],127);
		z.reg8[C] = -128;
		z.t_execute(0x49);
		EXPECT_EQ(z.reg8[C],-128);

		// LD C,D
		z.reg8[D] = 0x20;
		z.t_execute(0x4A);
		EXPECT_EQ(z.reg8[C],0x20);
		z.reg8[D] = -20;
		z.t_execute(0x4A);
		EXPECT_EQ(z.reg8[C],-20);
		z.reg8[D] = 127;
		z.t_execute(0x4A);
		EXPECT_EQ(z.reg8[C],127);
		z.reg8[D] = -128;
		z.t_execute(0x4A);
		EXPECT_EQ(z.reg8[C],-128);

		// LD C,E
		z.reg8[E] = 0x20;
		z.t_execute(0x4B);
		EXPECT_EQ(z.reg8[C],0x20);
		z.reg8[E] = -20;
		z.t_execute(0x4B);
		EXPECT_EQ(z.reg8[C],-20);
		z.reg8[E] = 127;
		z.t_execute(0x4B);
		EXPECT_EQ(z.reg8[C],127);
		z.reg8[E] = -128;
		z.t_execute(0x4B);
		EXPECT_EQ(z.reg8[C],-128);

		// LD C,H
		z.reg8[H] = 0x20;
		z.t_execute(0x4C);
		EXPECT_EQ(z.reg8[C],0x20);
		z.reg8[H] = -20;
		z.t_execute(0x4C);
		EXPECT_EQ(z.reg8[C],-20);
		z.reg8[H] = 127;
		z.t_execute(0x4C);
		EXPECT_EQ(z.reg8[C],127);
		z.reg8[H] = -128;
		z.t_execute(0x4C);
		EXPECT_EQ(z.reg8[C],-128);

		// LD C,L
		z.reg8[L] = 0x20;
		z.t_execute(0x4D);
		EXPECT_EQ(z.reg8[C],0x20);
		z.reg8[L] = -20;
		z.t_execute(0x4D);
		EXPECT_EQ(z.reg8[C],-20);
		z.reg8[L] = 127;
		z.t_execute(0x4D);
		EXPECT_EQ(z.reg8[C],127);
		z.reg8[L] = -128;
		z.t_execute(0x4D);
		EXPECT_EQ(z.reg8[C],-128);

		// LD, C,(HL)
		z.memory[0x1020] = 0x20;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x4E);
		EXPECT_EQ(z.reg8[C],0x20);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x4E);
		EXPECT_EQ(z.reg8[C],-20);*/
		z.memory[0x1020] = 127;
		z.t_execute(0x4E);
		EXPECT_EQ(z.reg8[C],127);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x4E);
		EXPECT_EQ(z.reg8[C],-128);*/

		// LD C,A
		z.reg8[A] = 0x20;
		z.t_execute(0x4F);
		EXPECT_EQ(z.reg8[C],0x20);
		z.reg8[A] = -20;
		z.t_execute(0x4F);
		EXPECT_EQ(z.reg8[C],-20);
		z.reg8[A] = 127;
		z.t_execute(0x4F);
		EXPECT_EQ(z.reg8[C],127);
		z.reg8[A] = -128;
		z.t_execute(0x4F);
		EXPECT_EQ(z.reg8[C],-128);

		////

		// LD D,B
		z.reg8[B] = 0x20;
		z.t_execute(0x50);
		EXPECT_EQ(z.reg8[D],0x20);
		z.reg8[B] = -20;
		z.t_execute(0x50);
		EXPECT_EQ(z.reg8[D],-20);
		z.reg8[B] = 127;
		z.t_execute(0x50);
		EXPECT_EQ(z.reg8[D],127);
		z.reg8[B] = -128;
		z.t_execute(0x50);
		EXPECT_EQ(z.reg8[D],-128);

		// LD D,C
		z.reg8[C] = 0x20;
		z.t_execute(0x51);
		EXPECT_EQ(z.reg8[D],0x20);
		z.reg8[C] = -20;
		z.t_execute(0x51);
		EXPECT_EQ(z.reg8[D],-20);
		z.reg8[C] = 127;
		z.t_execute(0x51);
		EXPECT_EQ(z.reg8[D],127);
		z.reg8[C] = -128;
		z.t_execute(0x51);
		EXPECT_EQ(z.reg8[D],-128);

		// LD D,D
		z.reg8[D] = 0x20;
		z.t_execute(0x52);
		EXPECT_EQ(z.reg8[D],0x20);
		z.reg8[D] = -20;
		z.t_execute(0x52);
		EXPECT_EQ(z.reg8[D],-20);
		z.reg8[D] = 127;
		z.t_execute(0x52);
		EXPECT_EQ(z.reg8[D],127);
		z.reg8[D] = -128;
		z.t_execute(0x52);
		EXPECT_EQ(z.reg8[D],-128);

		// LD D,E
		z.reg8[E] = 0x20;
		z.t_execute(0x53);
		EXPECT_EQ(z.reg8[D],0x20);
		z.reg8[E] = -20;
		z.t_execute(0x53);
		EXPECT_EQ(z.reg8[D],-20);
		z.reg8[E] = 127;
		z.t_execute(0x53);
		EXPECT_EQ(z.reg8[D],127);
		z.reg8[E] = -128;
		z.t_execute(0x53);
		EXPECT_EQ(z.reg8[D],-128);

		// LD D,H
		z.reg8[H] = 0x20;
		z.t_execute(0x54);
		EXPECT_EQ(z.reg8[D],0x20);
		z.reg8[H] = -20;
		z.t_execute(0x54);
		EXPECT_EQ(z.reg8[D],-20);
		z.reg8[H] = 127;
		z.t_execute(0x54);
		EXPECT_EQ(z.reg8[D],127);
		z.reg8[H] = -128;
		z.t_execute(0x54);
		EXPECT_EQ(z.reg8[D],-128);

		// LD D,L
		z.reg8[L] = 0x20;
		z.t_execute(0x55);
		EXPECT_EQ(z.reg8[D],0x20);
		z.reg8[L] = -20;
		z.t_execute(0x55);
		EXPECT_EQ(z.reg8[D],-20);
		z.reg8[L] = 127;
		z.t_execute(0x55);
		EXPECT_EQ(z.reg8[D],127);
		z.reg8[L] = -128;
		z.t_execute(0x55);
		EXPECT_EQ(z.reg8[D],-128);

		// LD, D,(HL)
		z.memory[0x1020] = 0x20;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x56);
		EXPECT_EQ(z.reg8[D],0x20);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x56);
		EXPECT_EQ(z.reg8[D],-20);*/
		z.memory[0x1020] = 127;
		z.t_execute(0x56);
		EXPECT_EQ(z.reg8[D],127);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x56);
		EXPECT_EQ(z.reg8[D],-128);*/

		// LD D,A
		z.reg8[A] = 0x20;
		z.t_execute(0x57);
		EXPECT_EQ(z.reg8[D],0x20);
		z.reg8[A] = -20;
		z.t_execute(0x57);
		EXPECT_EQ(z.reg8[D],-20);
		z.reg8[A] = 127;
		z.t_execute(0x57);
		EXPECT_EQ(z.reg8[D],127);
		z.reg8[A] = -128;
		z.t_execute(0x57);
		EXPECT_EQ(z.reg8[D],-128);

		////

		// LD E,B
		z.reg8[B] = 0x20;
		z.t_execute(0x58);
		EXPECT_EQ(z.reg8[E],0x20);
		z.reg8[B] = -20;
		z.t_execute(0x58);
		EXPECT_EQ(z.reg8[E],-20);
		z.reg8[B] = 127;
		z.t_execute(0x58);
		EXPECT_EQ(z.reg8[E],127);
		z.reg8[B] = -128;
		z.t_execute(0x58);
		EXPECT_EQ(z.reg8[E],-128);

		// LD E,C
		z.reg8[C] = 0x20;
		z.t_execute(0x59);
		EXPECT_EQ(z.reg8[E],0x20);
		z.reg8[C] = -20;
		z.t_execute(0x59);
		EXPECT_EQ(z.reg8[E],-20);
		z.reg8[C] = 127;
		z.t_execute(0x59);
		EXPECT_EQ(z.reg8[E],127);
		z.reg8[C] = -128;
		z.t_execute(0x59);
		EXPECT_EQ(z.reg8[E],-128);

		// LD E,D
		z.reg8[D] = 0x20;
		z.t_execute(0x5A);
		EXPECT_EQ(z.reg8[E],0x20);
		z.reg8[D] = -20;
		z.t_execute(0x5A);
		EXPECT_EQ(z.reg8[E],-20);
		z.reg8[D] = 127;
		z.t_execute(0x5A);
		EXPECT_EQ(z.reg8[E],127);
		z.reg8[D] = -128;
		z.t_execute(0x5A);
		EXPECT_EQ(z.reg8[E],-128);

		// LD E,E
		z.reg8[E] = 0x20;
		z.t_execute(0x5B);
		EXPECT_EQ(z.reg8[E],0x20);
		z.reg8[E] = -20;
		z.t_execute(0x5B);
		EXPECT_EQ(z.reg8[E],-20);
		z.reg8[E] = 127;
		z.t_execute(0x5B);
		EXPECT_EQ(z.reg8[E],127);
		z.reg8[E] = -128;
		z.t_execute(0x5B);
		EXPECT_EQ(z.reg8[E],-128);

		// LD E,H
		z.reg8[H] = 0x20;
		z.t_execute(0x5C);
		EXPECT_EQ(z.reg8[E],0x20);
		z.reg8[H] = -20;
		z.t_execute(0x5C);
		EXPECT_EQ(z.reg8[E],-20);
		z.reg8[H] = 127;
		z.t_execute(0x5C);
		EXPECT_EQ(z.reg8[E],127);
		z.reg8[H] = -128;
		z.t_execute(0x5C);
		EXPECT_EQ(z.reg8[E],-128);

		// LD E,L
		z.reg8[L] = 0x20;
		z.t_execute(0x5D);
		EXPECT_EQ(z.reg8[E],0x20);
		z.reg8[L] = -20;
		z.t_execute(0x5D);
		EXPECT_EQ(z.reg8[E],-20);
		z.reg8[L] = 127;
		z.t_execute(0x5D);
		EXPECT_EQ(z.reg8[E],127);
		z.reg8[L] = -128;
		z.t_execute(0x5D);
		EXPECT_EQ(z.reg8[E],-128);

		// LD, E,(HL)
		z.memory[0x1020] = 0x20;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x5E);
		EXPECT_EQ(z.reg8[E],0x20);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x5E);
		EXPECT_EQ(z.reg8[E],-20);*/
		z.memory[0x1020] = 127;
		z.t_execute(0x5E);
		EXPECT_EQ(z.reg8[E],127);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x5E);
		EXPECT_EQ(z.reg8[E],-128);*/

		// LD E,A
		z.reg8[A] = 0x20;
		z.t_execute(0x5F);
		EXPECT_EQ(z.reg8[E],0x20);
		z.reg8[A] = -20;
		z.t_execute(0x5F);
		EXPECT_EQ(z.reg8[E],-20);
		z.reg8[A] = 127;
		z.t_execute(0x5F);
		EXPECT_EQ(z.reg8[E],127);
		z.reg8[A] = -128;
		z.t_execute(0x5F);
		EXPECT_EQ(z.reg8[E],-128);

		////

		// LD H,B
		z.reg8[B] = 0x20;
		z.t_execute(0x60);
		EXPECT_EQ(z.reg8[H],0x20);
		z.reg8[B] = -20;
		z.t_execute(0x60);
		EXPECT_EQ(z.reg8[H],-20);
		z.reg8[B] = 127;
		z.t_execute(0x60);
		EXPECT_EQ(z.reg8[H],127);
		z.reg8[B] = -128;
		z.t_execute(0x60);
		EXPECT_EQ(z.reg8[H],-128);

		// LD H,C
		z.reg8[C] = 0x20;
		z.t_execute(0x61);
		EXPECT_EQ(z.reg8[H],0x20);
		z.reg8[C] = -20;
		z.t_execute(0x61);
		EXPECT_EQ(z.reg8[H],-20);
		z.reg8[C] = 127;
		z.t_execute(0x61);
		EXPECT_EQ(z.reg8[H],127);
		z.reg8[C] = -128;
		z.t_execute(0x61);
		EXPECT_EQ(z.reg8[H],-128);

		// LD H,D
		z.reg8[D] = 0x20;
		z.t_execute(0x62);
		EXPECT_EQ(z.reg8[H],0x20);
		z.reg8[D] = -20;
		z.t_execute(0x62);
		EXPECT_EQ(z.reg8[H],-20);
		z.reg8[D] = 127;
		z.t_execute(0x62);
		EXPECT_EQ(z.reg8[H],127);
		z.reg8[D] = -128;
		z.t_execute(0x62);
		EXPECT_EQ(z.reg8[H],-128);

		// LD H,E
		z.reg8[E] = 0x20;
		z.t_execute(0x63);
		EXPECT_EQ(z.reg8[H],0x20);
		z.reg8[E] = -20;
		z.t_execute(0x63);
		EXPECT_EQ(z.reg8[H],-20);
		z.reg8[E] = 127;
		z.t_execute(0x63);
		EXPECT_EQ(z.reg8[H],127);
		z.reg8[E] = -128;
		z.t_execute(0x63);
		EXPECT_EQ(z.reg8[H],-128);

		// LD H,H
		z.reg8[H] = 0x20;
		z.t_execute(0x64);
		EXPECT_EQ(z.reg8[H],0x20);
		z.reg8[H] = -20;
		z.t_execute(0x64);
		EXPECT_EQ(z.reg8[H],-20);
		z.reg8[H] = 127;
		z.t_execute(0x64);
		EXPECT_EQ(z.reg8[H],127);
		z.reg8[H] = -128;
		z.t_execute(0x64);
		EXPECT_EQ(z.reg8[H],-128);

		// LD H,L
		z.reg8[L] = 0x20;
		z.t_execute(0x65);
		EXPECT_EQ(z.reg8[H],0x20);
		z.reg8[L] = -20;
		z.t_execute(0x65);
		EXPECT_EQ(z.reg8[H],-20);
		z.reg8[L] = 127;
		z.t_execute(0x65);
		EXPECT_EQ(z.reg8[H],127);
		z.reg8[L] = -128;
		z.t_execute(0x65);
		EXPECT_EQ(z.reg8[H],-128);

		// LD, H,(HL)
		z.memory[0x1020] = 0x20;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x66);
		EXPECT_EQ(z.reg8[H],0x20);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x66);
		EXPECT_EQ(z.reg8[H],-20);*/
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.memory[0x1020] = 127;
		z.t_execute(0x66);
		EXPECT_EQ(z.reg8[H],127);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x66);
		EXPECT_EQ(z.reg8[H],-128);*/

		// LD H,A
		z.reg8[A] = 0x20;
		z.t_execute(0x67);
		EXPECT_EQ(z.reg8[H],0x20);
		z.reg8[A] = -20;
		z.t_execute(0x67);
		EXPECT_EQ(z.reg8[H],-20);
		z.reg8[A] = 127;
		z.t_execute(0x67);
		EXPECT_EQ(z.reg8[H],127);
		z.reg8[A] = -128;
		z.t_execute(0x67);
		EXPECT_EQ(z.reg8[H],-128);

		////

		// LD L,B
		z.reg8[B] = 0x20;
		z.t_execute(0x68);
		EXPECT_EQ(z.reg8[L],0x20);
		z.reg8[B] = -20;
		z.t_execute(0x68);
		EXPECT_EQ(z.reg8[L],-20);
		z.reg8[B] = 127;
		z.t_execute(0x68);
		EXPECT_EQ(z.reg8[L],127);
		z.reg8[B] = -128;
		z.t_execute(0x68);
		EXPECT_EQ(z.reg8[L],-128);

		// LD L,C
		z.reg8[C] = 0x20;
		z.t_execute(0x69);
		EXPECT_EQ(z.reg8[L],0x20);
		z.reg8[C] = -20;
		z.t_execute(0x69);
		EXPECT_EQ(z.reg8[L],-20);
		z.reg8[C] = 127;
		z.t_execute(0x69);
		EXPECT_EQ(z.reg8[L],127);
		z.reg8[C] = -128;
		z.t_execute(0x69);
		EXPECT_EQ(z.reg8[L],-128);

		// LD L,D
		z.reg8[D] = 0x20;
		z.t_execute(0x6A);
		EXPECT_EQ(z.reg8[L],0x20);
		z.reg8[D] = -20;
		z.t_execute(0x6A);
		EXPECT_EQ(z.reg8[L],-20);
		z.reg8[D] = 127;
		z.t_execute(0x6A);
		EXPECT_EQ(z.reg8[L],127);
		z.reg8[D] = -128;
		z.t_execute(0x6A);
		EXPECT_EQ(z.reg8[L],-128);

		// LD L,E
		z.reg8[E] = 0x20;
		z.t_execute(0x6B);
		EXPECT_EQ(z.reg8[L],0x20);
		z.reg8[E] = -20;
		z.t_execute(0x6B);
		EXPECT_EQ(z.reg8[L],-20);
		z.reg8[E] = 127;
		z.t_execute(0x6B);
		EXPECT_EQ(z.reg8[L],127);
		z.reg8[E] = -128;
		z.t_execute(0x6B);
		EXPECT_EQ(z.reg8[L],-128);

		// LD L,H
		z.reg8[H] = 0x20;
		z.t_execute(0x6C);
		EXPECT_EQ(z.reg8[L],0x20);
		z.reg8[H] = -20;
		z.t_execute(0x6C);
		EXPECT_EQ(z.reg8[L],-20);
		z.reg8[H] = 127;
		z.t_execute(0x6C);
		EXPECT_EQ(z.reg8[L],127);
		z.reg8[H] = -128;
		z.t_execute(0x6C);
		EXPECT_EQ(z.reg8[L],-128);

		// LD L,L
		z.reg8[L] = 0x20;
		z.t_execute(0x6D);
		EXPECT_EQ(z.reg8[L],0x20);
		z.reg8[L] = -20;
		z.t_execute(0x6D);
		EXPECT_EQ(z.reg8[L],-20);
		z.reg8[L] = 127;
		z.t_execute(0x6D);
		EXPECT_EQ(z.reg8[L],127);
		z.reg8[L] = -128;
		z.t_execute(0x6D);
		EXPECT_EQ(z.reg8[L],-128);

		// LD, L,(HL)
		z.memory[0x1020] = 0x20;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x6E);
		EXPECT_EQ(z.reg8[L],0x20);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x6E);
		EXPECT_EQ(z.reg8[L],-20);*/
		z.memory[0x1020] = 127;
		z.t_execute(0x6E);
		EXPECT_EQ(z.reg8[L],127);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x6E);
		EXPECT_EQ(z.reg8[L],-128);*/

		// LD L,A
		z.reg8[A] = 0x20;
		z.t_execute(0x6F);
		EXPECT_EQ(z.reg8[L],0x20);
		z.reg8[A] = -20;
		z.t_execute(0x6F);
		EXPECT_EQ(z.reg8[L],-20);
		z.reg8[A] = 127;
		z.t_execute(0x6F);
		EXPECT_EQ(z.reg8[L],127);
		z.reg8[A] = -128;
		z.t_execute(0x6F);
		EXPECT_EQ(z.reg8[L],-128);

		////

		// LD (HL), B
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.reg8[B] = 0x20;
		z.t_execute(0x70);
		EXPECT_EQ(z.memory[0x1020],0x20);
		/*z.reg8[B] = -20;
		z.t_execute(0x70);
		EXPECT_EQ(z.memory[0x1020],-20);*/
		z.reg8[B] = 127;
		z.t_execute(0x70);
		EXPECT_EQ(z.memory[0x1020],127);
		/*z.reg8[B] = -128;
		z.t_execute(0x70);
		EXPECT_EQ(z.memory[0x1020],-128);*/

		// LD (HL), C
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.reg8[C] = 0x20;
		z.t_execute(0x71);
		EXPECT_EQ(z.memory[0x1020],0x20);
		/*z.reg8[C] = -20;
		z.t_execute(0x71);
		EXPECT_EQ(z.memory[0x1020],-20);*/
		z.reg8[C] = 127;
		z.t_execute(0x71);
		EXPECT_EQ(z.memory[0x1020],127);
		/*z.reg8[C] = -128;
		z.t_execute(0x71);
		EXPECT_EQ(z.memory[0x1020],-128);*/

		// LD (HL), D
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.reg8[D] = 0x20;
		z.t_execute(0x72);
		EXPECT_EQ(z.memory[0x1020],0x20);
		/*z.reg8[D] = -20;
		z.t_execute(0x72);
		EXPECT_EQ(z.memory[0x1020],-20);*/
		z.reg8[D] = 127;
		z.t_execute(0x72);
		EXPECT_EQ(z.memory[0x1020],127);
		/*z.reg8[D] = -128;
		z.t_execute(0x72);
		EXPECT_EQ(z.memory[0x1020],-128);*/

		// LD (HL), E
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.reg8[E] = 0x20;
		z.t_execute(0x73);
		EXPECT_EQ(z.memory[0x1020],0x20);
		/*z.reg8[E] = -20;
		z.t_execute(0x73);
		EXPECT_EQ(z.memory[0x1020],-20);*/
		z.reg8[E] = 127;
		z.t_execute(0x73);
		EXPECT_EQ(z.memory[0x1020],127);
		/*z.reg8[E] = -128;
		z.t_execute(0x73);
		EXPECT_EQ(z.memory[0x1020],-128);*/

		// LD (HL), H
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x74);
		EXPECT_EQ(z.memory[0x1020],0x10);

		// LD (HL), L
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x75);
		EXPECT_EQ(z.memory[0x1020],0x20);

		// HALT
		z.t_execute(0x76);
		EXPECT_EQ(z.halt,true);

		// LD (HL), A
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.reg8[A] = 0x20;
		z.t_execute(0x77);
		EXPECT_EQ(z.memory[0x1020],0x20);
		/*z.reg8[A] = -20;
		z.t_execute(0x77);
		EXPECT_EQ(z.memory[0x1020],-20);*/
		z.reg8[A] = 127;
		z.t_execute(0x77);
		EXPECT_EQ(z.memory[0x1020],127);
		/*z.reg8[A] = -128;
		z.t_execute(0x77);
		EXPECT_EQ(z.memory[0x1020],-128);*/

		////

		// LD A,B
		z.reg8[B] = 0x20;
		z.t_execute(0x78);
		EXPECT_EQ(z.reg8[A],0x20);
		z.reg8[B] = -20;
		z.t_execute(0x78);
		EXPECT_EQ(z.reg8[A],-20);
		z.reg8[B] = 127;
		z.t_execute(0x78);
		EXPECT_EQ(z.reg8[A],127);
		z.reg8[B] = -128;
		z.t_execute(0x78);
		EXPECT_EQ(z.reg8[A],-128);

		// LD A,C
		z.reg8[C] = 0x20;
		z.t_execute(0x79);
		EXPECT_EQ(z.reg8[A],0x20);
		z.reg8[C] = -20;
		z.t_execute(0x79);
		EXPECT_EQ(z.reg8[A],-20);
		z.reg8[C] = 127;
		z.t_execute(0x79);
		EXPECT_EQ(z.reg8[A],127);
		z.reg8[C] = -128;
		z.t_execute(0x79);
		EXPECT_EQ(z.reg8[A],-128);

		// LD A,D
		z.reg8[D] = 0x20;
		z.t_execute(0x7A);
		EXPECT_EQ(z.reg8[A],0x20);
		z.reg8[D] = -20;
		z.t_execute(0x7A);
		EXPECT_EQ(z.reg8[A],-20);
		z.reg8[D] = 127;
		z.t_execute(0x7A);
		EXPECT_EQ(z.reg8[A],127);
		z.reg8[D] = -128;
		z.t_execute(0x7A);
		EXPECT_EQ(z.reg8[A],-128);

		// LD A,E
		z.reg8[E] = 0x20;
		z.t_execute(0x7B);
		EXPECT_EQ(z.reg8[A],0x20);
		z.reg8[E] = -20;
		z.t_execute(0x7B);
		EXPECT_EQ(z.reg8[A],-20);
		z.reg8[E] = 127;
		z.t_execute(0x7B);
		EXPECT_EQ(z.reg8[A],127);
		z.reg8[E] = -128;
		z.t_execute(0x7B);
		EXPECT_EQ(z.reg8[A],-128);

		// LD A,H
		z.reg8[H] = 0x20;
		z.t_execute(0x7C);
		EXPECT_EQ(z.reg8[A],0x20);
		z.reg8[H] = -20;
		z.t_execute(0x7C);
		EXPECT_EQ(z.reg8[A],-20);
		z.reg8[H] = 127;
		z.t_execute(0x7C);
		EXPECT_EQ(z.reg8[A],127);
		z.reg8[H] = -128;
		z.t_execute(0x7C);
		EXPECT_EQ(z.reg8[A],-128);

		// LD A,L
		z.reg8[L] = 0x20;
		z.t_execute(0x7D);
		EXPECT_EQ(z.reg8[A],0x20);
		z.reg8[L] = -20;
		z.t_execute(0x7D);
		EXPECT_EQ(z.reg8[A],-20);
		z.reg8[L] = 127;
		z.t_execute(0x7D);
		EXPECT_EQ(z.reg8[A],127);
		z.reg8[L] = -128;
		z.t_execute(0x7D);
		EXPECT_EQ(z.reg8[A],-128);

		// LD, A,(HL)
		z.memory[0x1020] = 0x20;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0x7E);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.memory[0x1020] = -20;
		z.t_execute(0x7E);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.memory[0x1020] = 127;
		z.t_execute(0x7E);
		EXPECT_EQ(z.reg8[A],127);
		/*z.memory[0x1020] = -128;
		z.t_execute(0x7E);
		EXPECT_EQ(z.reg8[A],-128);*/

		// LD A,A
		z.reg8[A] = 0x20;
		z.t_execute(0x7F);
		EXPECT_EQ(z.reg8[A],0x20);
		z.reg8[A] = -20;
		z.t_execute(0x7F);
		EXPECT_EQ(z.reg8[A],-20);
		z.reg8[A] = 127;
		z.t_execute(0x7F);
		EXPECT_EQ(z.reg8[A],127);
		z.reg8[A] = -128;
		z.t_execute(0x7F);
		EXPECT_EQ(z.reg8[A],-128);

		// ADD instructions
		// 1. Normal addition
		// 2. Overflow
		// 3. Zero
		// 4. Addition of negative numbers
		// 5. Addition that performs a half-carry

		// ADD A,B

		z.reg8[A] = 0x20;
		z.reg8[B] = 0x10;
		z.t_execute(0x80);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 150;
		z.reg8[B] = 150;
		z.t_execute(0x80);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = -10;
		z.reg8[B] = 10;
		z.t_execute(0x80);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -10;
		z.reg8[B] = -10;
		z.t_execute(0x80);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x80);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// ADD A,C

		z.reg8[A] = 0x20;
		z.reg8[C] = 0x10;
		z.t_execute(0x81);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 150;
		z.reg8[C] = 150;
		z.t_execute(0x81);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = -10;
		z.reg8[C] = 10;
		z.t_execute(0x81);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -10;
		z.reg8[C] = -10;
		z.t_execute(0x81);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.reg8[C] = 0x08;
		z.t_execute(0x81);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// ADD A,D

		z.reg8[A] = 0x20;
		z.reg8[D] = 0x10;
		z.t_execute(0x82);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 150;
		z.reg8[D] = 150;
		z.t_execute(0x82);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = -10;
		z.reg8[D] = 10;
		z.t_execute(0x82);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -10;
		z.reg8[D] = -10;
		z.t_execute(0x82);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.reg8[D] = 0x08;
		z.t_execute(0x82);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// ADD A,E

		z.reg8[A] = 0x20;
		z.reg8[E] = 0x10;
		z.t_execute(0x83);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 150;
		z.reg8[E] = 150;
		z.t_execute(0x83);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = -10;
		z.reg8[E] = 10;
		z.t_execute(0x83);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -10;
		z.reg8[E] = -10;
		z.t_execute(0x83);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.reg8[E] = 0x08;
		z.t_execute(0x83);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// ADD A,H

		z.reg8[A] = 0x20;
		z.reg8[H] = 0x10;
		z.t_execute(0x84);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 150;
		z.reg8[H] = 150;
		z.t_execute(0x84);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = -10;
		z.reg8[H] = 10;
		z.t_execute(0x84);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -10;
		z.reg8[H] = -10;
		z.t_execute(0x84);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.reg8[H] = 0x08;
		z.t_execute(0x84);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// ADD A,L

		z.reg8[A] = 0x20;
		z.reg8[L] = 0x10;
		z.t_execute(0x85);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 150;
		z.reg8[L] = 150;
		z.t_execute(0x85);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = -10;
		z.reg8[L] = 10;
		z.t_execute(0x85);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
	/*	z.reg8[A] = -10;
		z.reg8[L] = -10;
		z.t_execute(0x85);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.reg8[L] = 0x08;
		z.t_execute(0x85);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// ADD A,(HL)

		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.memory[0x1020] = 0x10;
		z.reg8[A] = 0x20;
		z.t_execute(0x86);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 150;
		z.memory[0x1020] = 150;
		z.t_execute(0x86);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = -10;
		z.memory[0x1020] = 10;
		z.t_execute(0x86);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -10;
		z.memory[0x1020] = -10;
		z.t_execute(0x86);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.memory[0x1020] = 0x08;
		z.t_execute(0x86);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// ADD A,A

		z.reg8[A] = 0x20;
		z.t_execute(0x87);
		EXPECT_EQ(z.reg8[A],0x40);
		z.reg8[A] = 150;
		z.t_execute(0x87);
		EXPECT_EQ(z.reg8[A],44);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);
		z.reg8[A] = 0;
		z.t_execute(0x87);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		/*z.reg8[A] = -10;
		z.t_execute(0x87);
		EXPECT_EQ(z.reg8[A],-20);*/
		z.reg8[A] = 0x08;
		z.t_execute(0x87);
		EXPECT_EQ(z.reg8[A],16);
		//EXPECT_EQ(z.reg8[F] & 0x20,0x20);

		// TODO: ADC instructions


		// Subtraction instructions
		// 1. Normal subtraction
		// 2. Underflow
		// 3. Zero
		// 4. Subtraction of negative numbers
		// 5. Subtraction that performs a half-carry

		// SUB B

		z.reg8[A] = 0x50;
		z.reg8[B] = 0x30;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.reg8[A] = 150;
		z.reg8[B] = 160;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],-10);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);*/
		z.reg8[A] = 10;
		z.reg8[B] = 10;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.reg8[B] = -20;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],10);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// SUB C

		z.reg8[A] = 0x50;
		z.reg8[C] = 0x30;
		z.t_execute(0x91);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.reg8[A] = 150;
		z.reg8[C] = 160;
		z.t_execute(0x91);
		EXPECT_EQ(z.reg8[A],-10);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);*/
		z.reg8[A] = 10;
		z.reg8[C] = 10;
		z.t_execute(0x91);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.reg8[C] = -20;
		z.t_execute(0x91);
		EXPECT_EQ(z.reg8[A],10);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// SUB D

		z.reg8[A] = 0x50;
		z.reg8[D] = 0x30;
		z.t_execute(0x92);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.reg8[A] = 150;
		z.reg8[D] = 160;
		z.t_execute(0x92);
		EXPECT_EQ(z.reg8[A],-10);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);*/
		z.reg8[A] = 10;
		z.reg8[D] = 10;
		z.t_execute(0x92);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.reg8[D] = -20;
		z.t_execute(0x92);
		EXPECT_EQ(z.reg8[A],10);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// SUB E

		z.reg8[A] = 0x50;
		z.reg8[E] = 0x30;
		z.t_execute(0x93);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.reg8[A] = 150;
		z.reg8[E] = 160;
		z.t_execute(0x93);
		EXPECT_EQ(z.reg8[A],-10);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);*/
		z.reg8[A] = 10;
		z.reg8[E] = 10;
		z.t_execute(0x93);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.reg8[E] = -20;
		z.t_execute(0x93);
		EXPECT_EQ(z.reg8[A],10);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// SUB H

		z.reg8[A] = 0x50;
		z.reg8[H] = 0x30;
		z.t_execute(0x94);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.reg8[A] = 150;
		z.reg8[H] = 160;
		z.t_execute(0x94);
		EXPECT_EQ(z.reg8[A],-10);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);*/
		z.reg8[A] = 10;
		z.reg8[H] = 10;
		z.t_execute(0x94);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.reg8[H] = -20;
		z.t_execute(0x94);
		EXPECT_EQ(z.reg8[A],10);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// SUB L

		z.reg8[A] = 0x50;
		z.reg8[L] = 0x30;
		z.t_execute(0x95);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.reg8[A] = 150;
		z.reg8[L] = 160;
		z.t_execute(0x95);
		EXPECT_EQ(z.reg8[A],-10);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);*/
		z.reg8[A] = 10;
		z.reg8[L] = 10;
		z.t_execute(0x95);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.reg8[L] = -20;
		z.t_execute(0x95);
		EXPECT_EQ(z.reg8[A],10);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// SUB (HL)

		z.reg8[A] = 0x50;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.memory[0x1020] = 0x30;
		z.t_execute(0x96);
		EXPECT_EQ(z.reg8[A],0x20);
		/*z.reg8[A] = 150;
		z.memory[0x1020] = 160;
		z.t_execute(0x96);
		EXPECT_EQ(z.reg8[A],-10);
		EXPECT_EQ(z.reg8[F] & 0x10,0x10);*/
		z.reg8[A] = 10;
		z.memory[0x1020] = 10;
		z.t_execute(0x96);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.memory[0x1020] = -20;
		z.t_execute(0x96);
		EXPECT_EQ(z.reg8[A],10);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// SUB A

		z.reg8[A] = 10;
		z.t_execute(0x97);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);
		z.reg8[A] = -10;
		z.t_execute(0x97);
		EXPECT_EQ(z.reg8[A],0);
		//TODO: Add test for half-carry flag on subtraction
		/*z.reg8[A] = 0x08;
		z.reg8[B] = 0x08;
		z.t_execute(0x90);
		EXPECT_EQ(z.reg8[A],16);
		EXPECT_EQ(z.reg8[F] & 0x20,0x20);*/

		// TODO: Subtraction with carry tests

		// AND tests
		// 1. Normal AND test
		// 2. Zero flag set test

		// AND B

		z.reg8[A] = 0x10;
		z.reg8[B] = 0x10;
		z.t_execute(0xA0);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x10;
		z.reg8[B] = 0x01;
		z.t_execute(0xA0);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// AND C

		z.reg8[A] = 0x10;
		z.reg8[C] = 0x10;
		z.t_execute(0xA1);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x10;
		z.reg8[C] = 0x01;
		z.t_execute(0xA1);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// AND D

		z.reg8[A] = 0x10;
		z.reg8[D] = 0x10;
		z.t_execute(0xA2);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x10;
		z.reg8[D] = 0x01;
		z.t_execute(0xA2);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// AND E

		z.reg8[A] = 0x10;
		z.reg8[E] = 0x10;
		z.t_execute(0xA3);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x10;
		z.reg8[E] = 0x01;
		z.t_execute(0xA3);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// AND H

		z.reg8[A] = 0x10;
		z.reg8[H] = 0x10;
		z.t_execute(0xA4);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x10;
		z.reg8[H] = 0x01;
		z.t_execute(0xA4);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// AND L

		z.reg8[A] = 0x10;
		z.reg8[L] = 0x10;
		z.t_execute(0xA5);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x10;
		z.reg8[L] = 0x01;
		z.t_execute(0xA5);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// AND (HL)

		z.reg8[A] = 0x10;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.memory[0x1020] = 0x10;
		z.t_execute(0xA6);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x10;
		z.memory[0x1020] = 0x01;
		z.t_execute(0xA6);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// AND A

		z.reg8[A] = 0x10;
		z.t_execute(0xA7);
		EXPECT_EQ(z.reg8[A],0x10);

		// XOR tests
		// 1. Normal XOR test
		// 2. XOR test that sets zero flag

		// XOR B

		z.reg8[A] = 0x10;
		z.reg8[B] = 0x20;
		z.t_execute(0xA8);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 0x10;
		z.reg8[B] = 0x10;
		z.t_execute(0xA8);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// XOR C

		z.reg8[A] = 0x10;
		z.reg8[C] = 0x20;
		z.t_execute(0xA9);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 0x10;
		z.reg8[C] = 0x10;
		z.t_execute(0xA9);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// XOR D

		z.reg8[A] = 0x10;
		z.reg8[D] = 0x20;
		z.t_execute(0xAA);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 0x10;
		z.reg8[D] = 0x10;
		z.t_execute(0xAA);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// XOR E

		z.reg8[A] = 0x10;
		z.reg8[E] = 0x20;
		z.t_execute(0xAB);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 0x10;
		z.reg8[E] = 0x10;
		z.t_execute(0xAB);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// XOR H

		z.reg8[A] = 0x10;
		z.reg8[H] = 0x20;
		z.t_execute(0xAC);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 0x10;
		z.reg8[H] = 0x10;
		z.t_execute(0xAC);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// XOR L

		z.reg8[A] = 0x10;
		z.reg8[L] = 0x20;
		z.t_execute(0xAD);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 0x10;
		z.reg8[L] = 0x10;
		z.t_execute(0xAD);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// XOR (HL)

		z.reg8[A] = 0x10;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.memory[0x1020] = 0x20;
		z.t_execute(0xAE);
		EXPECT_EQ(z.reg8[A],0x30);
		z.reg8[A] = 0x10;
		z.memory[0x1020] = 0x10;
		z.t_execute(0xAE);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// XOR A

		z.reg8[A] = 0x10;
		z.t_execute(0xAF);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR tests
		// 1. Normal OR test
		// 2. OR test that sets the zero flag

		// OR B

		z.reg8[A] = 0x10;
		z.reg8[B] = 0x01;
		z.t_execute(0xB0);
		EXPECT_EQ(z.reg8[A],0x11);
		z.reg8[A] = 0x00;
		z.reg8[B] = 0x00;
		z.t_execute(0xB0);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR C

		z.reg8[A] = 0x10;
		z.reg8[C] = 0x01;
		z.t_execute(0xB1);
		EXPECT_EQ(z.reg8[A],0x11);
		z.reg8[A] = 0x00;
		z.reg8[C] = 0x00;
		z.t_execute(0xB1);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR D

		z.reg8[A] = 0x10;
		z.reg8[D] = 0x01;
		z.t_execute(0xB2);
		EXPECT_EQ(z.reg8[A],0x11);
		z.reg8[A] = 0x00;
		z.reg8[D] = 0x00;
		z.t_execute(0xB2);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR E

		z.reg8[A] = 0x10;
		z.reg8[E] = 0x01;
		z.t_execute(0xB3);
		EXPECT_EQ(z.reg8[A],0x11);
		z.reg8[A] = 0x00;
		z.reg8[E] = 0x00;
		z.t_execute(0xB3);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR H

		z.reg8[A] = 0x10;
		z.reg8[H] = 0x01;
		z.t_execute(0xB4);
		EXPECT_EQ(z.reg8[A],0x11);
		z.reg8[A] = 0x00;
		z.reg8[H] = 0x00;
		z.t_execute(0xB4);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR L

		z.reg8[A] = 0x10;
		z.reg8[L] = 0x01;
		z.t_execute(0xB5);
		EXPECT_EQ(z.reg8[A],0x11);
		z.reg8[A] = 0x00;
		z.reg8[L] = 0x00;
		z.t_execute(0xB5);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR (HL)

		z.reg8[A] = 0x10;
		z.reg8[H] = 0x10;
		z.reg8[L] = 0x20;
		z.memory[0x1020] = 0x01;
		z.t_execute(0xB6);
		EXPECT_EQ(z.reg8[A],0x11);
		z.reg8[A] = 0x00;
		z.memory[0x1020] = 0x00;
		z.t_execute(0xB6);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// OR A

		z.reg8[A] = 0x10;
		z.t_execute(0xB7);
		EXPECT_EQ(z.reg8[A],0x10);
		z.reg8[A] = 0x00;
		z.t_execute(0xB7);
		EXPECT_EQ(z.reg8[A],0);
		EXPECT_EQ(z.reg8[F] & 0x80,0x80);

		// TODO: CP tests

	}

	TEST(GPU,UpdateTile) {
		gpu g;

		int address = 0x0000;
		g.vram[address] = 0xB4;
		g.vram[address+1] = 0x6D;

		for (int x=0; x<TILE_WIDTH; x++) {
			int sx = 1 << ((TILE_WIDTH - 1) - x);
			g.tileset[0][0][x] =
					((g.vram[address] & sx) >> ((TILE_WIDTH - 1) - x))
					+
					(((g.vram[address+1] & sx) >> ((TILE_WIDTH - 1) - x)) << 1);
		}

		EXPECT_EQ(g.tileset[0][0][0],1);
		EXPECT_EQ(g.tileset[0][0][1],2);
		EXPECT_EQ(g.tileset[0][0][2],3);
		EXPECT_EQ(g.tileset[0][0][3],1);
		EXPECT_EQ(g.tileset[0][0][4],2);
		EXPECT_EQ(g.tileset[0][0][5],3);
		EXPECT_EQ(g.tileset[0][0][6],0);
		EXPECT_EQ(g.tileset[0][0][7],2);

		address = 0x0000;
		g.vram[address] = 0x00;
		g.vram[address+1] = 0x00;

		for (int x=0; x<TILE_WIDTH; x++) {
			int sx = 1 << ((TILE_WIDTH - 1) - x);
			g.tileset[0][0][x] =
					((g.vram[address] & sx) >> ((TILE_WIDTH - 1) - x))
					+
					(((g.vram[address+1] & sx) >> ((TILE_WIDTH - 1) - x)) << 1);
		}

		EXPECT_EQ(g.tileset[0][0][0],0);
		EXPECT_EQ(g.tileset[0][0][1],0);
		EXPECT_EQ(g.tileset[0][0][2],0);
		EXPECT_EQ(g.tileset[0][0][3],0);
		EXPECT_EQ(g.tileset[0][0][4],0);
		EXPECT_EQ(g.tileset[0][0][5],0);
		EXPECT_EQ(g.tileset[0][0][6],0);
		EXPECT_EQ(g.tileset[0][0][7],0);

		address = 0x0000;
		g.vram[address] = 0xFF;
		g.vram[address+1] = 0xFF;

		for (int x=0; x<TILE_WIDTH; x++) {
			int sx = 1 << ((TILE_WIDTH - 1) - x);
			g.tileset[0][0][x] =
					((g.vram[address] & sx) >> ((TILE_WIDTH - 1) - x))
					+
					(((g.vram[address+1] & sx) >> ((TILE_WIDTH - 1) - x)) << 1);
		}

		EXPECT_EQ(g.tileset[0][0][0],3);
		EXPECT_EQ(g.tileset[0][0][1],3);
		EXPECT_EQ(g.tileset[0][0][2],3);
		EXPECT_EQ(g.tileset[0][0][3],3);
		EXPECT_EQ(g.tileset[0][0][4],3);
		EXPECT_EQ(g.tileset[0][0][5],3);
		EXPECT_EQ(g.tileset[0][0][6],3);
		EXPECT_EQ(g.tileset[0][0][7],3);

	}


int main() {
	//testing::InitGoogleTest("");
	RUN_ALL_TESTS();
	return 0;
}

