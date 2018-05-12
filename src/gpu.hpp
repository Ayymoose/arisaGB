#ifndef GPU_H
#define GPU_H

#include "gtest/gtest_prod.h"

enum GPU {
	VRAM_SIZE = 8192,
	SCREEN_WIDTH = 160,
	SCREEN_HEIGHT = 144,

	// Registers
	GPU_LCD = 0xFF40,
	GPU_STAT = 0xFF41,		// TODO: Implement
	GPU_SCY = 0xFF42,
	GPU_SCX = 0xFF43,
	GPU_LINE = 0xFF44,
	GPU_LYC = 0xFF45, 		// TODO: Implement
	GPU_DMA = 0xFF46, 		// TODO: Implement
	GPU_PALLETE = 0xFF47,

	// Modes
	HBLANK = 0,
	VBLANK = 1,
	SCANLINE_OAM = 2,
	SCANLINE_VRAM = 3,
	HBLANK_CYCLES = 51,
	SCANLINE_OAM_CYCLES = 20,
	SCANLINE_VRAM_CYCLES = 43,

	// Tiles
	TILE_MAP_0 = 0x1800,
	TILE_MAP_1 = 0x1C00,
	TILE_WIDTH = 8,
	TILE_HEIGHT = 8,
	TILE_MAX = 512
};


class gpu {

	public:
		gpu();
		void step(int m_cycle);
		void reset();
		void update_tile(int address/*, int value*/);
		int read_byte(int address);
		void write_byte(int address, int byte);


		void dump_screen();
		void dump_vram();
		void dump_tilemaps();
		void print_tile(int tile);

	private:
		FRIEND_TEST(GPU, UpdateTile);
		FRIEND_TEST(GPUMemory,ROM);
		//FRIEND_TEST(GPU,MemoryBadWrite);

		friend class z80;
		friend class gb;

		// Writes a single scanline to the frame buffer
		void render_scanline();

		// Makes a full frame ready to be displayed
		bool frame_ready;
		void push_frame();

		int mode;	// Current GPU mode
		int clock;	// Internal clock

		// GPU registers
		int scx;		// Scroll register X
		int scy;		// Scroll register Y
		int bg_map;		// Background map register
		int bg_tile;	// Background tile register
		int line;		// Current scanline
		int pallete;	// Pallete register
		int bg;			// Background register
		int lcd;		// LCD register
		int stat;		// STAT register


		int bg_pallete[4];

		unsigned char tileset[GPU::TILE_MAX][GPU::TILE_WIDTH][GPU::TILE_HEIGHT]; // Internal tileset
		int vram[GPU::VRAM_SIZE];									  // 8KB of VRAM which holds signed and unsigned bytes
		unsigned char screen[GPU::SCREEN_WIDTH*GPU::SCREEN_HEIGHT];

		/*
		 * 256 8x8 unique tiles in screen
		 * Tile map 0 (-128 to 127) (signed)
		 * Tile map 1 (0 to 255) (unsigned)
		 * 255+128 = 384 unique tiles
		 */
};

#endif


