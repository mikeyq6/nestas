#pragma once

// CPU Constants
constexpr auto MEMORY_SIZE = 0x800;

// PPU Constants
constexpr auto VRAM_SIZE = 0x800;
constexpr auto NUM_DOTS = 341;
constexpr auto NUM_SCANLINES = 262;
constexpr auto VISIBLE_SCANLINES = 240;
constexpr auto OAM_SIZE = 64;
constexpr auto OAM_BUFFER_SIZE = 256;
constexpr auto MAX_SPRITES = 8;
constexpr auto EXT_ADDRESS = 0x3f00;
constexpr auto PALETTE_SIZE = 0x20;
constexpr auto TILES_IN_SCANLINE = 0x20;

// Display Constants
constexpr auto SCREEN_WIDTH = 256;
constexpr auto SCREEN_HEIGHT = 240;
constexpr auto NUM_PIXELS = SCREEN_WIDTH * SCREEN_HEIGHT;
constexpr auto SLEEP_DISPLAY_MS = 30;
constexpr auto NUM_TILE_MAP_TILES = 0x200;
constexpr auto TILE_MAP_DATA_SIZE = 0x2000;
constexpr auto TILE_MAP_WIDTH = 0x100;
constexpr auto TILE_MAP_HEIGHT = 0x80;
constexpr auto TILE_MAP_PIXELS = TILE_MAP_WIDTH * TILE_MAP_HEIGHT;