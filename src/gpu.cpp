#include <cstring>
#include "gpu.hpp"

gpu::gpu() {
	reset();
}

void gpu::reset() {
	mode = 0;
	line = 0;
	clock = 0;
	scx = 0;
	scy = 0;
	bg = 0;
	// Clear VRAM
	memset(vram,0x00,sizeof(int) * VRAM_SIZE);

	// Clear tile set
	for (int tile=0; tile<TILE_MAX; tile++) {
		for (int y=0; y<TILE_HEIGHT; y++) {
			for (int x=0; x<TILE_WIDTH; x++) {
				tileset[tile][y][x] = 0x00;
			}
		}
	}
}

void gpu::clear_screen(int colour) {
	memset(screen,colour,SCREEN_WIDTH*SCREEN_HEIGHT);
}

void gpu::update_tile(int address, int value) {

	// Get base address
	address &= 0x1FEE;

	// Find the tile index in the set
	int tile = (address >> 4) & 0x01FF;

	// Find the row to update
	int y = (address >> 1) & (TILE_HEIGHT - 1);

	// A single row of a tile is stored in 2 bytes
	// Low bit of pixel data is in first byte
	// High bit of pixel data is in second byte
	// Loop over the row and calculate the pixel value

	for (int x=0; x<TILE_WIDTH; x++) {
		int sx = 1 << ((TILE_WIDTH - 1) - x);
		tileset[tile][y][x] =
				((vram[address] & sx) >> ((TILE_WIDTH - 1) - x))
				+
				(((vram[address+1] & sx) >> ((TILE_WIDTH - 1) - x)) << 1);
	}
}

void gpu::render() {
	int map;
	// Select the tile map to draw from
	if (bg == 0) {
		map = TILE_MAP_0;
	} else {
		map = TILE_MAP_1;
	}

	// Tile index used is based on
	// SCX register
	// SCY register
	// Current line
	// ANDing with 0xFF allows the y index to wrap
	// Same with x index
	int tile_index = ((((line + scy) & 0xFF) >> 3) << 5) + ((scx >> 3) & 0x1F);

	// Grab a tile from the current map
	int tile = vram[VRAM + tile_index];

	// Get the row and column of the tile to draw from
	int tile_y = (line + scy) & (TILE_HEIGHT - 1);
	int tile_x = scx & (TILE_WIDTH - 1);

	// Get the colour of the current "pixel" in the tile
	int tile_colour = tileset[tile_index][tile_y][tile_x];
}

void gpu::step(int m_cycle) {
	clock += m_cycle;
	switch (mode) {
	case (HBLANK):
			if (clock >= HBLANK_CYCLES) {
				clock = 0;
				line++;
				if (line == SCREEN_HEIGHT) {
					mode = VBLANK;
					// TODO: Write data to screen
				} else {
					mode = SCANLINE_OAM;
				}
			}
	break;
	case (VBLANK):
		if (clock >= 456) {
			clock = 0;
			line++;
			if (line > 153) {
				mode = SCANLINE_OAM;
				line = 0;
			}
		}
	break;
	case (SCANLINE_OAM):
		if (clock >= SCANLINE_OAM_CYCLES) {
			clock = 0;
			mode = SCANLINE_VRAM;
		}
	break;
	case (SCANLINE_VRAM):
			if (clock >= SCANLINE_VRAM_CYCLES) {
				clock = 0;
				mode = HBLANK;
				render();
			}
	break;
	}
}
