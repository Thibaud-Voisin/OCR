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
		printf("Incompatible Matrices for multiplication\n");
		exit(-1);
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

Matrix Sum_column(Matrix matrix)
{
	Matrix res = Init_matrix(matrix.nb_column,1);
	
	for(int i = 0; i < matrix.nb_rows*matrix.nb_column; ++i)
	{
		res.matrix_data[i%matrix.nb_column] += matrix.matrix_data[i];
	}

	return res;
}

Matrix Transp_mat(Matrix matrix)
{
	Matrix res = Init_matrix(matrix.nb_rows,matrix.nb_column);

	for(int i = 0; i < res.nb_rows * res.nb_column; ++i)
	{
		res.matrix_data[i] = matrix.matrix_data[(i / res.nb_column) + ((i % res.nb_column) * res.nb_rows)];
	}
	
	return res;
}

Matrix Sum_bias(Matrix a,Matrix b)
{
	
	if((a.nb_column != b.nb_column) || (b.nb_rows != 1))
	{
		printf("Incompatible matrix in Sum_bias\n");
		exit(-1);
	}
	

	Matrix res = Init_matrix(a.nb_column,a.nb_rows);
	
	for(int i = 0; i < a.nb_column * a.nb_rows;++i)
	{
		res.matrix_data[i] = a.matrix_data[i] + b.matrix_data[i % b.nb_column];
	}	
	
	return res;
}

Matrix Multip_factor(Matrix matrix, double a)
{
	Matrix res = Init_matrix(matrix.nb_column,matrix.nb_rows);
	
	for(int i =0; i < matrix.nb_column * matrix.nb_rows; ++i)
		res.matrix_data[i]=matrix.matrix_data[i]*a;
			
	return res;
}

Matrix Sum_weights(Matrix a,Matrix b)
{
	Matrix res = Init_matrix(a.nb_column,a.nb_rows);

	if((a.nb_column != b.nb_column) || (b.nb_rows != a.nb_rows))
	{
		printf("Incompatible matrix in Sum_weights\n");
		exit(-1);
	}
	
	for(int i = 0; i < a.nb_rows * a.nb_column; ++i)
		res.matrix_data[i] = a.matrix_data[i] + b.matrix_data[i];

	return res;
}

Matrix Sigmo_mat(Matrix a)
{
	Matrix res = Init_matrix(a.nb_column,a.nb_rows);	

	for(int i = 0;i < a.nb_column * a.nb_rows;++i)
		res.matrix_data[i] = (1/(1+exp((-1)*a.matrix_data[i]))); 
	
	return res;
}

Matrix Sigmo_mat_derivate(Matrix a)
{
	Matrix res = Init_matrix(a.nb_column,a.nb_rows);
	
	for(int i = 0;i < a.nb_column * a.nb_rows;++i)
		res.matrix_data[i] = (1 * (1 - a.matrix_data[i])); 
	
	return res;
}

Matrix Mult_simple(Matrix a,Matrix b)
{
	if((a.nb_column != b.nb_column) || (b.nb_rows != a.nb_rows))
	{
		printf("Incompatible matrix in Mult_Simple\n");
		exit(-1);
	}
	
	
	Matrix res = Init_matrix(a.nb_column,a.nb_rows);

	for(int i = 0; i< a.nb_rows*a.nb_column;++i)
		res.matrix_data[i] = a.matrix_data[i]*b.matrix_data[i];
	
	return res;

}

void Fill_mat_data(Matrix a, double b[],int size)
{
	for(int i = 0;i<size;++i)
		a.matrix_data[i]=b[i];
}



