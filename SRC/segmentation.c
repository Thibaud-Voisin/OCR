#include "segmentation.h"
#include "neural_network.h"
#define FALSE2 0
#define TRUE2 1

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
	int InProcess = FALSE2;

	float average = 0;
	int nbletters = 0;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE2)
		{
			InProcess = TRUE2;
			nbletters++;
		}

		if(histov.array_data[i] == 0 && InProcess == TRUE2)
			InProcess = FALSE2;
		if(InProcess == TRUE2)
			average++;
	}

	return average/nbletters;
}

int Count(Array histo)
{
	int InProcess = FALSE2;

	int nbLines = 0;

	/*counts the number of lines it will need to separate, to have the size of the returned array*/
	for(int i = 0; i < histo.size; i++)
	{
		if(histo.array_data[i] != 0 && InProcess == FALSE2)
		{	
			InProcess = TRUE2;

			nbLines++;
		}

		if(histo.array_data[i] == 0 && InProcess == TRUE2)
			InProcess = FALSE2;
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
Matrix_Array Seg_Lines(Matrix matrix, Array histo, SDL_Surface *image, Array LinesIndex, GtkToggleButton *display)
{	
    int counter = 0;

	int InProcess = FALSE2;

	int StartIndex = 0;

	int nbLines = Count(histo);

	Matrix_Array lines = Init_Matrix_Array(nbLines);

	Matrix line;

	/*copies the selected line into a new matrix*/
	for(int i = 0; i < histo.size; i++)
	{
		if(histo.array_data[i] != 0 && InProcess == FALSE2)
		{
			InProcess = TRUE2;
			StartIndex = i;
		}

		if((histo.array_data[i] == 0 || i == (histo.size-1)) && InProcess == TRUE2)
		{
			InProcess = FALSE2;

			line = Init_matrix(matrix.nb_column, i-StartIndex);

			//copies the selected part of matrix into line
			CopyMatrix(matrix, line, 0, StartIndex);

			lines.array_data[counter] = line;

			LinesIndex.array_data[counter] = StartIndex;
			
			counter++;

			//for visual render
            if(gtk_toggle_button_get_active(display))
			    image = DrawLine(image, i, StartIndex);
		}
	}

	return lines;
}



/*same process as for lines, but vertically for words*/
Matrix_Array Seg_Words(Matrix line, Array histov, float average, SDL_Surface *image, int index, Array WordsIndex, GtkToggleButton *display)
{
	int counter = 0;

	int InProcess = FALSE2;

	int StartIndex = 0;

	int nbofzeros = 0;

	average *= 0.53;

	int nbWords = CountWords(histov, average);

	Matrix_Array words = Init_Matrix_Array(nbWords);

	Matrix word;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE2)
		{
			InProcess = TRUE2;
			StartIndex = i;
		}

		if(histov.array_data[i] != 0 && InProcess == TRUE2)
			nbofzeros = 0;

		if((histov.array_data[i] == 0 || i == (histov.size-1)) && InProcess == TRUE2)
		{
			nbofzeros++;
			if(nbofzeros >= average || i == histov.size-1)
			{

				InProcess = FALSE2;

				word = Init_matrix(i-(StartIndex+nbofzeros-2), line.nb_rows);

				//copies the selected part of line into word
				CopyMatrix(line, word, StartIndex, 0);

				words.array_data[counter] = word;
				WordsIndex.array_data[counter] = StartIndex;
				counter++;

				//next line for visual render
                if(gtk_toggle_button_get_active(display))
				    image = DrawSep(image, StartIndex-1, index, i-nbofzeros+1, GREEN, line.nb_rows);
			}
		}
	}

	return words;
}


Matrix DoubleCheck(Matrix letter)
{
    Array histo = histoV(letter);
    Array histo2 = histoH(letter);
    int a = 0;
    int b = 0; 
    int c = 0; 
    int d = 0;

    while(histo.array_data[0+a] == 0)
        a += 1;
   
    while(histo.array_data[histo.size-1-b] == 0)
        b += 1; 
    
    while(histo2.array_data[0+c] == 0)
        c += 1;

    while(histo2.array_data[histo.size-1-d] == 0)
        d += 1;

    Matrix copy = Init_matrix(letter.nb_column-a-b, letter.nb_rows-c-d);
    
    CopyMatrix(letter,copy,a,c);

    free(histo.array_data);
    free(histo2.array_data);
 
    return copy;
}


