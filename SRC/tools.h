#ifndef tools
#define tools

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include "SDL2/SDL.h"
#include "pixel.h"

typedef struct Matrix_t
{
	double *matrix_data;

	int nb_column;
	
	int nb_rows;		
}Matrix;

Matrix Init_matrix(int width,int height);

void Fill_mat_rand(Matrix matrix);

void Pretty_print(Matrix matrix);

void Pretty_print_xor(Matrix matrix);

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

void wait_key_pressed();

void freeArrays(Matrix_Array array);

void CopyMatrix(Matrix matrix, Matrix copy, int a, int b);

int CountWords(Array histov, float average);

int CountLetters(Array histov);

int CompareMatrix(Matrix a, Matrix b);

void load_data(char *str, Matrix hidden_weight, Matrix output_weight, Matrix hidden_bias, Matrix output_bias);

void save_data(Matrix a, Matrix b, Matrix c, Matrix d);

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
