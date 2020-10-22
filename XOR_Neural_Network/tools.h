#ifndef tools
#define tools

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Matrix_t
{
	double *matrix_data;

	int nb_column;
	
	int nb_rows;		
}Matrix;

Matrix Init_matrix(int width,int height);

void Fill_mat_rand(Matrix matrix);

void Pretty_print(Matrix matrix);

void Mult_mat_1(Matrix a, Matrix b, Matrix res);

void Sum_column(Matrix a, Matrix res);

void Transp_mat(Matrix a, Matrix res);

void Sum_bias(Matrix a, Matrix b, Matrix res);

void Multip_factor(Matrix matrix, double a, Matrix res);

void Sum_weights(Matrix a, Matrix b, Matrix res);

void Sigmo_mat(Matrix a, Matrix res);

void Sigmo_mat_derivate(Matrix b, Matrix res);

void Fill_mat_data(Matrix a, double b[],int size);

void Mult_simple(Matrix a,Matrix b, Matrix res);
#endif
