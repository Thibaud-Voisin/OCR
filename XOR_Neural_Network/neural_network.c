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

	
	long nb_rep = 10;
	double precision = 1;

	int input_layer_neurons = 2;
	int hidden_layer_neurons = 2;
	int output_layer_neurons = 1; 

	Matrix hidden_weight = Init_matrix(input_layer_neurons,hidden_layer_neurons);
//	Fill_mat_rand(hidden_weight);
	double data_hidden_weight[4] = {0.342,0.6178,0.4376,0.9499};
	Fill_mat_data(hidden_weight,data_hidden_weight,4);
	Pretty_print(hidden_weight);

	Matrix hidden_bias = Init_matrix(hidden_layer_neurons,1);
//	Fill_mat_rand(hidden_bias);
	double data_hidden_bias[2] = {0.865,0.7432};
	Fill_mat_data(hidden_bias,data_hidden_bias,2);
	Pretty_print(hidden_bias);

	Matrix output_weight = Init_matrix(output_layer_neurons,hidden_layer_neurons);
//	Fill_mat_rand(output_weight);
	double data_output_weight[2] = {0.563,0.8539};
	Fill_mat_data(output_weight,data_output_weight,2);
	Pretty_print(output_weight);

	Matrix output_bias = Init_matrix(output_layer_neurons,1);
//	Fill_mat_rand(output_bias);
	double data_output_bias[1] = {0.4321};
	Fill_mat_data(output_bias,data_output_bias,1);
	Pretty_print(output_bias);

	Matrix res_hidden_layer = Init_matrix(hidden_weight.nb_column,input.nb_rows);
		
	//Matrix res_hidden_layer = Init_matrix(res_hidden_layer.nb_column,res_hidden_layer.nb_rows);

	Matrix hidden_layer_output = Init_matrix(res_hidden_layer.nb_column,res_hidden_layer.nb_rows);
	
	Matrix res_output_layer = Init_matrix(output_weight.nb_column,hidden_layer_output.nb_rows);

	Matrix final_res = Init_matrix(res_output_layer.nb_column,res_output_layer.nb_rows);

	Matrix error_multip_factor = Init_matrix(final_res.nb_column,final_res.nb_rows);

	Matrix error = Init_matrix(error_multip_factor.nb_column,error_multip_factor.nb_rows);

	Matrix back_final_res_sigmo = Init_matrix(final_res.nb_column,final_res.nb_rows);
		
	Matrix back_final_res = Init_matrix(error.nb_column,error.nb_rows);

	Matrix error_hidden_layer_transp = Init_matrix(output_weight.nb_rows,output_weight.nb_column);

	Matrix error_hidden_layer = Init_matrix(error_hidden_layer_transp.nb_column,back_final_res.nb_rows);

	Matrix back_hidden_layer_sigmod = Init_matrix(hidden_layer_output.nb_column,hidden_layer_output.nb_rows);

	Matrix back_hidden_layer = Init_matrix(error_hidden_layer.nb_column,error_hidden_layer.nb_rows);

	Matrix output_weight_back_transp = Init_matrix(hidden_layer_output.nb_rows,hidden_layer_output.nb_column);

	Matrix output_weight_back_multmath = Init_matrix(back_final_res.nb_column,output_weight_back_transp.nb_rows);

	Matrix output_weight_back_multfac = Init_matrix(output_weight_back_multmath.nb_column,output_weight_back_multmath.nb_rows);
	
	Matrix output_bias_back_sumcol = Init_matrix(back_final_res.nb_column,1);

	Matrix output_bias_back_multifac = Init_matrix(output_bias_back_sumcol.nb_column,output_bias_back_sumcol.nb_rows);

	Matrix hidden_weight_back_transp = Init_matrix(input.nb_rows,input.nb_column);

	Matrix hidden_weight_back_multmath = Init_matrix(back_hidden_layer.nb_column,hidden_weight_back_transp.nb_rows);
		
	Matrix hidden_weight_back_multfac = Init_matrix(hidden_weight_back_multmath.nb_column,hidden_weight_back_multmath.nb_rows);

	Matrix hidden_bias_back_sumcol = Init_matrix(back_hidden_layer.nb_column,1);

	Matrix hidden_bias_back_multfact = Init_matrix(hidden_bias_back_sumcol.nb_column,hidden_bias_back_sumcol.nb_rows);





	for(;nb_rep > 0;--nb_rep)
	{
	//	printf("Number of steps = %ld\n",(10000 - nb_rep));
		Mult_mat_1(input,hidden_weight,res_hidden_layer);
		Sum_bias(res_hidden_layer,hidden_bias,res_hidden_layer);
		Sigmo_mat(res_hidden_layer,hidden_layer_output);
	
		Mult_mat_1(hidden_layer_output,output_weight,res_output_layer);
		Sum_bias(res_output_layer,output_bias,res_output_layer);
		Sigmo_mat(res_output_layer,final_res);
			
		Pretty_print(final_res);	
		//BACKPROPAGATION
		Multip_factor(final_res,(-1),error_multip_factor);
	
		Sum_weights(error_multip_factor,expected_output,error);	

//		Pretty_print(error);

		Sigmo_mat_derivate(final_res,back_final_res_sigmo);
		Mult_simple(error,back_final_res_sigmo,back_final_res);
//		Pretty_print(back_final_res);
		Transp_mat(output_weight,error_hidden_layer_transp);
		Mult_mat_1(back_final_res,error_hidden_layer_transp,error_hidden_layer);

	
		Sigmo_mat_derivate(hidden_layer_output,back_hidden_layer_sigmod);
		Mult_simple(error_hidden_layer,back_hidden_layer_sigmod,back_hidden_layer);





		Transp_mat(hidden_layer_output,output_weight_back_transp);
	
	
		Mult_mat_1(output_weight_back_transp,back_final_res,output_weight_back_multmath);
		
		Multip_factor(output_weight_back_multmath,precision,output_weight_back_multfac);
		
		Sum_weights(output_weight,output_weight_back_multfac,output_weight);

	


		Sum_column(back_final_res,output_bias_back_sumcol);

		Multip_factor(output_bias_back_sumcol,precision,output_bias_back_multifac);

		Sum_bias(output_bias,output_bias_back_multifac,output_bias);

		

		Transp_mat(input,hidden_weight_back_transp);

		Mult_mat_1(hidden_weight_back_transp,back_hidden_layer,hidden_weight_back_multmath);

		Multip_factor(hidden_weight_back_multmath,precision,hidden_weight_back_multfac);
		Sum_weights(hidden_weight,hidden_weight_back_multfac,hidden_weight);
			
	
		
		Sum_column(back_hidden_layer,hidden_bias_back_sumcol);

		Multip_factor(hidden_bias_back_sumcol,precision,hidden_bias_back_multfact);
		Sum_weights(hidden_bias,hidden_bias_back_multfact,hidden_bias);
	}	
	
	return 0;
}



struct Neural_Network
{
	//TODO
};










