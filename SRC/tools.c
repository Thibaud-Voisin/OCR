#include "pre_processing.h"
#include "segmentation.h"
#include "neural_network.h"

Matrix Init_matrix(int width,int height)
{
	Matrix matrix;
	matrix.matrix_data = malloc(width*height*sizeof(double)*8);
	for(int i =0; i < width*height;++i)
		matrix.matrix_data[i] = 0 ;
	matrix.nb_column = width;

	matrix.nb_rows = height;

	return matrix;
}


Matrix Init_matrix2(int width,int height)
{
	Matrix matrix;

	matrix.matrix_data = calloc(width*height,sizeof(double));

	matrix.nb_column = width;

	matrix.nb_rows = height;

	return matrix;
}

void Fill_mat_rand_for_output(Matrix matrix)
{
	int max_value = sizeof(double)*matrix.nb_column*matrix.nb_rows;
	
	double sign;

	for (int i = 0; i < max_value; ++i)
	{
		double rd = (double) rand() / RAND_MAX;
		sign = (double) rand();
	   	if(sign*2 > RAND_MAX)
		{
			rd*=(-1);
		}	
			rd/=5;
		*(matrix.matrix_data+i) = rd;	
	}
}

void Fill_mat_rand(Matrix matrix)
{
	int max_value = sizeof(double)*matrix.nb_column*matrix.nb_rows;
	
	double sign;

	for (int i = 0; i < max_value; ++i)
	{
		double rd = (double) rand() / RAND_MAX;
		sign = (double) rand();
	   	if(sign*2 > RAND_MAX)
		{
			rd*=(-1);
		}	
			rd/=25;
		*(matrix.matrix_data+i) = rd;	
	}
}


void Pretty_print_xor(Matrix matrix)
{
	int width_pretty = 1+(41*matrix.nb_column);
	
	char *top=(char *) calloc(width_pretty+1,sizeof(char));
	
	memset(top, '-', width_pretty);	
	
	int top_i = 0;
	int top_j = 0;

	while(*(top + top_i)!='\0')
        printf("%c",*(top + top_i++));
	printf("\n");

	char *middle=(char *) calloc(width_pretty+1,sizeof(char));
	
	int j = 0;
	char tmp[45];

	for(int i = 0;i<matrix.nb_column * matrix.nb_rows;++i)
	{	
		sprintf(tmp,"%.18f",matrix.matrix_data[i]);
		for (int k = 0;k<40;++k)
		{
			memset((middle+j+k),tmp[k],1);	
		}
		middle[j+40] = '|';
		j+=41;

		if (i%matrix.nb_column == matrix.nb_column-1)
		{
			printf("|%s\n",middle);
			j = 0;
		}
		
	}
	while(*(top + top_j)!='\0')
        printf("%c",*(top + top_j++));
	printf("\n");
	
	free(middle);
	free(top);
}




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

int CompareMatrix(Matrix a, Matrix b)
{
    if(a.nb_rows != b.nb_rows || a.nb_column != b.nb_column)
        return 0;
    for(int i = 0; i < a.nb_rows; i++)
    {
        for(int j = 0; j < a.nb_column; j++)
        {
            if(a.matrix_data[i*a.nb_column + j] != b.matrix_data[i*a.nb_column +j])
                return 0;
        }
    }
    return 1;
}


