#include "tools.h"

void Write_hidden_weights(Matrix hidden_weights, Matrix hidden_bias, Matrix output_weights, Matrix output_bias)
{
	char *str_final = calloc(((hidden_weights.nb_column*hidden_weights.nb_rows)*21)+(hidden_weights.nb_column) + (hidden_bias.nb_column*21) + ((output_weights.nb_column*output_weights.nb_rows)*21)+(output_weights.nb_column) + (output_bias.nb_column*21) + 4, sizeof(char));
	
	int pos_str_final = 0;

	for(int i = 0; i < hidden_weights.nb_column; ++i)
	{
		for(int j = 0; j < hidden_weights.nb_rows; ++j)
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
			str_final[pos_str_final] = ';';
			++pos_str_final;
		}
		str_final[pos_str_final] = '\n';
		++pos_str_final;
	}
	
	str_final[pos_str_final] = '!';
	++pos_str_final;
	
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
		str_final[pos_str_final] = ';';
		++pos_str_final;
	}
	
	str_final[pos_str_final] = '!';
	++pos_str_final;

	for(int i = 0; i < output_weights.nb_column; ++i)
	{
		for(int j = 0; j < output_weights.nb_rows; ++j)
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
			str_final[pos_str_final] = ';';
			++pos_str_final;
		}
		str_final[pos_str_final] = '\n';
		++pos_str_final;
	}
	
	str_final[pos_str_final] = '!';
	++pos_str_final;
	
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
		str_final[pos_str_final] = ';';
		++pos_str_final;
	}
		
	str_final[pos_str_final] = '!';
	++pos_str_final;
	
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

	
	file = fopen(name,"w");

	if(file == NULL)
		printf("ERROR\n");
	fprintf(file,"%s", str_final );

	fclose(file);

}


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
	int width_pretty = 1+(11*matrix.nb_column);
	
	char *top=(char *) calloc(width_pretty+1,sizeof(char));
	
	memset(top, '-', width_pretty);	
	
	int top_i = 0;
	int top_j = 0;

	while(*(top + top_i)!='\0')
        printf("%c",*(top + top_i++));
	printf("\n");

	char *middle=(char *) calloc(width_pretty+1,sizeof(char));
	
	int j = 0;
	char tmp[15];

	for(int i = 0;i<matrix.nb_column * matrix.nb_rows;++i)
	{	
		sprintf(tmp,"%.8f",matrix.matrix_data[i]);
		for (int k = 0;k<10;++k)
		{
			memset((middle+j+k),tmp[k],1);	
		}
		middle[j+10] = '|';
		j+=11;

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
    

void Sum_column(Matrix matrix, Matrix res)
{
	for(int i = 0; i<res.nb_column*res.nb_rows;++i)
		res.matrix_data[i]=0;
	for(int i = 0; i < matrix.nb_rows*matrix.nb_column; ++i)
	{
		res.matrix_data[i%matrix.nb_column] += matrix.matrix_data[i];
	}
}

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

void Multip_factor(Matrix matrix, double a,Matrix res)
{
	for(int i =0; i < matrix.nb_column * matrix.nb_rows; ++i)
		res.matrix_data[i]=matrix.matrix_data[i]*a;
}

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

void Sigmo_mat(Matrix a,Matrix res)
{
	for(int i = 0;i < a.nb_column * a.nb_rows;++i)
		res.matrix_data[i] = (1/(1+exp((-1)*a.matrix_data[i]))); 
}

void Sigmo_mat_derivate(Matrix a, Matrix res)
{
	
	for(int i = 0;i < a.nb_column * a.nb_rows;++i)
		res.matrix_data[i] = (a.matrix_data[i] * (1 - a.matrix_data[i])); 
}

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

void Fill_mat_data(Matrix a, double b[],int size)
{
	for(int i = 0;i<size;++i)
		a.matrix_data[i]=b[i];
}



