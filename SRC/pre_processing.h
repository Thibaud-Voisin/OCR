#ifndef PRE_PROCESSING_H
#define PRE_PROCESSING_H	

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "pixel.h"
#include "tools.h"

Matrix binarize_image(SDL_Surface *image);

void grayscale(SDL_Surface *image);

void blackwhite(SDL_Surface *image);

void contrast(SDL_Surface *image);

void noise_reduction(SDL_Surface *image);

void image_rotation(SDL_Surface *image, double angle);

#endif
