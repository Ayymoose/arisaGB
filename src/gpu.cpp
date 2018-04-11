#include <cstring>
#include <cstdio>
#include <iostream>

#include "gpu.hpp"

// FIXME: Array out of bounds access
void gpu::dump_screen() {
	std::cout << "{";
	for (int y=0; y<SCREEN_HEIGHT; y++) {
		for (int x=0; x<SCREEN_WIDTH; x++) {
			std::cout << (int)screen[SCREEN_WIDTH * y + x] << ",";
		}
		std::cout << std::endl;
	}
	std::cout << "}" << std::endl;
}


gpu::gpu() {
	reset();
}

int gpu::read_byte(int address) {
	switch (address) {
	case GPU_LCD:
		return (bg ? 0x01 : 0x00) | (bg_map ? 0x08 : 0x00) | (bg_tile ? 0x10 : 0x00) | (lcd ? 0x80 : 0x00);
		break;
	case GPU_STAT:
		fprintf(stderr,"GPU STAT register read\n");
		return 0;
		break;
	case GPU_SCY:
		return scy;
		break;
	case GPU_SCX:
		return scx;
		break;
	case GPU_LINE:
		return line;
		break;
	default:
		fprintf(stderr,"GPU register read at 0x%04X\n",address);
		return 0;
	}
}

void gpu::write_byte(int address, int byte) {
	switch (address) {
	case GPU_LCD:
		bg = (byte & 0x01) ? 1 : 0;
		bg_map = (byte & 0x08) ? 1 : 0;
		bg_tile = (byte & 0x10) ? 1 : 0;
		lcd = (byte & 0x80) ? 1 : 0;
		break;
	case GPU_STAT:
		fprintf(stderr,"GPU STAT register write\n");
		break;
	case GPU_SCY:
		scy = byte;
		break;
	case GPU_SCX:
		scx = byte;
		break;
	case GPU_PALLETE:
		//TODO: Implement pallete
		fprintf(stderr,"Pallete not done!\n");
		//throw "Pallete not done!";
		break;
	default: fprintf(stderr,"[GPU] GPU register write at 0x%04X\n",address);
	}
}

void gpu::reset() {
	mode = SCANLINE_OAM;
	clock = 0;
	scx = 0;
	scy = 0;
	bg_map = 0;
	bg_tile = 0;
	line = 0;
	pallete = 0;
	bg = 0;
	lcd = 0;

	// Clear screen
	memset(screen,0x00,SCREEN_WIDTH*SCREEN_HEIGHT);

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

// TODO: Test
void gpu::update_tile(int address) {

	// Get base address
	//address &= 0x1FFE;

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
void gpu::push_frame() {
	fprintf(stderr,"Frame ready!\n");
}

// TODO: Complete and test
// Renders a single scan line
void gpu::render_scanline() {

	//fprintf(stderr,"Rendering scanline!\n");

	int map;
	// Select the tile map to draw from
	if (bg == 0) {
		map = TILE_MAP_0;
	} else {
		map = TILE_MAP_1;
	}

	// Tile index is calculated using
	// SCX register
	// SCY register
	// Current line

	// ANDing with 0xFF allows the y index to wrap
	// Same with x index

	int tile_x_offset = (scx >> 3);
	int tile_index = ((((line + scy) & 0xFF) >> 3) << 5) + (tile_x_offset & 0x1F);

	// Grab a tile from the current map
	int tile = vram[map + tile_index];

	// NOTE: I don't understand this?
	if (bg_tile == 1 && tile < 128) {
		tile += 256;
	}

	// Get the row and column of the tile to draw from
	int tile_y = (line + scy) & (TILE_HEIGHT - 1);
	int tile_x = scx & (TILE_WIDTH - 1);

	for (int x=0; x<SCREEN_WIDTH; x++) {

		// Get the 2-bit colour of the current "pixel" in the tile row
		// [XX,XX,XX,XX,XX,XX,XX,XX]
		unsigned char pixel = tileset[tile_index][tile_y][tile_x];

		// Draw pixel of current tile to screen
		screen[SCREEN_WIDTH * line + x] = pixel;

		// Draw next pixel until a complete tile row is drawn
		tile_x++;
		if (tile_x == TILE_WIDTH) {
			tile_x  = 0;
			tile_x_offset = (tile_x_offset + 1) & 0x1F;

			// Grab the next tile
			tile = vram[map + tile_x_offset];

			// NOTE: I don't understand this?
			if (bg_tile == 1 && tile < 128) {
				tile += 256;
			}
		}

	}
}

// TODO: Test
// NOTE: ALL CLOCKS TAKE USE M CYCLES
void gpu::step(int m_cycle) {
	clock += m_cycle;
	switch (mode) {
	case (HBLANK):
			if (clock >= HBLANK_CYCLES) {
				clock = 0;
				line++;
				if (line == SCREEN_HEIGHT) {
					mode = VBLANK;
					push_frame();
				} else {
					mode = SCANLINE_OAM;
				}
			}
	break;
	case (VBLANK):
		if (clock >= 114) {
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
				render_scanline();
			}
	break;
	}
}
