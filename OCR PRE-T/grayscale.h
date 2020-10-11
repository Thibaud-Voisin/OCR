#ifndef GSL_H
#define GSL_H	

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"


SDL_Surface* grayscale(SDL_Surface *image);

SDL_Surface* blackwhite(SDL_Surface *image);


#endif
