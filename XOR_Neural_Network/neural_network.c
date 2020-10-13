#include "neural_network.h"

int main()
{
	srand(time(NULL));	
	

	Matrix input = Init_matrix(2,4);
	double data_input[8] = {0,0,0,1,1,0,1,1};
	Fill_mat_data(input,data_input,8);
	

	Matrix expected_output = Init_matrix(1,4);
	double data_expected_output[4] = {0,1,1,0};
	Fill_mat_data(expected_output,data_expected_output,4);

	
	long nb_rep = 10000;
	double precision = 0.1;

	int input_layer_neurons = 2;
	int hidden_layer_neurons = 2;
	int output_layer_neurons = 1; 

	Matrix hidden_weight = Init_matrix(input_layer_neurons,hidden_layer_neurons);
	Fill_mat_rand(hidden_weight);
	Pretty_print(hidden_weight);

	Matrix hidden_bias = Init_matrix(hidden_layer_neurons,1);
	Fill_mat_rand(hidden_bias);
	Pretty_print(hidden_bias);
	Matrix output_weight = Init_matrix(output_layer_neurons,hidden_layer_neurons);
	Fill_mat_rand(output_weight);
	Pretty_print(output_weight);
	Matrix output_bias = Init_matrix(output_layer_neurons,1);
	Fill_mat_rand(output_bias);	
	Pretty_print(output_bias);


	for(;nb_rep > 0;--nb_rep)
	{
		printf("1\n");
		Matrix res_hidden_layer = Mult_mat_1(input,hidden_weight);
		Matrix res_hidden_layer_bias = Sum_bias(res_hidden_layer,hidden_bias);

		Matrix hidden_layer_output = res_hidden_layer_bias;
		Sigmo_mat(hidden_layer_output);
		printf("2\n");
		Pretty_print(output_weight);
		Pretty_print(hidden_layer_output);	
		Matrix output_layer_res = Mult_mat_1(hidden_layer_output,output_weight);
		Matrix output_layer_res_bias = Sum_bias(output_layer_res,output_bias);
		Matrix final_res = Init_matrix(output_layer_res_bias.nb_column,output_layer_res_bias.nb_rows);
		
		//BACKPROPAGATION
	
		final_res = Multip_factor(final_res,(-1));	
		Matrix error = Sum_weights(final_res,expected_output);
		Matrix hidden_layer_output_sigmod = final_res;
		Sigmo_mat_derivate(hidden_layer_output_sigmod);
		Matrix back_final_res = Mult_simple(error,hidden_layer_output_sigmod);
		printf("3\n");	
		Matrix error_hidden_layer = Mult_mat_1(back_final_res,Transp_mat(output_weight));

		Sigmo_mat_derivate(hidden_layer_output);
		
		Matrix back_hidden_layer = Mult_simple(error_hidden_layer,hidden_layer_output); 
		printf("4\n");
		Pretty_print(output_weight);
		Pretty_print(Mult_mat_1(Transp_mat(hidden_layer_output),back_final_res));
		printf("4bis\n");
		Pretty_print(Multip_factor(Mult_mat_1(Transp_mat(hidden_layer_output),back_final_res),precision));
		output_weight = Sum_bias(output_weight,Multip_factor(Mult_mat_1(Transp_mat(hidden_layer_output),back_final_res),precision)); 
		printf("5\n");
		output_bias = Sum_bias(output_bias,Multip_factor(Sum_column(back_final_res),precision));
		printf("6\n");
		hidden_weight = Sum_bias(hidden_weight,Multip_factor(Mult_mat_1(Transp_mat(input),back_hidden_layer),precision));
		printf("7\n");
		hidden_bias = Sum_bias(hidden_bias,Multip_factor(Sum_column(back_final_res),precision));
			
	}	

	return 0;
}



struct Neural_Network
{
	//TODO
};










