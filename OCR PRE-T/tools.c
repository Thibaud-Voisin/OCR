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
		
	srand(time(NULL));
	for (int i = 0; i < max_value; ++i)
	{
		double rd = (double) rand() / RAND_MAX;
		*(matrix.matrix_data+i) = rd;	
	}
}


/*void Pretty_print(Matrix matrix)
{
	int width_pretty = 1+(7*matrix.nb_column);
	char top[width_pretty];
	
	for(int i = 0;i<width_pretty;++i)
		top[i] = '_';

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
}*/


void Pretty_print(Matrix matrix)
{
	printf("matrix size : %d x %d\n", matrix.nb_rows, matrix.nb_column);
	printf("----------------------\n");
	for(int i = 0; i < matrix.nb_rows; i++)
	{
		for(int j = 0; j < matrix.nb_column; j++)
			printf("%.0f", matrix.matrix_data[j + (i * matrix.nb_column)]);
		printf("\n");
	}
	printf("----------------------\n");
}




Array Init_Array(int size)
{
	Array array;

	array.size = size;

	array.array_data = calloc(size, sizeof(int));

	return array;
}


void Pretty_print_array(Array array)
{
	printf("size of array : %d\n", array.size);
	printf("----------------------\n");
	for(int i = 0; i < array.size; i++)
		printf("-- %d\n", array.array_data[i]);
	printf("----------------------\n");
}


Matrix_Array Init_Matrix_Array(int size)
{
	Matrix_Array matrix_array;

	matrix_array.size = size;

	matrix_array.array_data = calloc(size*sizeof(Matrix), sizeof(Matrix));

	return matrix_array;
}


Array_Array Init_Array_Array(int size)
{
	Array_Array array_array;

	array_array.size = size;

	array_array.array_data = calloc(size*sizeof(Array), sizeof(Array));

	return array_array;
}


void wait_key_pressed()
{
	int i = 0;
	SDL_Event event;
	while(i == 0)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					i = 1;
					break;
				default:
					continue;
			}
			continue;
		}
	}
}


void freeArrays(Matrix_Array array)
{
	for(int i = 0; i < array.size; i++)
		free(array.array_data[i].matrix_data);
	free(array.array_data);
}

//used in segmentation
void CopyMatrix(Matrix matrix, Matrix copy, int a, int b)
{
	for(int i = 0; i < copy.nb_rows; i++)
	{
		for(int j = 0; j < copy.nb_column; j++)
			copy.matrix_data[j + (i * copy.nb_column)] = matrix.matrix_data[ j + a + ((i + b) * matrix.nb_column)];
			
	}
}

/*counts number of words, calculating the space between words to differentiate it from space between letters*/
int CountWords(Array histov, float average)
{
	int InProcess = 0;
	int nbWords = 0;
	int nbofzeros = 0;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == 0)
		{	
			InProcess = 1;
		}

		if(histov.array_data[i] != 0 && InProcess == 1)
			nbofzeros = 0;

		if(histov.array_data[i] == 0 && InProcess == 1)
		{
			nbofzeros++;
			if(nbofzeros >= average || i == histov.size-1)
			{
				nbWords++;
				InProcess = 0;
			}
		}
	}
	return nbWords;
}

int CountLetters(Array histov)
{
	int InProcess = 0;
	int nbLetters = 0;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == 0)
		{	
			InProcess = 1;
		}

		if((histov.array_data[i] == 0 || i == histov.size-1) && InProcess == 1)
		{
			nbLetters++;
			InProcess = 0;
		}
	}
	return nbLetters;
}
