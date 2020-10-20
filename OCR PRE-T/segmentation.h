#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "tools.h"


Array histoH(Matrix matrix);

Array histoV(Matrix matrix);

float LetterSizeAverage(Array histov);

Matrix_Array Seg_Lines(Matrix matrix, Array histo);

Matrix_Array Seg_Words(Matrix line, Array histov, float average);

Matrix_Array Seg_Letters(Matrix word, Array histov);

#endif
