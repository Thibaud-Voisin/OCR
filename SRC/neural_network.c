#include "neural_network.h"

Neural_network Init_neural_network(Matrix input, int input_layer_neurons, int hidden_layer_neurons, int output_layer_neurons, int load_data_b)
{
	Neural_network N_n;

	N_n.hidden_weight = Init_matrix(hidden_layer_neurons,input_layer_neurons);

	N_n.output_weight = Init_matrix(output_layer_neurons,hidden_layer_neurons);

	N_n.hidden_bias = Init_matrix(hidden_layer_neurons,1);
	
	N_n.output_bias = Init_matrix(output_layer_neurons,1);
	
	if(!load_data_b)
	{

		Fill_mat_rand_for_output(N_n.output_weight);
		Fill_mat_rand(N_n.hidden_weight);
		
	}
	else
	{
		int digits_input = 0;
		int digits_hidden = 0;	
	
		int i = input_layer_neurons;
		int j = hidden_layer_neurons;
	
		
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
	
		snprintf(str,15,"%d",input_layer_neurons);
	
		int pos = 0;

		for(int k = 0; k <= digits_input; ++k)
		{
			name[k] = str[k];
			++pos;
		}

		name[pos] = '_';
		++pos;

		char str2[10000];
		
		snprintf(str2,15,"%d",hidden_layer_neurons);
	

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
	
		long number_of_char = ((input_layer_neurons*(input_layer_neurons*hidden_layer_neurons))*21)+(input_layer_neurons) + (hidden_layer_neurons*21) + ((hidden_layer_neurons*output_layer_neurons)*21)+(hidden_layer_neurons) + (output_layer_neurons*21) + 4 ;

		char *str_f = calloc(number_of_char ,sizeof(char));

		char path_r[300] = "training/save/";

		strcat(path_r,name);

		FILE *file;
		
		file = fopen(path_r,"r");

		if(file == NULL)
		{	
			printf("error on read save");
			exit(0);
		}

		while(fgets(str_f,number_of_char, file) != NULL)
		
		load_data(str_f, N_n.hidden_weight,N_n.output_weight,N_n.hidden_bias,N_n.output_bias);
		
		fclose(file);
	}


	N_n.res_hidden_layer = Init_matrix(N_n.hidden_weight.nb_column,input.nb_rows);
		

	N_n.hidden_layer_output = Init_matrix(N_n.res_hidden_layer.nb_column,N_n.res_hidden_layer.nb_rows);
	
	N_n.res_output_layer = Init_matrix(N_n.output_weight.nb_column,N_n.hidden_layer_output.nb_rows);

	N_n.final_res = Init_matrix(N_n.res_output_layer.nb_column,N_n.res_output_layer.nb_rows);

	N_n.error_multip_factor = Init_matrix(N_n.final_res.nb_column,N_n.final_res.nb_rows);

	N_n.error = Init_matrix(N_n.error_multip_factor.nb_column,N_n.error_multip_factor.nb_rows);

	N_n.back_final_res_sigmo = Init_matrix(N_n.final_res.nb_column,N_n.final_res.nb_rows);
		
	N_n.back_final_res = Init_matrix(N_n.error.nb_column,N_n.error.nb_rows);

	N_n.error_hidden_layer_transp = Init_matrix(N_n.output_weight.nb_rows,N_n.output_weight.nb_column);

	N_n.error_hidden_layer = Init_matrix(N_n.error_hidden_layer_transp.nb_column,N_n.back_final_res.nb_rows);

	N_n.back_hidden_layer_sigmod = Init_matrix(N_n.hidden_layer_output.nb_column,N_n.hidden_layer_output.nb_rows);

	N_n.back_hidden_layer = Init_matrix(N_n.error_hidden_layer.nb_column,N_n.error_hidden_layer.nb_rows);

	N_n.output_weight_back_transp = Init_matrix(N_n.hidden_layer_output.nb_rows,N_n.hidden_layer_output.nb_column);

	N_n.output_weight_back_multmath = Init_matrix(N_n.back_final_res.nb_column,N_n.output_weight_back_transp.nb_rows);

	N_n.output_weight_back_multfac = Init_matrix(N_n.output_weight_back_multmath.nb_column,N_n.output_weight_back_multmath.nb_rows);
	
	N_n.output_bias_back_sumcol = Init_matrix(N_n.back_final_res.nb_column,1);

	N_n.output_bias_back_multifac = Init_matrix(N_n.output_bias_back_sumcol.nb_column,N_n.output_bias_back_sumcol.nb_rows);

	N_n.hidden_weight_back_transp = Init_matrix(input.nb_rows,input.nb_column);

	N_n.hidden_weight_back_multmath = Init_matrix(N_n.back_hidden_layer.nb_column,N_n.hidden_weight_back_transp.nb_rows);
		
	N_n.hidden_weight_back_multfac = Init_matrix(N_n.hidden_weight_back_multmath.nb_column,N_n.hidden_weight_back_multmath.nb_rows);

	N_n.hidden_bias_back_sumcol = Init_matrix(N_n.back_hidden_layer.nb_column,1);

	N_n.hidden_bias_back_multfact = Init_matrix(N_n.hidden_bias_back_sumcol.nb_column,N_n.hidden_bias_back_sumcol.nb_rows);

	return N_n;
}

