#ifndef GPU_H
#define GPU_H

#include "gtest/gtest_prod.h"

#define VRAM_SIZE 8192
#define VRAM 0x8000
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

// GPU modes
#define HBLANK 0
#define VBLANK 1
#define SCANLINE_OAM 2
#define SCANLINE_VRAM 3

#define HBLANK_CYCLES 51
#define SCANLINE_OAM_CYCLES 20
#define SCANLINE_VRAM_CYCLES 43

// Tiles
#define TILE_MAP_0 0x1800
#define TILE_MAP_1 0x1C00

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_MAX 512

// GPU registers
#define GPU_LCD 0xFF40
#define GPU_STAT 0xFF41	// TODO: Implement
#define GPU_SCY 0xFF42
#define GPU_SCX 0xFF43
#define GPU_LINE 0xFF44
#define GPU_LYC 0xFF45 // TODO: Implement
#define GPU_DMA 0xFF46 // TODO: Implement
#define GPU_PALLETE 0xFF47

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

	unsigned char tileset[TILE_MAX][TILE_WIDTH][TILE_HEIGHT]; // Internal tileset
	int vram[VRAM_SIZE];									  // 8KB of VRAM which holds signed and unsigned bytes
	unsigned char screen[SCREEN_WIDTH*SCREEN_HEIGHT];

	/*
	 * 256 8x8 unique tiles in screen
	 * Tile map 0 (-128 to 127) (signed)
	 * Tile map 1 (0 to 255) (unsigned)
	 * 255+128 = 384 unique tiles
	 */
};

#endif


