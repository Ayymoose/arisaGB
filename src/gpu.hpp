#ifndef GPU_H
#define GPU_H

#define VRAM_SIZE 8192
#define VRAM 0x8000
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

// GPU modes
#define HBLANK 0
#define VBLANK 1
#define SCANLINE_OAM 2
#define SCANLINE_VRAM 3

#define HBLANK_CYCLES 204
#define SCANLINE_OAM_CYCLES 80
#define SCANLINE_VRAM_CYCLES 172

// Tiles
#define TILE_MAP_0 0x9800
#define TILE_MAP_1 0x9C00

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_MAX 384

class gpu {

public:
	gpu();
	void clear_screen(int colour);
	void step(int m_cycle);
	void reset();
	void update_tile(int address, int value);
private:
	void render();

	friend class z80;

	int mode;	// Current GPU mode
	int line;	// Current scanline
	int clock;	// Internal clock
	int scx;	// Scroll register X
	int scy;	// Scroll register Y
	int bg;		// Background pallete register

	unsigned char tileset[TILE_MAX][TILE_WIDTH][TILE_HEIGHT]; // Internal tileset
	int vram[VRAM_SIZE];	//8KB of VRAM which holds signed and unsigned bytes
	unsigned char screen[SCREEN_WIDTH*SCREEN_HEIGHT];

	/*
	 * 256 8x8 unique tiles in map
	 * Tile map 0 (-128 to 127)
	 * Tile map 1 (0 to 255)
	 * Each map holds maximum 32x32 tiles
	 */
};

#endif