char Evaluate_char(Neural_network N_n,Matrix input)
{

			Mult_mat_1(input, N_n.hidden_weight,N_n.res_hidden_layer);

			Sum_bias(N_n.res_hidden_layer,N_n.hidden_bias,N_n.res_hidden_layer);
	
			Sigmo_mat(N_n.res_hidden_layer,N_n.hidden_layer_output);
			
//		Pretty_print_xor(N_n.hidden_layer_output);


		Mult_mat_1(N_n.hidden_layer_output,N_n.output_weight,N_n.res_output_layer);
		
		Sum_bias(N_n.res_output_layer,N_n.output_bias,N_n.res_output_layer);
		
		Sigmo_mat(N_n.res_output_layer,N_n.final_res);
	

		return find_char(N_n.final_res);
}

Neural_network Train_N_n(Neural_network N_n,Matrix input, Matrix expected_output, unsigned long nb_rep,double precision, int do_back)
{
/*	Pretty_print_xor(input);
	Pretty_print(expected_output);

	for(;nb_rep > 0;--nb_rep)
	{
			Mult_mat_1(input, N_n.hidden_weight,N_n.res_hidden_layer);

printf("\n================\n");	
			Pretty_print_xor(N_n.res_hidden_layer);

			Sum_bias(N_n.res_hidden_layer,N_n.hidden_bias,N_n.res_hidden_layer);

printf("\n=====1===========\n");	
			Pretty_print_xor(N_n.res_hidden_layer);
	
			Sigmo_mat(N_n.res_hidden_layer,N_n.hidden_layer_output);

printf("\n======2==========\n");	
			Pretty_print_xor(N_n.hidden_layer_output);


printf("\n=======3=========\n");	

		Mult_mat_1(N_n.hidden_layer_output,N_n.output_weight,N_n.res_output_layer);
		
printf("\n========4========\n");	
	Pretty_print_xor(N_n.res_output_layer);
		Sum_bias(N_n.res_output_layer,N_n.output_bias,N_n.res_output_layer);
		
printf("\n======5==========\n");	
	Pretty_print_xor(N_n.res_output_layer);
		Sigmo_mat(N_n.res_output_layer,N_n.final_res);
		
printf("\n=======6=========\n");	
	Pretty_print_xor(N_n.final_res);
	//BACKPROPAGATION

		Multip_factor(N_n.final_res,(-1),N_n.error_multip_factor);
		
printf("\n======7==========\n");	
	Pretty_print_xor(N_n.error_multip_factor);
		Sum_weights(N_n.error_multip_factor,expected_output,N_n.error);	
		
printf("\n=======8=========\n");	
	Pretty_print_xor(N_n.error);
		Sigmo_mat_derivate(N_n.final_res,N_n.back_final_res_sigmo);
		
printf("\n========9========\n");	
	Pretty_print_xor(N_n.back_final_res_sigmo);
		Mult_simple(N_n.error,N_n.back_final_res_sigmo,N_n.back_final_res);
		
printf("\n=========10=======\n");	
	Pretty_print_xor(N_n.back_final_res);
		Transp_mat(N_n.output_weight,N_n.error_hidden_layer_transp);
		
printf("\n===========11=====\n");	
	Pretty_print_xor(N_n.error_hidden_layer_transp);
		Mult_mat_1(N_n.back_final_res,N_n.error_hidden_layer_transp,N_n.error_hidden_layer);
		
printf("\n====12============\n");	
	Pretty_print_xor(N_n.error_hidden_layer);
		Sigmo_mat_derivate(N_n.hidden_layer_output,N_n.back_hidden_layer_sigmod);
	

printf("\n======13==========\n");	
	Pretty_print_xor(N_n.back_hidden_layer_sigmod);
		Mult_simple(N_n.error_hidden_layer,N_n.back_hidden_layer_sigmod,N_n.back_hidden_layer);
		
printf("\n========14========\n");	
	Pretty_print_xor(N_n.back_hidden_layer);
		Transp_mat(N_n.hidden_layer_output,N_n.output_weight_back_transp);
		
printf("\n==========15======\n");	
	Pretty_print_xor(N_n.output_weight_back_transp);
		Mult_mat_1(N_n.output_weight_back_transp,N_n.back_final_res,N_n.output_weight_back_multmath);

printf("\n=======16=========\n");	
		Pretty_print_xor(N_n.output_weight_back_multmath);
		
		Multip_factor(N_n.output_weight_back_multmath,precision,N_n.output_weight_back_multfac);
		
printf("\n=========17=======\n");	
	Pretty_print_xor(N_n.output_weight_back_multfac);
		

		Sum_weights(N_n.output_weight,N_n.output_weight_back_multfac,N_n.output_weight);
		
printf("\n======18==========\n");	
	Pretty_print_xor(N_n.output_weight);
		Sum_column(N_n.back_final_res,N_n.output_bias_back_sumcol);
		
printf("\n========19========\n");	
	Pretty_print_xor(N_n.output_bias_back_sumcol);
		Multip_factor(N_n.output_bias_back_sumcol,precision,N_n.output_bias_back_multifac);
		
printf("\n=======20=========\n");	
	Pretty_print_xor(N_n.output_bias_back_multifac);
		Sum_bias(N_n.output_bias,N_n.output_bias_back_multifac,N_n.output_bias);
		
printf("\n=======21=========\n");	
	Pretty_print_xor(N_n.output_bias);
		
		Transp_mat(input,N_n.hidden_weight_back_transp);
		
printf("\n=======22=========\n");	
	Pretty_print_xor(N_n.hidden_weight_back_transp);
		Mult_mat_1(N_n.hidden_weight_back_transp,N_n.back_hidden_layer,N_n.hidden_weight_back_multmath);
		
printf("\n========23========\n");	
	Pretty_print_xor(N_n.hidden_weight_back_multmath);
		Multip_factor(N_n.hidden_weight_back_multmath,precision,N_n.hidden_weight_back_multfac);
		
printf("\n=======24=========\n");	
	Pretty_print_xor(N_n.hidden_weight_back_multfac);
		Sum_weights(N_n.hidden_weight,N_n.hidden_weight_back_multfac,N_n.hidden_weight);
			
printf("\n=========25=======\n");	
	Pretty_print_xor(N_n.hidden_weight);
		
		
		Sum_column(N_n.back_hidden_layer,N_n.hidden_bias_back_sumcol);
		
printf("\n=======26=========HELPPPPPPPP\n");	
	Pretty_print_xor(N_n.hidden_bias_back_sumcol);
		Multip_factor(N_n.hidden_bias_back_sumcol,precision,N_n.hidden_bias_back_multfact);
		
printf("\n=========27=======\n");	
	Pretty_print_xor(N_n.hidden_bias_back_multfact);
		Sum_bias(N_n.hidden_bias,N_n.hidden_bias_back_multfact,N_n.hidden_bias);
	
printf("\n=======28=========\n");	
	Pretty_print_xor(N_n.hidden_bias);
	}
*/
	for(;nb_rep > 0;--nb_rep)
	{
			Mult_mat_1(input, N_n.hidden_weight,N_n.res_hidden_layer);

			Sum_bias(N_n.res_hidden_layer,N_n.hidden_bias,N_n.res_hidden_layer);
	
			Sigmo_mat(N_n.res_hidden_layer,N_n.hidden_layer_output);
			
//		Pretty_print_xor(N_n.hidden_layer_output);


		Mult_mat_1(N_n.hidden_layer_output,N_n.output_weight,N_n.res_output_layer);
		
		Sum_bias(N_n.res_output_layer,N_n.output_bias,N_n.res_output_layer);
		
		Sigmo_mat(N_n.res_output_layer,N_n.final_res);
		
	//BACKPROPAGATION

		if(do_back)
		{
		Multip_factor(N_n.final_res,(-1),N_n.error_multip_factor);
		
		Sum_weights(N_n.error_multip_factor,expected_output,N_n.error);	
		
		Sigmo_mat_derivate(N_n.final_res,N_n.back_final_res_sigmo);
		
		Mult_simple(N_n.error,N_n.back_final_res_sigmo,N_n.back_final_res);
		
		Transp_mat(N_n.output_weight,N_n.error_hidden_layer_transp);
		
		Mult_mat_1(N_n.back_final_res,N_n.error_hidden_layer_transp,N_n.error_hidden_layer);
		
		Sigmo_mat_derivate(N_n.hidden_layer_output,N_n.back_hidden_layer_sigmod);
		
		Mult_simple(N_n.error_hidden_layer,N_n.back_hidden_layer_sigmod,N_n.back_hidden_layer);
		
		Transp_mat(N_n.hidden_layer_output,N_n.output_weight_back_transp);
		
		Mult_mat_1(N_n.output_weight_back_transp,N_n.back_final_res,N_n.output_weight_back_multmath);
		
		Multip_factor(N_n.output_weight_back_multmath,precision,N_n.output_weight_back_multfac);
//				Pretty_print_xor(N_n.output_weight_back_multfac);	
		

		Sum_weights(N_n.output_weight,N_n.output_weight_back_multfac,N_n.output_weight);
		
		Sum_column(N_n.back_final_res,N_n.output_bias_back_sumcol);
		
		Multip_factor(N_n.output_bias_back_sumcol,precision,N_n.output_bias_back_multifac);
//			Pretty_print_xor(N_n.output_bias_back_multifac);	

		Sum_bias(N_n.output_bias,N_n.output_bias_back_multifac,N_n.output_bias);
		
		
		Transp_mat(input,N_n.hidden_weight_back_transp);
		
		Mult_mat_1(N_n.hidden_weight_back_transp,N_n.back_hidden_layer,N_n.hidden_weight_back_multmath);
		
		Multip_factor(N_n.hidden_weight_back_multmath,precision,N_n.hidden_weight_back_multfac);
//				Pretty_print_xor(N_n.hidden_weight_back_multfac);	
		Sum_weights(N_n.hidden_weight,N_n.hidden_weight_back_multfac,N_n.hidden_weight);
			
		
		
		Sum_column(N_n.back_hidden_layer,N_n.hidden_bias_back_sumcol);
		
		Multip_factor(N_n.hidden_bias_back_sumcol,precision,N_n.hidden_bias_back_multfact);
//		Pretty_print_xor(N_n.hidden_bias_back_multfact);	
		Sum_bias(N_n.hidden_bias,N_n.hidden_bias_back_multfact,N_n.hidden_bias);
	
		}
	}

		//save_data(N_n.hidden_weight ,N_n.hidden_bias ,N_n.output_weight ,N_n.output_bias);
	
//		printf("\n\n");
//		printf("\n\n");
		printf("%c", find_char(N_n.final_res));
		fflush(stdout);	
		return N_n;
}

