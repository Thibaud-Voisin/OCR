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
		printf("Number of steps = %ld\n",(10000 - nb_rep));
	
		Matrix res_hidden_layer = Mult_mat_1(input,hidden_weight);
		
		res_hidden_layer = Sum_bias(res_hidden_layer,hidden_bias);

		Matrix hidden_layer_output = Sigmo_mat(res_hidden_layer);

		
		Matrix res_output_layer = Mult_mat_1(hidden_layer_output,output_weight);
		
		res_output_layer = Sum_bias(res_output_layer,output_bias);
		
		Matrix final_res = Sigmo_mat(res_output_layer);
		

		//BACKPROPAGATION
		Matrix error = Sum_weights(Multip_factor(final_res,(-1)),expected_output);
		
		Matrix back_final_res = Mult_simple(error,Sigmo_mat(final_res));	

		Matrix error_hidden_layer = Mult_mat_1(back_final_res,Transp_mat(output_weight));

				
		Matrix back_hidden_layer = Mult_simple(error_hidden_layer,Sigmo_mat_derivate(hidden_layer_output));
 
		output_weight = Sum_weights(output_weight,Multip_factor(Mult_mat_1(Transp_mat(hidden_layer_output),back_final_res),precision)); 
		output_bias = Sum_bias(output_bias,Multip_factor(Sum_column(back_final_res),precision));
		
		hidden_weight = Sum_weights(hidden_weight,Multip_factor(Mult_mat_1(Transp_mat(input),back_hidden_layer),precision));
		
		hidden_bias = Sum_weights(hidden_bias,Multip_factor(Sum_column(back_hidden_layer),precision));
			
	}	

	return 0;
}



struct Neural_Network
{
	//TODO
};










