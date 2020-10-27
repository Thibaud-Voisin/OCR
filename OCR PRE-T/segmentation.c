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




SDL_Surface* DrawSep(SDL_Surface *image, int a, int b, int c, int COLOR, int rows)
{
	for(int y = 0; y < rows; y++)
	{
		if(a < 0)
			a = 0;
		if(b >= (image -> h))
			b = (image -> h) - 1;
		if(c < 0)
			c = 0;

		put_pixel(image, a, b+y, COLOR);
		put_pixel(image, c, b+y, COLOR);
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

			//copies the selected part of matrix into line
			CopyMatrix(matrix, line, 0, StartIndex);

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

	int nbofzeros = 0;

	average *= 0.6;

	int nbWords = CountWords(histov, average);

	Matrix_Array words = Init_Matrix_Array(nbWords);

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

				word = Init_matrix(i-(StartIndex+nbofzeros-2), line.nb_rows);

				//copies the selected part of line into word
				CopyMatrix(line, word, StartIndex, 0);

				words.array_data[counter] = word;
				WordsIndex.array_data[counter] = StartIndex;
				counter++;

				//next line for visual render
				image = DrawSep(image, StartIndex-1, index, i-nbofzeros+1, GREEN, line.nb_rows);
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

	int nbLetters = CountLetters(histov);
	
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

			//copies the selected part of word into letter
			CopyMatrix(word, letter, StartIndex, 0);

			letters.array_data[counter] = letter;
			
			counter++;

			//for visual render
			image = DrawSep(image, StartIndex+index2-1, index, i+index2-1, BLUE, word.nb_rows);
			
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

	free(words.array_data);
	free(letters.array_data);

	srand(time(NULL)); //used to init the random in RandomLetter

	printf("-------- Texte :\n");

	Array histo = histoH(matrix);
	Array LinesIndex = Init_Array(histo.size);
	Array WordsIndex = Init_Array(0);
	
	free(WordsIndex.array_data);
	
	Matrix_Array lines = Seg_Lines(matrix, histo, image, LinesIndex);

	//next lines for graphic render
	contrast(image);
	display_image(image, texture, renderer);
	wait_key_pressed();

	//this loop is for graphic render only : it does the same as bellow but without the letters
	for(int i = 0; i < lines.size; i++)
	{
		histov = histoV(lines.array_data[i]);
		average = LetterSizeAverage(histov);
		WordsIndex = Init_Array(histov.size);
		words = Seg_Words(lines.array_data[i], histov, average, image, LinesIndex.array_data[i], WordsIndex);
		freeArrays(words);
		free(histov.array_data);
		free(WordsIndex.array_data);
	}
	contrast(image);
	display_image(image, texture, renderer);
	wait_key_pressed();


	//core segmentation
	for(int i = 0; i < lines.size; i++)
	{
		histov = histoV(lines.array_data[i]);
		average = LetterSizeAverage(histov);
		WordsIndex = Init_Array(histov.size);
		words = Seg_Words(lines.array_data[i], histov, average, image, LinesIndex.array_data[i], WordsIndex);
		free(histov.array_data);	
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
			freeArrays(letters);
			free(histov.array_data);	

		}
		printf("\n");	
		freeArrays(words);
		free(WordsIndex.array_data);


	}
	printf("----------- Fin Texte\n");

	//next line here to update the displayed image
	contrast(image);


	free(histo.array_data);
	free(LinesIndex.array_data);
	freeArrays(lines);
}


