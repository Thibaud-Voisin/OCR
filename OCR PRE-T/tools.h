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


typedef struct Array_t
{
	int *array_data;

	int size;

}Array;

Array Init_Array(int size);

void Pretty_print_array(Array array);

typedef struct Matrix_Array_t
{

	Matrix *array_data;

	int size;
}Matrix_Array;

Matrix_Array Init_Matrix_Array(int size);

typedef struct Array_Array_t
{
	Array *array_data;
	
	int size;
}Array_Array;

Array_Array Init_Array_Array(int size);

#endif


