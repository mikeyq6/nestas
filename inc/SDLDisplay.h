#pragma once

#include <thread>
#include <vector>

#include <SDL2/SDL.h>

#include "Display.h"

using std::fill;

class SDLDisplay : public Display
{
public:
    SDLDisplay(SharedData *shared_data);
    ~SDLDisplay();

    void draw() override;
    void init() override;

private:
    bool quit;
    int xpos, ypos, flags, zoom;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

    void process_key_event(SDL_Event* event);
};