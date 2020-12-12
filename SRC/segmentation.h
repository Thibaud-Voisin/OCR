#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "tools.h"
#include "SDL2/SDL.h"
#include "pixel.h"
#include "pre_processing.h"
#include "display.h"


Array histoH(Matrix matrix);

Array histoV(Matrix matrix);

float LetterSizeAverage(Array histov);

Matrix_Array Seg_Lines(Matrix matrix, Array histo, SDL_Surface *image, Array LinesIndex, GtkToggleButton *display);

Matrix_Array Seg_Words(Matrix line, Array histov, float average, SDL_Surface *image, int index, Array WordsIndex, GtkToggleButton *display);

Matrix_Array Seg_Letters(Matrix word, Array histov, SDL_Surface *image, int index, int index2, GtkToggleButton *display);

Matrix CutEdges(Matrix letter);

Matrix Resize(Matrix letter);

gchar* Segmentation(Matrix matrix, SDL_Surface *image, gchar *txt, app_widgets *app_wdgts);

Matrix_Array Segmentation2(Matrix matrix, SDL_Surface *image, app_widgets *app_wdgts);

Matrix_Array PropagationFix(Matrix_Array letters, Matrix letter, int* counter, int* nbLetters);

char RandomLetter();

#endif