/*same process as for words*/
Matrix_Array Seg_Letters(Matrix word, Array histov, SDL_Surface *image, int index, int index2, GtkToggleButton *display)
{
	int counter = 0;

	int InProcess = FALSE2;

	int StartIndex = 0;

	int nbLetters = CountLetters(histov);
	
	Matrix_Array letters = Init_Matrix_Array(nbLetters);

	Matrix letter;

	for(int i = 0; i < histov.size; i++)
	{
		if(histov.array_data[i] != 0 && InProcess == FALSE2)
		{
			InProcess = TRUE2;
			StartIndex = i;
		}

		if((histov.array_data[i] == 0 || i == (histov.size-1)) && InProcess == TRUE2)
		{

			if(i == (histov.size-1))
				i++;

			InProcess = FALSE2;

			letter = Init_matrix(i-StartIndex, word.nb_rows);

			//copies the selected part of word into letter
			CopyMatrix(word, letter, StartIndex, 0);
 
            if((float)letter.nb_column/(float)letter.nb_rows > 5 || (float)letter.nb_rows/(float)letter.nb_column > 100)
            {
                nbLetters--;
                continue;
            }
            letters = PropagationFix(letters, letter, &counter, &nbLetters);
            
            counter++;

			//for visual render
            if(gtk_toggle_button_get_active(display))
			    image = DrawSep(image, StartIndex+index2-1, index, i+index2-1, BLUE, word.nb_rows);			
		}
	}
    Matrix_Array final = Init_Matrix_Array(nbLetters);
    for(int i = 0; i < nbLetters; i++)
    {
        final.array_data[i] = letters.array_data[i];
    }

	return final;
}


/*returning a random letter
  soon to be replaced by neural network*/
char RandomLetter()
{
	int c = rand() % 25;
	return 'A'+c;
}


Matrix CutEdges(Matrix letter)
{
    Array histo = histoH(letter);
    int StartIndex = 0;
    while(histo.array_data[StartIndex] == 0)
        StartIndex++;
    int EndIndex = letter.nb_rows-1;
    while(histo.array_data[EndIndex] == 0)
        EndIndex--;
    Matrix cropped = Init_matrix(letter.nb_column,letter.nb_rows-StartIndex-(letter.nb_rows-EndIndex)+1);
    CopyMatrix(letter, cropped, 0, StartIndex);
    free(histo.array_data);
    return cropped;
}

Matrix CutEdges2(Matrix letter)
{
    Array histo = histoV(letter);
    int StartIndex = 0;
    while(histo.array_data[StartIndex] == 0)
        StartIndex++;
    int EndIndex = letter.nb_column-1;
    while(histo.array_data[EndIndex] == 0)
        EndIndex--;
    Matrix cropped = Init_matrix(letter.nb_column-StartIndex-(letter.nb_column-EndIndex)+1, letter.nb_rows);
    CopyMatrix(letter, cropped, StartIndex, 0);
    free(histo.array_data);
    return cropped;
}

Matrix Resize(Matrix letter)
{
    int a = letter.nb_rows;
    int b = letter.nb_column;
    float tmp1 = a;
    float tmp2 = b;
    while(tmp1 > 21 || tmp2 > 21)
    {
       tmp1 = tmp1/1.005f;
       tmp2 = tmp2/1.005f;
    }
    a = (int)tmp1;
    b = (int)tmp2;
    Matrix resized = Init_matrix(20,20);
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            if(i >= a || j >= b)
                resized.matrix_data[j+(i*20)] = 1;
            else
            {
                if(letter.nb_rows < 20 && letter.nb_column < 20)
                    resized.matrix_data[j+(i*20)] = letter.matrix_data[j+(i*b)]; 
                else
                {
                    resized.matrix_data[j+(i*20)] = letter.matrix_data[(int)(j*((float)letter.nb_rows/a))+((int)(i*((float)letter.nb_column/b))*letter.nb_column)];
                }
            }
        }
    }
    return resized;
}

