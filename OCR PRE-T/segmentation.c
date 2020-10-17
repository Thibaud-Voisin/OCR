#include "segmentation.h"

#define FALSE 0
#define TRUE 1



Array histoH(Matrix matrix)
{
	Array histo = Init_Array(matrix.nb_rows);

	for(int i = 0; i < matrix.nb_rows; i++)
	{
		for(int j = 0; j < matrix.nb_column; j++)
		{
			if(matrix.matrix_data[j + (i * matrix.nb_column)] == 0)
			{
				histo.array_data[i] += 1;
			}		
		}
	}
	return histo;
}

Matrix_Array Seg_Lines(Matrix matrix, Array histo)
{

	int counter = 0;

	int InProcess = FALSE;

	int StartIndex = 0;

	int nbLines = 0;

	for(int i = 0; i < histo.size; i++)
	{
		if(histo.array_data[i] != 0 && InProcess == FALSE)
		{	
			InProcess = TRUE;

			nbLines++;

		}

		if(histo.array_data[i] == 0 && InProcess == TRUE)
			InProcess = FALSE;
	}

	Matrix_Array lines = Init_Matrix_Array(nbLines);

	Matrix line;

	for(int i = 0; i < histo.size; i++)
	{
		if(histo.array_data[i] != 0 && InProcess == FALSE)
		{
			InProcess = TRUE;
			StartIndex = i;
		}

		if((histo.array_data[i] == 0 && InProcess == TRUE) || i == (histo.size-1))
		{
			InProcess = FALSE;

			line = Init_matrix(matrix.nb_rows, i-StartIndex);

			for(int i = 0; i < line.nb_rows; i++)
			{
				for(int j = 0; j < line.nb_column; j++)
				{
					line.matrix_data[j + (i * line.nb_column)] = matrix.matrix_data[j + (i+StartIndex * matrix.nb_column)];
				}
			}
			lines.array_data[counter] = line;
			counter++;
		}
	}

	return lines;
}