void Free_Neural_network(Neural_network N_n)
{

	free(N_n.hidden_weight.matrix_data);
	free(N_n.output_weight.matrix_data);
	free(N_n.hidden_bias.matrix_data);
	free(N_n.output_bias.matrix_data);
	free(N_n.res_hidden_layer.matrix_data);
	free(N_n.hidden_layer_output.matrix_data);
	free(N_n.res_output_layer.matrix_data);
	free(N_n.final_res.matrix_data);
	free(N_n.error_multip_factor.matrix_data);
	free(N_n.error.matrix_data);
	free(N_n.back_final_res_sigmo.matrix_data);
	free(N_n.back_final_res.matrix_data);
	free(N_n.error_hidden_layer_transp.matrix_data);
	free(N_n.error_hidden_layer.matrix_data);
	free(N_n.back_hidden_layer_sigmod.matrix_data);
	free(N_n.back_hidden_layer.matrix_data);
	free(N_n.output_weight_back_transp.matrix_data);
	free(N_n.output_weight_back_multmath.matrix_data);
	free(N_n.output_weight_back_multfac.matrix_data);
	free(N_n.output_bias_back_sumcol.matrix_data);
	free(N_n.output_bias_back_multifac.matrix_data);
	free(N_n.hidden_weight_back_transp.matrix_data);
	free(N_n.hidden_weight_back_multmath.matrix_data);		
	free(N_n.hidden_weight_back_multfac.matrix_data); 
	free(N_n.hidden_bias_back_sumcol.matrix_data); 
	free(N_n.hidden_bias_back_multfact.matrix_data); 
}
