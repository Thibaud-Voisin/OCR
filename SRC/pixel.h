#ifndef PIXEL_H

#define PIXEL_H

#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);


#endif
