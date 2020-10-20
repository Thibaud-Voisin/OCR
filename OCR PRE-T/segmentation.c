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

Array histoV(Matrix matrix)
{
	Array histo = Init_Array(matrix.nb_column);

	for(int i = 0; i < matrix.nb_column; i++)
	{
		for(int j = 0; j < matrix.nb_rows; j++)
		{	
			if(matrix.matrix_data[i + (j * matrix.nb_column)] == 0)
			{
				histo.array_data[i] += 1;
			}	
		}
	}
	return histo;
}

float LetterSizeAverage(Array histov)
{
	int InProcess = FALSE;

	float average = 0;
	int nbletters = 0;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE)
		{
			InProcess = TRUE;
			nbletters++;
		}

		if(histov.array_data[i] == 0 && InProcess == TRUE)
			InProcess = FALSE;
		if(InProcess == TRUE)
			average++;
	}

	return average/nbletters;

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

		if((histo.array_data[i] == 0 || i == (histo.size-1)) && InProcess == TRUE)
		{
			InProcess = FALSE;

			line = Init_matrix(matrix.nb_column, i-StartIndex);

			for(int k = 0; k < line.nb_rows; k++)
			{
				for(int j = 0; j < line.nb_column; j++)
				{
					line.matrix_data[j + (k * line.nb_column)] = matrix.matrix_data[j+((k+StartIndex)*matrix.nb_column)];

				}
			}
			lines.array_data[counter] = line;
			counter++;
		}
	}

	return lines;
}

Matrix_Array Seg_Words(Matrix line, Array histov, float average)
{
	int counter = 0;

	int InProcess = FALSE;

	int StartIndex = 0;

	int nbWords = 0;

	int nbofzeros = 0;

	average *= 0.6;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE)
		{	
			InProcess = TRUE;
		}

		if(histov.array_data[i] != 0 && InProcess == TRUE)
			nbofzeros = 0;

		if(histov.array_data[i] == 0 && InProcess == TRUE)
		{
			nbofzeros++;
			if(nbofzeros >= average || i == histov.size-1)
			{
				nbWords++;
				InProcess = FALSE;
			}
		}
	}

	Matrix_Array words = Init_Matrix_Array(nbWords);

	nbofzeros = 0;

	Matrix word;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE)
		{
			InProcess = TRUE;
			StartIndex = i;
		}

		if(histov.array_data[i] != 0 && InProcess == TRUE)
			nbofzeros = 0;

		if((histov.array_data[i] == 0 || i == (histov.size-1)) && InProcess == TRUE)
		{
			nbofzeros++;
			if(nbofzeros >= average || i == histov.size-1)
			{

				InProcess = FALSE;

				word = Init_matrix(i-(StartIndex+nbofzeros-1), line.nb_rows);

				for(int k = 0; k < word.nb_rows; k++)
				{
					for(int j = 0; j < word.nb_column; j++)
					{
						word.matrix_data[j + (k * word.nb_column)] = line.matrix_data[j+StartIndex + (k * line.nb_column)]; 
					}
				}
				words.array_data[counter] = word;
				counter++;
			}
		}
	}

	return words;
}




Matrix_Array Seg_Letters(Matrix word, Array histov)
{
	int counter = 0;

	int InProcess = FALSE;

	int StartIndex = 0;

	int nbLetters = 0;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE)
		{	
			InProcess = TRUE;
		}

		if((histov.array_data[i] == 0 || i == histov.size-1) && InProcess == TRUE)
		{
			nbLetters++;
			InProcess = FALSE;
		}
	}

	Matrix_Array letters = Init_Matrix_Array(nbLetters);

	Matrix letter;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE)
		{
			InProcess = TRUE;
			StartIndex = i;
		}

		if((histov.array_data[i] == 0 || i == (histov.size-1)) && InProcess == TRUE)
		{

			if(i == (histov.size-1))
				i++;

			InProcess = FALSE;

			letter = Init_matrix(i-StartIndex, word.nb_rows);

			for(int k = 0; k < letter.nb_rows; k++)
			{
				for(int j = 0; j < letter.nb_column; j++)
				{
					letter.matrix_data[j + (k * letter.nb_column)] = word.matrix_data[j+StartIndex + (k * word.nb_column)]; 
				}
			}
			letters.array_data[counter] = letter;
			counter++;
		}
	}

	return letters;
}

char RandomLetter()
{
	int c = rand() % 25;
	return 'A'+c;
}

void Segmentation(Matrix matrix)
{
	Array histov;
	float average;
	Matrix_Array words;
	Matrix_Array letters;
	srand(time(NULL));
	printf("-------- Texte :\n");
	Array histo = histoH(matrix);
	Matrix_Array lines = Seg_Lines(matrix, histo);
	for(int i = 0; i < lines.size; i++)
	{
		histov = histoV(lines.array_data[i]);
		average = LetterSizeAverage(histov);
		words = Seg_Words(lines.array_data[i], histov, average);
		for(int j = 0; j < words.size; j++)
		{
			histov = histoV(words.array_data[j]);
			letters = Seg_Letters(words.array_data[j], histov);
			for(int k = 0; k < letters.size; k++)
			{
				char c = RandomLetter();
				printf("%c", c);
			}
			printf("  ");
		}
		printf("\n");
	}
	printf("----------- Fin Texte\n");
}