/*Loads the save file str and fills the other parameters with their corresponding values in the save*/
void load_data(char *str, Matrix hidden_weight, Matrix output_weight, Matrix hidden_bias, Matrix output_bias)
{
	int position = 19;
	double number;
	char chh;
	int one_n_dec = 0;
	double stock_re;
	for(int i = 0; i < hidden_weight.nb_column*hidden_weight.nb_rows; ++i)
	{
	number = 0;
	stock_re =0;
	one_n_dec =0;
		for(int j = 0; j < 19; ++j)
		{
			chh = str[position-j];
			if(chh != ',')	
			{
					if(!one_n_dec)
					{
						number += chh-'0';
					
						number /= 10;
					}
					else
					{
						for(int kl = position-18; str[kl] != ','; ++kl)
						{
								stock_re*=10;
								stock_re+=str[kl] - '0';					
						}						
						number+=stock_re;
		if(str[position-19] == '-')
			number *= (-1);
						break;	
					}
			}
			else
			{
					one_n_dec =1;
			}
		}

			
		position+=20;
		hidden_weight.matrix_data[i] = number;
	}

	for(int i = 0; i < hidden_bias.nb_column*hidden_bias.nb_rows; ++i)
	{
		number = 0;
	stock_re =0;
	one_n_dec =0;
		for(int j = 0; j < 19; ++j)
		{
			chh = str[position-j];
			if(chh != ',')	
			{
					if(!one_n_dec)
					{
						number += chh-'0';
					
						number /= 10;
					}
					else
					{
						for(int kl = position-18; str[kl] != ','; ++kl)
						{
								stock_re*=10;
								stock_re+=str[kl] - '0';					
						}						
						number+=stock_re;

		if(str[position-19] == '-')
			number *= (-1);
						break;	
					}
			}
			else
			{
					one_n_dec =1;
			}
		}

		position+=20;

			hidden_bias.matrix_data[i] = number;
	}


	for(int i = 0; i < output_weight.nb_column*output_weight.nb_rows; ++i)
	{
	number = 0;
	stock_re =0;
	one_n_dec =0;
		for(int j = 0; j < 19; ++j)
		{
			chh = str[position-j];
			if(chh != ',')	
			{
					if(!one_n_dec)
					{
						number += chh-'0';
					
						number /= 10;
					}
					else
					{
						for(int kl = position-18; str[kl] != ','; ++kl)
						{
								stock_re*=10;
								stock_re+=str[kl] - '0';					
						}						
						number+=stock_re;

		if(str[position-19] == '-')
			number *= (-1);
						break;	
					}
			}
			else
			{
					one_n_dec =1;
			}
		}

			
		position+=20;


		output_weight.matrix_data[i] = number;
	}
	
	for(int i = 0; i < output_bias.nb_column*hidden_bias.nb_rows; ++i)
	{
					number = 0;
	stock_re =0;
	one_n_dec =0;
		for(int j = 0; j < 19; ++j)
		{
			chh = str[position-j];
			if(chh != ',')	
			{
					if(!one_n_dec)
					{
						number += chh-'0';
					
						number /= 10;
					}
					else
					{
						for(int kl = position-18; str[kl] != ','; ++kl)
						{
								stock_re*=10;
								stock_re+=str[kl] - '0';					
						}						
						number+=stock_re;

		if(str[position-19] == '-')
			number *= (-1);
						break;	
					}
			}
			else
			{
					one_n_dec =1;
			}
		}

			
		position+=20;
		
	output_bias.matrix_data[i] = number;
	}
}






