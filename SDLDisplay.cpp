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
}
SDLDisplay::~SDLDisplay() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
}

void SDLDisplay::init() {
    fill(pixels, pixels + NUM_PIXELS, 0);
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