void Recursive(Matrix letter, Matrix new, Matrix path, int x, int y)
{
    new.matrix_data[x*new.nb_column+y] = 0;
    path.matrix_data[x*path.nb_column+y] = 1;
 
    if(y+1 < new.nb_column && path.matrix_data[x*path.nb_column+y+1] == 0 && letter.matrix_data[x*letter.nb_column+y+1] == 0)
        Recursive(letter, new, path, x, y+1);
    if(x-1 >= 0 && path.matrix_data[(x-1)*path.nb_column+y] == 0 && letter.matrix_data[(x-1)*letter.nb_column+y] == 0)
        Recursive(letter, new, path, x-1, y);
    if(x+1 < new.nb_rows && path.matrix_data[(x+1)*path.nb_column+y] == 0 && letter.matrix_data[(x+1)*letter.nb_column+y] == 0)
        Recursive(letter, new, path, x+1, y);
    if(y-1 >= 0 && path.matrix_data[x*path.nb_column+y-1] == 0 && letter.matrix_data[x*letter.nb_column+y-1] == 0)
        Recursive(letter, new, path, x, y-1);
    if(x-1 >= 0 && y-1 >= 0 && path.matrix_data[(x-1)*path.nb_column+y-1] == 0 && letter.matrix_data[(x-1)*letter.nb_column+y-1] == 0)
        Recursive(letter, new, path, x-1, y-1);
    if(x-1 >= 0 && y+1 < new.nb_column && path.matrix_data[(x-1)*path.nb_column+y+1] == 0 && letter.matrix_data[(x-1)*letter.nb_column+y+1] == 0)
        Recursive(letter, new, path, x-1, y+1);
    if(x+1 < new.nb_rows && y-1 >= 0 && path.matrix_data[(x+1)*path.nb_column+y-1] == 0 && letter.matrix_data[(x+1)*letter.nb_column+y-1] == 0)
        Recursive(letter, new, path, x+1, y-1);
    if(x+1 < new.nb_rows && y+1 < new.nb_column && path.matrix_data[(x+1)*path.nb_column+y+1] == 0 && letter.matrix_data[(x+1)*letter.nb_column+y+1] == 0)
        Recursive(letter, new, path, x+1, y+1);

}

Matrix IsolateLetter(Matrix letter, int x, int y)
{
    Matrix new = Init_matrix(letter.nb_column,letter.nb_rows);
    for(int i = 0; i < new.nb_rows; i++)
        for(int j = 0; j < new.nb_column; j++)
            new.matrix_data[i*new.nb_column+j] = 1;
    Matrix boolM = Init_matrix(letter.nb_column, letter.nb_rows);
    Recursive(letter, new, boolM, x, y);
    return new;
}

Matrix_Array DoubleLetters(Matrix letter)
{
    Matrix_Array tmp = Init_Matrix_Array(2);
    int a = 0;
    int b = 0;
    while(a < letter.nb_rows && letter.matrix_data[a*letter.nb_column] == 1)
        a++;
    while(b < letter.nb_rows && letter.matrix_data[b*letter.nb_column+letter.nb_column-1] == 1)
        b++;
    tmp.array_data[0] = IsolateLetter(letter,a,0);
    tmp.array_data[1] = IsolateLetter(letter,b,letter.nb_column-1);
    return tmp;
}

Matrix_Array PropagationFix(Matrix_Array letters, Matrix letter, int* counter, int* nbLetters)
{
    letter = CutEdges(letter);
    letter = CutEdges2(letter);
    int add = 0;
    Matrix_Array tmp = Init_Matrix_Array(2);
    int k = 0;
    Array histo = histoH(letter);
    while(k < histo.size && histo.array_data[k] != 0)
        k++;
    if(k != histo.size)
    {
        letter = CutEdges(letter);
        letter = CutEdges2(letter);
        letter = Resize(letter);
        letters.array_data[*counter] = letter;
        return letters;
    }
    tmp = DoubleLetters(letter);

    if(CompareMatrix(tmp.array_data[0],tmp.array_data[1]) == 1)
    {
        letter = CutEdges(tmp.array_data[0]);
        letter = CutEdges2(letter);
        letter = Resize(letter);
        letters.array_data[*counter] = letter;
    }
    else
    {
        add = 1;
        letter = CutEdges(tmp.array_data[0]);
        letter = CutEdges2(letter);
        letter = Resize(letter);
        letters.array_data[*counter] = letter;
        *counter += 1;
        *nbLetters += 1;
        letter = CutEdges(tmp.array_data[1]);
        letter = CutEdges2(letter);
        letter = Resize(letter);
        letters.array_data[*counter] = letter;
    }
    Matrix_Array new = Init_Matrix_Array(letters.size+add);
    for(int x = 0; x < letters.size+add; x++)
    {
        new.array_data[x] = letters.array_data[x];
    }
    return new;
}

void refresh(SDL_Surface *image)
{
    for(int i = 0; i < image -> w; i++)
    {
        for(int j = 0; j < image -> h; j++)
        {
            Uint8 r,g,b;
            SDL_GetRGB(get_pixel(image, i, j), image -> format, &r, &g, &b);
            put_pixel(image, i, j, SDL_MapRGB(image -> format, r, g, b));
        }
    }
}