/*Save the values given in parameters in a save file named according to the amount pixels in input for the training and the anount of hidden neurons*/
void save_data(Matrix hidden_weights, Matrix hidden_bias, Matrix output_weights, Matrix output_bias)
{
	char *str_final = calloc(((hidden_weights.nb_column*hidden_weights.nb_rows)*21)+(hidden_weights.nb_column) + (hidden_bias.nb_column*21) + ((output_weights.nb_column*output_weights.nb_rows)*21)+(output_weights.nb_column) + (output_bias.nb_column*21), sizeof(char));
	
	int pos_str_final = 0;

	for(int i = 0; i < hidden_weights.nb_rows; ++i)
	{
		for(int j = 0; j < hidden_weights.nb_column; ++j)
		{
			char str[10000];
			snprintf(str,30,"%.20f",hidden_weights.matrix_data[i*hidden_weights.nb_column+j]);
	
			int deca = 0;
	
			for(int k = 0; k < 20; ++k)
			{
				if(!k && str[k] != '-')
				{
					str_final[pos_str_final] = '+';
					++pos_str_final;
					deca = 1;
				}
				else
				{
					if(deca)		
						str_final[pos_str_final] = str[k-1];
					else
						str_final[pos_str_final] = str[k];
					++pos_str_final;
				}
			}	
		}
	}
	
	for(int j = 0; j < hidden_bias.nb_column; ++j)
	{
		char str[10000];
		snprintf(str,30,"%.20f",hidden_bias.matrix_data[j]);
	
		int deca = 0;
	
		for(int k = 0; k < 20; ++k)
		{
			if(!k && str[k] != '-')
			{
				str_final[pos_str_final] = '+';
				++pos_str_final;
				deca = 1;
			}
			else
			{
				if(deca)		
					str_final[pos_str_final] = str[k-1];
				else
					str_final[pos_str_final] = str[k];
				++pos_str_final;
			}
		}	
	}

	for(int i = 0; i < output_weights.nb_rows; ++i)
	{
		for(int j = 0; j < output_weights.nb_column; ++j)
		{
			char str[10000];
			snprintf(str,30,"%.20f",output_weights.matrix_data[i*output_weights.nb_column+j]);
	
			int deca = 0;
	
			for(int k = 0; k < 20; ++k)
			{
				if(!k && str[k] != '-')
				{
					str_final[pos_str_final] = '+';
					++pos_str_final;
					deca = 1;
				}
				else
				{
					if(deca)		
						str_final[pos_str_final] = str[k-1];
					else
						str_final[pos_str_final] = str[k];
					++pos_str_final;
				}
			}	
		}
	}
	
	for(int j = 0; j < output_bias.nb_column; ++j)
	{
		char str[10000];
		snprintf(str,30,"%.20f",output_bias.matrix_data[j]);
	
		int deca = 0;
	
		for(int k = 0; k < 20; ++k)
		{
			if(!k && str[k] != '-')
			{
				str_final[pos_str_final] = '+';
				++pos_str_final;
				deca = 1;
			}
			else
			{
				if(deca)		
					str_final[pos_str_final] = str[k-1];
				else
					str_final[pos_str_final] = str[k];
				++pos_str_final;
			}
		}	
	}

	int j_l,n = strlen(str_final);

	for(int i = j_l = 0; i < n; ++i)
		if(str_final[i] != '.')
			str_final[j_l++] = str_final[i];
	str_final[j_l] = 0;
	
	FILE *file;

	int digits_input = 0;
	int digits_hidden = 0;	
	
	int i = hidden_weights.nb_rows;
	int j = hidden_bias.nb_column;
	
		
	for(; i >=10 ; i/=10)
	{
		++digits_input;
	}


	for(; j >=10 ; j/=10)
	{
		++digits_hidden;
	}


	char *name = calloc(digits_input + digits_hidden+6,sizeof(char));

	char str[10000];
	
	snprintf(str,15,"%d",hidden_weights.nb_rows);
	
	int pos = 0;

	for(int k = 0; k <= digits_input; ++k)
	{
		name[k] = str[k];
		++pos;
	}

	name[pos] = '_';
	++pos;

	char str2[10000];
		
	snprintf(str2,15,"%d",hidden_bias.nb_column);
	

	for(int k = 0; k <= digits_hidden; ++k)
	{
		name[pos] = str2[k];
		++pos;
	}

	name[pos] = '_';
	++pos;
	name[pos] = 's';
	++pos;
	name[pos] = 'a';
	++pos;
	name[pos] = 'v';
	++pos;
	name[pos] = 'e';
	++pos;

	char path_r[300] = "training/save/";

	strcat(path_r,name);


	file = fopen(path_r,"w");

	if(file == NULL)
		printf("ERROR\n");
	fprintf(file,"%s", str_final );

	fclose(file);

}





void Mult_mat_1(Matrix a, Matrix b,Matrix res)
{
	if (a.nb_column != b.nb_rows)
	{
		printf("Incompatible Matrices for multiplication\n");
		exit(-1);
	}
	
	for (int i = 0; i < a.nb_rows; ++i) 
	{
   
    	for (int j = 0; j < b.nb_column; ++j) 
		{
            double sum = 0;
        
		    for (int k = 0; k < a.nb_column; ++k)
                sum = sum + a.matrix_data[i * a.nb_column + k] * b.matrix_data[k * b.nb_column + j];
         
		   res.matrix_data[i * b.nb_column + j] = sum;
        }
    }
}
    
