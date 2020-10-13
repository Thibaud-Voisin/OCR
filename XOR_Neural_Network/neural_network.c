#include "neural_network.h"

int main()
{
	srand(time(NULL));	
	

	Matrix input = Init_matrix(2,4);
	input.matrix_data = [0,0,0,1,1,0,1,1];
	

	Matrix expected_output = Init_matrix(1,4);
	expected_output.matrix_data = [0,1,1,0];
	
	long nb_rep = 10000;
	double precision = 0.1;

	int input_layer_neurons = 2;
	int hidden_layer_neurons = 2;
	int output_layer_neurons = 1; 

	Matrix hidden_weight = Init_matrix(input_layer_neurons,hidden_layer_neurons);
	Fill_mat_rand(hidden_weight);
	Matrix hidden_bias = Init_matrix(hidden_layer_neurons,1);
	Fill_mat_rand(bias);
	Matrix output_weight = Init_matrix(hidde_layer_neurons,output_layer_neurons);
	Fill_mat_rand(output_weight);
	Matrix output_bias = Init_matrix(output_layer_neurons,1);

	for(;nb_rep > 0;--nb_rep)
	{
		Matrix res_hidden_layer = Mult_mat_1(input,hidden_layer);
		Matrix res_hidden_layer_bias = Sum_bias(res_hidden_layer,hidden_bias);

		Matrix hidden_layer_output = res_hidden_layer;
		Sigmo_mat(hidden_layer_output);
		
		Matrix output_layer_res = Mult_mat_1(hidden_layer_output, output_weight);
		Matrix output_layer_res_bias = Sum_bias(output_layer_res,output_bias);
		Matrix final_res = Init_matrix(output_layer_res_bias.nb_column,output_layer_res_bias.nb_rows);
		
		//BACKPROPAGATION
		
		Matrix error = Sum_weights(Multip_facto(final_res, (-1)),expected_output);
		Matrix hidden_layer_output_sigmod = final_res;
		Sigmod_mat_derivate(hidden_layer_output_sigmod);
		Matrix back_final_res = Mult_simple(error,hidden_layer_output_sigmod);
			
		Matrix error_hidden_layer = Mult_mat_1(back_final_res,Transp_mat(output_weight));

		Sigmo_mat_derivate(hidden_layer_output);
		
		Matrix back_hidden_layer = Mult_simple(error_hidden_layer,hidden_layer_output); 

		output_weight = Sum_bias(output_weight,Mult_factor(Mult_mat_1(Transp_mat(hidden_layer_output),back_final_res)),precision); 

		output_bias = Sum_bias(output_bias,Mult_factor(Sum_column(back_final_res),precision));

		hidden_weight = Sum_bias(hidden_weight,Mult_factor(Mult_mat_1(Transp_mat(inputs),back_hidden_layer),precision));

		hidden_bias = Sum_bias(hidden_bias,Mult_factor(Sum_column(back_final_res),precision));
			
	}	

	return 0;
}



struct Neural_Network
{
	//TODO
};










