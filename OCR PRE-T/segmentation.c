#include "segmentation.h"

#define FALSE 0
#define TRUE 1

#define RED 16580620
#define GREEN 1636389
#define BLUE 459007


/*create an histogram of the number of zeros in each line of the given matrix*/
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

/*create an histogram of the number of zeros in each column of the given matrix*/
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

/*returns the average size of one letter based on a given histogram*/
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

int Count(Array histo)
{
	int InProcess = FALSE;

	int nbLines = 0;

	/*counts the number of lines it will need to separate, to have the size of the returned array*/
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
	return nbLines;

}



//next 3 functions are used to make a visual render of the segmentation

SDL_Surface* DrawLine(SDL_Surface *image, int i, int StartIndex)
{
	for(int x = 0; x < image -> w; x++)
	{
		int a = StartIndex-1;
		if(a < 0)
			a = 0;

		put_pixel(image, x, a, RED);
		put_pixel(image, x, i, RED);
	}
	return image;
}




SDL_Surface* DrawWordSep(SDL_Surface *image, int i, int StartIndex, int index, int rows, int nbofzeros)
{
	for(int y = 0; y < rows; y++)
	{
		int a = StartIndex-1;
		int b = index+y;
		int c = i - nbofzeros+1;
		if(a < 0)
			a = 0;
		if(b >= (image -> h))
			b = (image -> h) - 1;
		if(c < 0)
			c = 0;

		put_pixel(image, a, b, GREEN);
		put_pixel(image, c, b, GREEN);
	}
	return image;

}


SDL_Surface* DrawLetterSep(SDL_Surface *image, int i, int StartIndex, int index, int index2, int rows)
{
	for(int y = 0; y < rows; y++)
	{
		int a = StartIndex+index2-1;
		int b = index+y;
		int c = i + index2;
		if(a < 0)
			a = 0;
		if(b >= (image -> h))
			b = (image -> h) - 1;
		if(c < 0)
			c = 0;

		put_pixel(image, a, b, BLUE);
		put_pixel(image, c, b, BLUE);
	}

	return image;
}








/*Separates the matrix into lines*/
Matrix_Array Seg_Lines(Matrix matrix, Array histo, SDL_Surface *image, Array LinesIndex)
{
	int counter = 0;

	int InProcess = FALSE;

	int StartIndex = 0;

	int nbLines = Count(histo);

	Matrix_Array lines = Init_Matrix_Array(nbLines);

	Matrix line;

	/*copies the selected line into a new matrix*/
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
			LinesIndex.array_data[counter] = StartIndex;
			counter++;

			//for visual render
			image = DrawLine(image, i, StartIndex);
		}
	}

	return lines;
}





/*same process as for lines, but vertically for words*/
Matrix_Array Seg_Words(Matrix line, Array histov, float average, SDL_Surface *image, int index, Array WordsIndex)
{
	int counter = 0;

	int InProcess = FALSE;

	int StartIndex = 0;

	int nbWords = 0;

	int nbofzeros = 0;

	average *= 0.6;

	/*counts number of words, calculating the space between words to differentiate it from space between letters*/
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
				WordsIndex.array_data[counter] = StartIndex;
				counter++;

				//next line for visual render
				image = DrawWordSep(image, i, StartIndex, index, line.nb_rows, nbofzeros);
			}
		}
	}

	return words;
}






/*same process as for words*/
Matrix_Array Seg_Letters(Matrix word, Array histov, SDL_Surface *image, int index, int index2)
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

			//for visual render
			image = DrawLetterSep(image, i, StartIndex, index, index2, word.nb_rows);
			
		}
	}

	return letters;
}






/*returning a random letter
  soon to be replaced by neural network*/
char RandomLetter()
{
	int c = rand() % 25;
	return 'A'+c;
}






/*uses all the above functions to fully split the image into letter,
  but keeping the format of the text*/
void Segmentation(Matrix matrix, SDL_Surface *image, SDL_Texture *texture, SDL_Renderer *renderer)
{
	Array histov;
	float average;
	Matrix_Array words = Init_Matrix_Array(0);
	Matrix_Array letters = Init_Matrix_Array(0);

	srand(time(NULL)); //used to init the random in RandomLetter

	printf("-------- Texte :\n");

	Array histo = histoH(matrix);
	Array LinesIndex = Init_Array(histo.size);
	Matrix_Array lines = Seg_Lines(matrix, histo, image, LinesIndex);

	//next lines for graphic render
	image = contrast(image);
	display_image(image, texture, renderer);
	wait_key_pressed();

	//this loop is for graphic render only : it does the same as bellow but without the letters
	for(int i = 0; i < lines.size; i++)
	{
		histov = histoV(lines.array_data[i]);
		average = LetterSizeAverage(histov);
		Array WordsIndex = Init_Array(histov.size);
		words = Seg_Words(lines.array_data[i], histov, average, image, LinesIndex.array_data[i], WordsIndex);
	}
	image = contrast(image);
	display_image(image, texture, renderer);
	wait_key_pressed();


	//core segmentation
	for(int i = 0; i < lines.size; i++)
	{
		histov = histoV(lines.array_data[i]);
		average = LetterSizeAverage(histov);
		Array WordsIndex = Init_Array(histov.size);
		words = Seg_Words(lines.array_data[i], histov, average, image, LinesIndex.array_data[i], WordsIndex);
		for(int j = 0; j < words.size; j++)
		{
			histov = histoV(words.array_data[j]);
			letters = Seg_Letters(words.array_data[j], histov, image, LinesIndex.array_data[i], WordsIndex.array_data[j]);
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

	//next line here to update the displayed image
	image = contrast(image);
	
	free(words.array_data);
	free(letters.array_data);
	free(histo.array_data);
	free(lines.array_data);
}
