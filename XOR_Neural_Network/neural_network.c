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

	Matrix hidden_bias = Init_matrix(hidden_layer_neurons,1);
	Fill_mat_rand(hidden_bias);
	Matrix output_weight = Init_matrix(output_layer_neurons,hidden_layer_neurons);
	Fill_mat_rand(output_weight);
	Matrix output_bias = Init_matrix(output_layer_neurons,1);
	Fill_mat_rand(output_bias);	


	for(;nb_rep > 0;--nb_rep)
	{
		Matrix res_hidden_layer = Mult_mat_1(input,hidden_weight);
		Matrix res_hidden_layer_bias = Sum_bias(res_hidden_layer,hidden_bias);

		Matrix hidden_layer_output = res_hidden_layer_bias;
		Sigmo_mat(hidden_layer_output);
		Matrix output_layer_res = Mult_mat_1(hidden_layer_output,output_weight);
		Matrix output_layer_res_bias = Sum_bias(output_layer_res,output_bias);
		Matrix final_res = output_layer_res_bias;
		Sigmo_mat(final_res);
		//BACKPROPAGATION
		Pretty_print(final_res);	
		Matrix final_res_neg = Multip_factor(final_res,(-1));	
		Pretty_print(final_res_neg);
		Pretty_print(expected_output);
		Matrix error = Sum_weights(final_res_neg,expected_output);
		Pretty_print(error);
		Matrix hidden_layer_output_sigmod = final_res;
		Sigmo_mat_derivate(hidden_layer_output_sigmod);
		Matrix back_final_res = Mult_simple(error,hidden_layer_output_sigmod);
		
		Pretty_print(back_final_res);
		printf("Hey");

		Pretty_print(Transp_mat(output_weight));
		Matrix error_hidden_layer = Mult_mat_1(back_final_res,Transp_mat(output_weight));
		Pretty_print(error_hidden_layer);


		Sigmo_mat_derivate(hidden_layer_output);
		
		Matrix back_hidden_layer = Mult_simple(error_hidden_layer,hidden_layer_output); 
		Pretty_print(output_weight);
		printf("4bis\n");
		Pretty_print(Multip_factor(Mult_mat_1(Transp_mat(hidden_layer_output),back_final_res),precision));
		output_weight = Sum_weights(output_weight,Multip_factor(Mult_mat_1(Transp_mat(hidden_layer_output),back_final_res),precision)); 
		printf("5\n");
		output_bias = Sum_bias(output_bias,Multip_factor(Sum_column(back_final_res),precision));
		printf("6\n");
		hidden_weight = Sum_weights(hidden_weight,Multip_factor(Mult_mat_1(Transp_mat(input),back_hidden_layer),precision));
		printf("7\n");
		Pretty_print(hidden_bias);
		
		Pretty_print(Multip_factor(Sum_column(back_final_res),precision));
		hidden_bias = Sum_weights(hidden_bias,Multip_factor(Sum_column(back_final_res),precision));
			
	}	

	return 0;
}



struct Neural_Network
{
	//TODO
};










