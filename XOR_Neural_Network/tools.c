#include "tools.h"


Matrix Init_matrix(int width,int height)
{
	Matrix matrix;

	matrix.matrix_data = calloc(width*height,sizeof(double));

	matrix.nb_column = width;

	matrix.nb_rows = height;

	return matrix;
}

void Fill_mat_rand(Matrix matrix)
{
	int max_value = sizeof(double)*matrix.nb_column*matrix.nb_rows;
		
	for (int i = 0; i < max_value; ++i)
	{
		double rd = (double) rand() / RAND_MAX;
		*(matrix.matrix_data+i) = rd;	
	}
}


void Pretty_print(Matrix matrix)
{
	int width_pretty = 1+(7*matrix.nb_column);
	char top[width_pretty];
	
	for(int i = 0;i<width_pretty;++i)
		top[i] = '-';

	printf("%s\n",top);

	char middle[width_pretty];
	int j = 0;
	char tmp[15];

	for(int i = 0;i<matrix.nb_column * matrix.nb_rows;++i)
	{	
		sprintf(tmp,"%.4f",matrix.matrix_data[i]);
		for (int k = 0;k<6;++k)
		{
			middle[j+k] = tmp[k];
		}
		middle[j+6] = '|';
		j+=7;

		if (i%matrix.nb_column == matrix.nb_column-1)
		{
			printf("|%s\n",middle);
			j = 0;
		}
		
	}
	printf("%s\n",top);
}

Matrix Mult_mat_1(Matrix a, Matrix b)
{
	if (a.nb_column != b.nb_rows)
	{
		printf("Incompatible Matrices for multiplication");
		exit(1);
	}
	
	Matrix res = Init_matrix(b.nb_column,a.nb_rows);

	for(int i = 0;i < b.nb_column*a.nb_rows;++i)
	{
		double tmp = 0;
		
		for(int j = 0;j<a.nb_column;++j)
			tmp += a.matrix_data[(i / res.nb_column + j)]*b.matrix_data[(i % res.nb_column + j*res.nb_column)];
		
		res.matrix_data[i]=tmp;
	}
	
	return res;
}