/*uses all the above functions to fully split the image into letter,
  but keeping the format of the text*/
gchar* Segmentation(Matrix matrix, SDL_Surface *image, gchar *txt, app_widgets *app_wdgts)
{
    txt = "";
    Array histov;
	float average;

	Matrix_Array words = Init_Matrix_Array(0);
	Matrix_Array letters = Init_Matrix_Array(0);

	Array histo = histoH(matrix);
	Array LinesIndex = Init_Array(histo.size);
	Array WordsIndex = Init_Array(0);
	
	Matrix_Array lines = Seg_Lines(matrix, histo, image, LinesIndex, GTK_TOGGLE_BUTTON(app_wdgts -> w_btn_drlines));

	//core segmentation
	for(int i = 0; i < lines.size; i++)
	{
		histov = histoV(lines.array_data[i]);
		average = LetterSizeAverage(histov);
		WordsIndex = Init_Array(histov.size);
		words = Seg_Words(lines.array_data[i], histov, average, image, LinesIndex.array_data[i], WordsIndex, GTK_TOGGLE_BUTTON(app_wdgts -> w_btn_drwords));
		for(int j = 0; j < words.size; j++)
		{
			histov = histoV(words.array_data[j]);

			letters = Seg_Letters(words.array_data[j], histov, image, LinesIndex.array_data[i], WordsIndex.array_data[j], GTK_TOGGLE_BUTTON(app_wdgts -> w_btn_drletters));

        
			Matrix k_2000;
			for(int k = 0; k < letters.size; k++)
			{
				k_2000 = letters.array_data[k];
				k_2000.nb_column *= k_2000.nb_rows;
				k_2000.nb_rows = 1;	
					
				Neural_network Net_train = Init_neural_network(k_2000,400,60,89,1);
				char c = Evaluate_char(Net_train,k_2000);
					
                txt = g_strdup_printf("%s%c",txt,c);
            }
			free(histov.array_data);
            txt = g_strdup_printf("%s%s",txt," ");

            freeArrays(letters);

		}
        txt = g_strdup_printf("%s%s",txt,"\n");
		freeArrays(words);
		free(WordsIndex.array_data);

	}
	free(histo.array_data);
	free(LinesIndex.array_data);
	freeArrays(lines);
    refresh(image);
    
    return txt;
}


/*uses all the above functions to fully split the image into letter,
  but keeping the format of the text*/
Matrix_Array Segmentation2(Matrix matrix, SDL_Surface *image, app_widgets *app_wdgts)

{
    Array histov;
	float average;


    Matrix_Array allLetters = Init_Matrix_Array(100000);
    int counter = 0;

	Matrix_Array words = Init_Matrix_Array(0);
	Matrix_Array letters = Init_Matrix_Array(0);

	free(words.array_data);
	free(letters.array_data);

	srand(time(NULL)); //used to init the random in RandomLetter

	Array histo = histoH(matrix);
	Array LinesIndex = Init_Array(histo.size);
	Array WordsIndex = Init_Array(0);
	
	free(WordsIndex.array_data);
	
	Matrix_Array lines = Seg_Lines(matrix, histo, image, LinesIndex, GTK_TOGGLE_BUTTON(app_wdgts -> w_btn_drlines));


	//core segmentation
	for(int i = 0; i < lines.size; i++)
	{
		histov = histoV(lines.array_data[i]);
		average = LetterSizeAverage(histov);
		WordsIndex = Init_Array(histov.size);
		words = Seg_Words(lines.array_data[i], histov, average, image, LinesIndex.array_data[i], WordsIndex, GTK_TOGGLE_BUTTON(app_wdgts -> w_btn_drwords));
;
		free(histov.array_data);	
		for(int j = 0; j < words.size; j++)
		{
			histov = histoV(words.array_data[j]);
			letters = Seg_Letters(words.array_data[j], histov, image, LinesIndex.array_data[i], WordsIndex.array_data[j], GTK_TOGGLE_BUTTON(app_wdgts -> w_btn_drletters));
;
			for(int k = 0; k < letters.size; k++)
			{
                allLetters.array_data[counter] = letters.array_data[k];
                counter++;
            }
			free(histov.array_data);	

		}
		freeArrays(words);
		free(WordsIndex.array_data);


	}

	free(histo.array_data);
	free(LinesIndex.array_data);
	freeArrays(lines);

    Matrix_Array final = Init_Matrix_Array(counter);

    for(int i = 0; i < counter; i++)
        final.array_data[i] = allLetters.array_data[i];

    return final;
}