/*Sums the values in each column i of the matrix matrix and returns this sum at the index i in matrix res*/
void Sum_column(Matrix matrix, Matrix res)
{
	for(int i = 0; i<res.nb_column*res.nb_rows;++i)
		res.matrix_data[i]=0;
	for(int i = 0; i < matrix.nb_rows*matrix.nb_column; ++i)
	{
		res.matrix_data[i%matrix.nb_column] += matrix.matrix_data[i];
	}
}

/*Transposate the matrix matrix, result matrix is stocked in matrix res*/
void Transp_mat(Matrix matrix,Matrix res)
{

	for(int i = 0; i < res.nb_rows * res.nb_column; ++i)
	{
		res.matrix_data[i] = matrix.matrix_data[(i / res.nb_column) + ((i % res.nb_column) * res.nb_rows)];
	}
}

void Sum_bias(Matrix a,Matrix b, Matrix res)
{
	if((a.nb_column != b.nb_column) || (b.nb_rows != 1))
	{
		printf("Incompatible matrix in Sum_bias\n");
		exit(-1);
	}
	
	for(int i = 0; i < a.nb_column * a.nb_rows;++i)
	{
		res.matrix_data[i] = a.matrix_data[i] + b.matrix_data[i % b.nb_column];
	}	
}



/*Multiplies every value in the matrix matrix by a, results are stocked in matrix res (must be of the same size than matrix)*/
void Multip_factor(Matrix matrix, double a,Matrix res)
{
	for(int i =0; i < matrix.nb_column * matrix.nb_rows; ++i)
		res.matrix_data[i]=matrix.matrix_data[i]*a;
}

/*Add the value at index i in matrix a to the value at index i in matrix bm results are stocked in matrix res (the three matrix need to have the same size) */
void Sum_weights(Matrix a,Matrix b,Matrix res)
{

	if((a.nb_column != b.nb_column) || (b.nb_rows != a.nb_rows))
	{
		printf("Incompatible matrix in Sum_weights\n");
		exit(-1);
	}
	
	for(int i = 0; i < a.nb_rows * a.nb_column; ++i)
		res.matrix_data[i] = a.matrix_data[i] + b.matrix_data[i];
}

/*Applies the sigmoid function to every value in matrix a, results are stocked in matrix res*/
void Sigmo_mat(Matrix a,Matrix res)
{
	for(int i = 0;i < a.nb_column * a.nb_rows;++i)
		res.matrix_data[i] = (1/(1+exp((-1)*a.matrix_data[i]))); 
}

/*Applies the derivate sigmoid function to every value in matrix a, results are stocked in matrix res*/
void Sigmo_mat_derivate(Matrix a, Matrix res)
{
	
	for(int i = 0;i < a.nb_column * a.nb_rows;++i)
		res.matrix_data[i] = (a.matrix_data[i] * (1 - a.matrix_data[i])); 
}

/*Multiplies every value at index i in matrix i with the value at index i in matrix b. The result is stocked at index i in matrix res*/
void Mult_simple(Matrix a,Matrix b,Matrix res)
{
	if((a.nb_column != b.nb_column) || (b.nb_rows != a.nb_rows))
	{
		printf("Incompatible matrix in Mult_Simple\n");
		exit(-1);
	}

	for(int i = 0; i< a.nb_rows*a.nb_column;++i)
		res.matrix_data[i] = a.matrix_data[i]*b.matrix_data[i];
}

/*Fills the matrix a with values of b*/
void Fill_mat_data(Matrix a, double b[],int size)
{
	for(int i = 0;i<size;++i)
		a.matrix_data[i]=b[i];
}

