#ifndef tools
#define tools

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct Matrix_t
{
	double *matrix_data;

	int nb_column;
	
	int nb_rows;		
}Matrix;

Matrix Init_matrix(int width,int height);

void Fill_mat_rand(Matrix matrix);

void Pretty_print(Matrix matrix);

Matrix Mult_mat_1(Matrix a, Matrix b);

Matrix Sum_column(Matrix a);
#endif


