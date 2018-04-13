#include <cstring>
#include <cstdio>
#include <iostream>

#include "gpu.hpp"

void gpu::dump_tilemaps() {

	printf("Tile map 0\n{");
	for (int i=0; i<1024; i++) {
		printf("0x%02X, ",vram[TILE_MAP_0 + i]);
		if (i % 32 == 0) {
			printf("\n");
		}
	}
	printf("}\n");
	printf("Tile map 1\n{");
	for (int i=0; i<1024; i++) {
		printf("0x%02X, ",vram[TILE_MAP_1 + i]);
		if (i % 32 == 0) {
			printf("\n");
		}
	}
	printf("}\n");
}

void gpu::dump_screen() {
	std::cout << "{";
	for (int y=0; y<SCREEN_HEIGHT; y++) {
		for (int x=0; x<SCREEN_WIDTH; x++) {
			printf("0x%02X, ",(int)screen[SCREEN_WIDTH * y + x]);
		}
		std::cout << std::endl;
	}
	printf("}");
}

void gpu::dump_vram() {
	printf("{");
	for (int i=0; i<VRAM_SIZE; i++) {
		printf("0x%02X, ",vram[i]);
		if (i % 128 == 0) {
			printf("\n");
		}
	}
	printf("}");
}

void gpu::print_tile(int tile) {
	for (int y=0; y<TILE_HEIGHT; y++) {
		for (int x=0; x<TILE_HEIGHT; x++) {
			printf("0x%02X, ",tileset[tile][y][x]);
		}
		printf("\n");
	}
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
		return stat;
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
		stat = byte;
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
	frame_ready = false;
	stat = 0;

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

// Updates a single row in a tile in the tileset
void gpu::update_tile(int address) {

	// If the address is odd update the even address row
	if (address & 0x001) {
		address--;
	}

	// Find the tile index in the set
	// Clamp value to TILE_MAX-1
	int tile = (address >> 4) & 0x01FF;

	// Find the row to update
	int y = (address >> 1) & (TILE_HEIGHT - 1);

	// A single row of a tile is stored in 2 bytes
	// 1 byte represents half a row
	// 0x8000 [XX,XX,XX,XX] 0x8001 [XX,XX,XX,XX]
	// 0x8002 [XX,XX,XX,XX] 0x8003 [XX,XX,XX,XX]
	// 0x8004 [XX,XX,XX,XX] 0x8005 [XX,XX,XX,XX]
	// ...
	//

	// Low bit of pixel data is in first byte
	// High bit of pixel data is in second byte

	// Loop over the row and calculate the pixel value
	// FIXME: Change to calculate value of entire row using bit manipulation
	for (int x=0; x<TILE_WIDTH; x++) {
		int sx = 1 << ((TILE_WIDTH - 1) - x);
		tileset[tile][y][x] = ((vram[address] & sx) >> ((TILE_WIDTH - 1) - x)) + (((vram[address+1] & sx) >> ((TILE_WIDTH - 1) - x)) << 1);
	}
}

// TODO: Pallete missing
// Renders a single scan line from the tile map
void gpu::render_scanline() {
	int map;
	// Select the tile map to draw from
	if (bg == 0) {
		map = TILE_MAP_0;
	} else {
		map = TILE_MAP_1;
	}

	// Rendering a scanline
	// 1. Find the index of the tile to render inside the current screen
	// 2. Tile map is at 0x9800 or 0x9C00 of VRAM in the memory map

	// Tile index is calculated using
	// SCX register
	// SCY register
	// Current line

	// ANDing with 0xFF allows the y index to wrap
	// Same with x index

	int tile_x_offset = (scx >> 3) & 0x1F;
	int tile_y_offset = (((line + scy) & 0xFF) >> 3) << 5;
	int tile_index = tile_y_offset + tile_x_offset;

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
		unsigned char pixel = tileset[tile][tile_y][tile_x];

		// TODO: Run through pallete

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
					frame_ready = true;
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
