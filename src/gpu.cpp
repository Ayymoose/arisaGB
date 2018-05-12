#include <cstring>
#include <cstdio>
#include <iostream>

#include "gpu.hpp"

void gpu::dump_tilemaps() {

	printf("Tile map 0\n{");
	for (int i=0; i<1024; i++) {
		printf("0x%02X, ",vram[0x1800 + i]);
		if (i % 32 == 0) {
			printf("\n");
		}
	}
	printf("}\n");
	printf("Tile map 1\n{");
	for (int i=0; i<1024; i++) {
		printf("0x%02X, ",vram[0x1C00 + i]);
		if (i % 32 == 0) {
			printf("\n");
		}
	}
	printf("}\n");
}

void gpu::dump_screen() {
	std::cout << "{";
	for (int y=0; y<GPU::SCREEN_HEIGHT; y++) {
		for (int x=0; x<GPU::SCREEN_WIDTH; x++) {
			printf("0x%02X, ",(int)screen[GPU::SCREEN_WIDTH * y + x]);
		}
		std::cout << std::endl;
	}
	printf("}");
}

void gpu::dump_vram() {
	printf("{");
	for (int i=0; i<GPU::VRAM_SIZE; i++) {
		printf("0x%02X, ",vram[i]);
		if (i % 32 == 0) {
			printf("\n");
		}
	}
	printf("}");
}

void gpu::print_tile(int tile) {
	for (int y=0; y<GPU::TILE_HEIGHT; y++) {
		for (int x=0; x<GPU::TILE_HEIGHT; x++) {
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
	case GPU::GPU_LCD:
		return (bg ? 0x01 : 0x00) | (bg_map ? 0x08 : 0x00) | (bg_tile ? 0x10 : 0x00) | (lcd ? 0x80 : 0x00);
		break;
	case GPU::GPU_STAT:
		return stat;
		break;
	case GPU::GPU_SCY:
		return scy;
		break;
	case GPU::GPU_SCX:
		return scx;
		break;
	case GPU::GPU_LINE:
		return line;
		break;
	default:
		fprintf(stderr,"GPU register read at 0x%04X\n",address);
		return 0;
	}
}

void gpu::write_byte(int address, int byte) {
	switch (address) {
	case GPU::GPU_LCD:
		bg = (byte & 0x01) ? 1 : 0;
		bg_map = (byte & 0x08) ? 1 : 0;
		bg_tile = (byte & 0x10) ? 1 : 0;
		lcd = (byte & 0x80) ? 1 : 0;
		break;
	case GPU::GPU_STAT:
		stat = byte;
		break;
	case GPU::GPU_SCY:
		scy = byte;
		break;
	case GPU::GPU_SCX:
		scx = byte;
		break;
	case GPU::GPU_PALLETE:
		//TODO: Implement pallete


        for (int i = 0; i < 4; i++) {
            switch ((byte >> (i * 2)) & 3) {
                case 0:
                	bg_pallete[i] = 255;
                    break;
                case 1:
                	bg_pallete[i] = 192;
                    break;
                case 2:
                	bg_pallete[i] = 96;
                    break;
                case 3:
                	bg_pallete[i] = 0;
                    break;
            }
        }

		fprintf(stderr,"Pallete not done!\n");
		break;
	default: fprintf(stderr,"[GPU] GPU register write at 0x%04X\n",address);
	}
}

void gpu::reset() {
	mode = GPU::SCANLINE_OAM;
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

	bg_pallete[0] = 255;
	bg_pallete[1] = 255;
	bg_pallete[2] = 255;
	bg_pallete[3] = 255;

	// Clear screen
	memset(screen,0x00,GPU::SCREEN_WIDTH*GPU::SCREEN_HEIGHT);

	// Clear GPU::VRAM
	memset(vram,0x00,sizeof(int) * GPU::VRAM_SIZE);

	// Clear tile set
	for (int tile=0; tile<GPU::TILE_MAX; tile++) {
		for (int y=0; y<GPU::TILE_HEIGHT; y++) {
			for (int x=0; x<GPU::TILE_WIDTH; x++) {
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
	// Clamp value to GPU::TILE_MAX-1
	int tile = (address >> 4) & 0x01FF;

	// Find the row to update
	int y = (address >> 1) & (GPU::TILE_HEIGHT - 1);

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
	for (int x=0; x<GPU::TILE_WIDTH; x++) {
		int sx = 1 << ((GPU::TILE_WIDTH - 1) - x);
		tileset[tile][y][x] = ((vram[address] & sx) >> ((GPU::TILE_WIDTH - 1) - x)) + (((vram[address+1] & sx) >> ((GPU::TILE_WIDTH - 1) - x)) << 1);
	}
}

// TODO: Pallete missing
// Renders a single scan line from the tile map
void gpu::render_scanline() {
	int map;
	// Select the tile map to draw from
	if (bg == 0) {
		map = GPU::TILE_MAP_0;
	} else {
		map = GPU::TILE_MAP_1;
	}

	// Rendering a scanline
	// 1. Find the index of the tile to render inside the current screen
	// 2. Tile map is at 0x9800 or 0x9C00 of GPU::VRAM in the memory map

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
	int tile_y = (line + scy) & (GPU::TILE_HEIGHT - 1);
	int tile_x = scx & (GPU::TILE_WIDTH - 1);

	for (int x=0; x<GPU::SCREEN_WIDTH; x++) {

		// Get the 2-bit colour of the current "pixel" in the tile row
		// [XX,XX,XX,XX,XX,XX,XX,XX]
		unsigned char pixel = tileset[tile][tile_y][tile_x];

		// TODO: Run through pallete

		// Draw pixel of current tile to screen
		screen[GPU::SCREEN_WIDTH * line + x] = bg_pallete[pixel];

		// Draw next pixel until a complete tile row is drawn
		tile_x++;
		if (tile_x == GPU::TILE_WIDTH) {
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
	case (GPU::HBLANK):
			if (clock >= GPU::HBLANK_CYCLES) {
				clock = 0;
				line++;
				if (line == GPU::SCREEN_HEIGHT) {
					mode = GPU::VBLANK;
					// Render frame
					frame_ready = true;
				} else {
					mode = GPU::SCANLINE_OAM;
				}
			}
	break;
	case (GPU::VBLANK):
		if (clock >= 114) {
			clock = 0;
			line++;
			if (line > 153) {
				mode = GPU::SCANLINE_OAM;
				line = 0;
			}
		}
	break;
	case (GPU::SCANLINE_OAM):
		if (clock >= GPU::SCANLINE_OAM_CYCLES) {
			clock = 0;
			mode = GPU::SCANLINE_VRAM;
		}
	break;
	case (GPU::SCANLINE_VRAM):
			if (clock >= GPU::SCANLINE_VRAM_CYCLES) {
				clock = 0;
				mode = GPU::HBLANK;
				render_scanline();
			}
	break;
	}
}
