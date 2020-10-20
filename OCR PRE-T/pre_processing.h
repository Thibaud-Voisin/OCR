#ifndef PRE_PROCESSING_H
#define PRE_PROCESSING_H	

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "pixel.h"
#include "tools.h"

Matrix binarize_image(SDL_Surface *image);

SDL_Surface* grayscale(SDL_Surface *image);

SDL_Surface* blackwhite(SDL_Surface *image);

SDL_Surface* contrast(SDL_Surface *image);

SDL_Surface* noise_reduction(SDL_Surface *image);

#endif