/*Starts a training of the neural network on our data set, saves the network at the end of the training in a save file*/
void training(app_widgets *app_wdgts)
{
  Matrix input_template = Init_matrix(400,1);	
  Neural_network Net_train = Init_neural_network(input_template,400,60,89,0);

  unsigned int i = 0;
  while(1)
    {
      if(i > 49)
	break;
       
      if( i== 26 || i ==27 || i == 28|| i == 29 || i == 30 ||i == 34 || i==39 || i ==20 || i ==12 || i ==15  || i ==6 || i == 7  || i ==41  || i ==2||  i == 36 || i == 40 || i == 42 || i == 45 )
	{
	  i++;
	  continue;
        }
	
      FILE *file;
      char num[4];
      sprintf(num,"%d",i);
      char name[30]; 
      char image[40];
      strcpy(name, "./training/texts/");
      strcpy(image, "./training/images/");
      strcat(name, num);
      strcat(image,num);
      strcat(image,".bmp");

      SDL_Surface *surface = SDL_LoadBMP(image);
      if(surface == NULL)
	break;
        
      grayscale(surface);
      blackwhite(surface);
      Matrix matrix = binarize_image(surface);
      Matrix_Array letters = Segmentation2(matrix, surface, app_wdgts);

      file = fopen(name,"r");

      if(file == NULL)
	break;

      char c = fgetc(file);
      int counter = 0;
      char tmp[100000];

      for(; c != EOF; c = fgetc(file))
        {
	  if(c != 0 && c != ' ' && c != '\n')
            {
	      tmp[counter] = c;
	      counter++;
            }
        }

      fclose(file);
        
      char str[counter];

      for(int j = 0; j < counter; j++)
        {
	  str[j] = tmp[j];
        }
      str[counter] = 0;

      if(counter != letters.size)
        {
	  ++i;
	  continue;
        }


      printf("%s", str);
      printf("\n");
      for(int l = 0; l < letters.size; ++l)
	{
	  letters.array_data[l].nb_column *= letters.array_data[l].nb_rows;
	  letters.array_data[l].nb_rows = 1;
		

	  Matrix expected_output = Init_matrix(89,1);
	  expected_output.matrix_data[swap_to_int(str[l])] = 1;
		

	  Net_train = Train_N_n(Net_train,letters.array_data[l],expected_output,2000,0.036, 1);
	  if(find_char(Net_train.final_res) != str[l])
	    {
	      printf("->Res = Fail;\n");
	    }
	  else
	    {
	      printf("->Res = Succeed; error = %.5f\n", (1-Net_train.final_res.matrix_data[swap_to_int(str[l])]));
	    }
	  if(l == (letters.size-1))
	    {  
	      save_data(Net_train.hidden_weight, Net_train.hidden_bias, Net_train.output_weight, Net_train.output_bias);
	      break;
	    }
	}
      //letters = matrix list
      //str = string that contains the letters 
        
      i++;
    }
}

void reload_image(app_widgets *app_wdgts)
{
  SDL_SaveBMP(app_wdgts -> image, "tmp.bmp");
  gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_img_main), "tmp.bmp"); 
}


SDL_Surface* Resize_Image(SDL_Surface *image, int width, int height)
{
  SDL_Surface *new = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
  for(int i = 0; i < width; i++)
    {
      for(int j = 0; j < height; j++)
        {
	  put_pixel(new, i, j, get_pixel(image, (int)(i*((float)image -> w / width)),(int)(j*((float)image -> h / height))));
        }
    }
  return new;
}
int swap_to_int(char c)
{
  if(c < '!' || c > 'z')
    {
      printf("Error in swap_to_int\n");
      return -1;
    }

  return((int) c-'!');
}

char swap_to_char(int i)
{
  return((char)i+'!');
}

char find_char(Matrix k)
{
  double max = k.matrix_data[0];
  int index_max =0;
  for(int i = 1; i < (k.nb_column*k.nb_rows); ++i)
    {
      if(k.matrix_data[i] > max)
	{
	  max = k.matrix_data[i];
	  index_max = i;
	}
    }
  return(swap_to_char(index_max));
}






