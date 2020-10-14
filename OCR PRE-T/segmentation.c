#include "segmentation.h"

#define FALSE 0
#define TRUE 1

void histoH(Matrix matrix, int *histo)
{
	for(int i = 0; i < matrix.nb_rows; i++)
		histo[i] = 0;

	for(int i = 0; i < matrix.nb_rows; i++)
	{
		for(int j = 0; j < matrix.nb_column; j++)
		{
			if(matrix.matrix_data[j + (i * matrix.nb_column)] == 0)
			{
				histo[i] += 1;
			}		
		}
	}
}

void Seg_Lines(Matrix matrix, int *histo, Matrix *lines)
{
	int counter = 0;

	int InProcess = 0;

	int StartIndex = 0;

	for(int i = 0; i < sizeof(histo)/sizeof(histo[0]); i++)
		printf("Apres --- %d\n", histo[i]);




	for(int i = 0; i < sizeof(histo)/sizeof(histo[0]); i++)
	{
		printf("histo[i] = %d\n", histo[i]);
		if(histo[i] != 0 && InProcess == FALSE)
		{
			InProcess = TRUE;
			StartIndex = i;
		}

		if((histo[i] == 0 && InProcess == TRUE) || i == (sizeof(histo)/sizeof(histo[0])-1))
		{
			InProcess = FALSE;

			Matrix line = Init_matrix(matrix.nb_rows, i-StartIndex);

			printf("rows : %d -- col : %d", line.nb_rows, line.nb_column);

			for(int i = 0; i < line.nb_rows; i++)
			{
				for(int j = 0; j < line.nb_column; j++)
				{
					printf("here");
					line.matrix_data[j + (i * line.nb_column)] = matrix.matrix_data[j + (i+StartIndex * matrix.nb_column)];
				}
			}
			lines[counter] = line;
			counter++;
		}
	}
}
