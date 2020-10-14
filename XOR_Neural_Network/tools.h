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

Matrix Transp_mat(Matrix a);

Matrix Sum_bias(Matrix a, Matrix b);

Matrix Multip_factor(Matrix matrix, double a);

Matrix Sum_weights(Matrix a, Matrix b);

Matrix Sigmo_mat(Matrix a);

Matrix Sigmo_mat_derivate(Matrix b);

void Fill_mat_data(Matrix a, double b[],int size);

Matrix Mult_simple(Matrix a,Matrix b);
#endif
