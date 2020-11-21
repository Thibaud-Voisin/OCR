#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "tools.h"
#include "SDL2/SDL.h"
#include "pixel.h"
#include "display.h"
#include "pre_processing.h"

Array histoH(Matrix matrix);

Array histoV(Matrix matrix);

float LetterSizeAverage(Array histov);

Matrix_Array Seg_Lines(Matrix matrix, Array histo, SDL_Surface *image, Array LinesIndex);

Matrix_Array Seg_Words(Matrix line, Array histov, float average, SDL_Surface *image, int index, Array WordsIndex);

Matrix_Array Seg_Letters(Matrix word, Array histov, SDL_Surface *image, int index, int index2);

Matrix CutEdges(Matrix letter);

Matrix Resize(Matrix letter);

void Segmentation(Matrix matrix, SDL_Surface *image, SDL_Texture *texture, SDL_Renderer *renderer);

char RandomLetter();

#endif
