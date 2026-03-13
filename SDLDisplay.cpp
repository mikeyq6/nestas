#include "inc/SDLDisplay.h"

SDLDisplay::SDLDisplay(SharedData *shared_data) : Display(shared_data) {
    const char *title = "NesTas";
    xpos = 300;
    ypos = 300;
    flags = 0;
	zoom = 2;

    SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * zoom, SCREEN_HEIGHT * zoom, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | flags);
	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGB888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	tile_map_window = SDL_CreateWindow("CHR Viewer",
		100, 100, TILE_MAP_WIDTH * zoom, TILE_MAP_HEIGHT * zoom,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | flags);
	tile_map_renderer = SDL_CreateRenderer(tile_map_window, -1, 0);
	tile_map_texture = SDL_CreateTexture(tile_map_renderer,
		SDL_PIXELFORMAT_RGB888,
		SDL_TEXTUREACCESS_STREAMING,
		TILE_MAP_WIDTH, TILE_MAP_HEIGHT);
}
SDLDisplay::~SDLDisplay() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyRenderer(tile_map_renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(tile_map_texture);
}

void SDLDisplay::init() {
    fill(pixels, pixels + NUM_PIXELS, 0);
	fill(tile_map_pixels, tile_map_pixels + TILE_MAP_PIXELS, 0);
	fill(tile_map_data, tile_map_data + TILE_MAP_PIXELS, 0);
}

void SDLDisplay::draw() {
    uint8_t r, g, b;
    uint16_t val;

    quit = false;
    SDL_Event e;
    while (!quit) {
        if (SDL_PollEvent(&e) != 0) {
			// std::cout << e.type << std::endl;
			switch(e.type) {
				case SDL_QUIT: 
					quit = true;
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					process_key_event(&e);
					break;
			}
        }
        r = shared_data->get_random(255);
        g = shared_data->get_random(255);
        b = shared_data->get_random(255);
        val = shared_data->get_random(NUM_PIXELS - 1);
        pixels[val] = (r << 16) + (g << 8) + b;

		// sdata->copy_pixels_to(pixels_buffer);
		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		if(shared_data->get_show_tile_map()) {
			shared_data->copy_tile_map_pixels_to(tile_map_data);
			set_tile_map_pixels();
			SDL_UpdateTexture(tile_map_texture, NULL, tile_map_pixels, TILE_MAP_WIDTH * sizeof(uint32_t));
			SDL_RenderClear(tile_map_renderer);
			SDL_RenderCopy(tile_map_renderer, tile_map_texture, NULL, NULL);
			SDL_RenderPresent(tile_map_renderer);
			// shared_data->set_show_tile_map(false);
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DISPLAY_MS));
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}


void SDLDisplay::process_key_event(SDL_Event* event) {

	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_t:
				bool show = shared_data->get_show_tile_map();
				if(!show) {
					SDL_ShowWindow(tile_map_window);
				} else {
					SDL_HideWindow(tile_map_window);
				}
				shared_data->set_show_tile_map(!show);
				break;
			// case SDLK_v:
			// 	sdata->set_key_down_flag(0xf);
			// 	break;
		}
	} 
    // else if (event->type == SDL_KEYUP) {
	// 	switch (event->key.keysym.sym) {
	// 		case SDLK_v:
	// 			sdata->set_key_up_flag(0xf);
	// 			break;
	// 	}
	// }
}

void SDLDisplay::set_tile_map_pixels() {
	uint16_t tile_index = 0, pixel_index = 0;
	uint8_t tile_data[64];

	for(int i=0; i<NUM_TILE_MAP_TILES; i++) { // for each tile (16 bytes)
		pixel_index = ((i / 0x20) * TILE_MAP_WIDTH * 8) + ((i % 0x20) * 0x8);
		get_tile_data(tile_data, tile_map_data, i);

		for(int j=0; j<0x8; j++) {
			for(int k=0; k<0x8; k++) {
				tile_index = (j * 0x8) + k;
				tile_map_pixels[pixel_index + k] = PALETTE_COLOURS[tile_data[tile_index] * 0x10];
			}
			pixel_index += TILE_MAP_WIDTH;
		}
	}
}

void SDLDisplay::get_tile_data(uint8_t *tile, uint8_t *tile_map_data, uint8_t tile_number) {
	uint16_t index = tile_number * 0x10;
	uint8_t r1, r2, bit1, bit2, tile_index = 0;
	for(int j=0; j<0x8; j++) {
		tile_index = j * 0x8;

		r1 = tile_map_data[index];
		r2 = tile_map_data[index + 0x8];
		for(int i=7; i>=0; i--) {
			bit1 = (r1 >> i) & 0x1;
			bit2 = (r2 >> i) & 0x1;
			tile[tile_index + (7 - i)] = (bit2 << 1) | bit1;
		}
	}
}