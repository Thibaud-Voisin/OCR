#include "neural_network.h"

void Neural_network(unsigned long rep,double pres)
{
	Matrix input = Init_matrix(2,4);
	double data_input[8] = {0,0,0,1,1,0,1,1};
	Fill_mat_data(input,data_input,8);
	

	Matrix expected_output = Init_matrix(1,4);
	double data_expected_output[4] = {0,1,1,0};
	Fill_mat_data(expected_output,data_expected_output,4);

	
	unsigned long nb_rep = rep;
	double precision = pres;

	int input_layer_neurons = 2;
	int hidden_layer_neurons = 40;
	int output_layer_neurons = 1; 

	Matrix hidden_weight = Init_matrix(hidden_layer_neurons,input_layer_neurons);
	Fill_mat_rand(hidden_weight);
	Matrix output_weight = Init_matrix(output_layer_neurons,hidden_layer_neurons);
	Fill_mat_rand(output_weight);

	Matrix hidden_bias = Init_matrix(hidden_layer_neurons,1);
	Fill_mat_rand(hidden_bias);
	
	Matrix output_bias = Init_matrix(output_layer_neurons,1);
	Fill_mat_rand(output_bias);

	Matrix res_hidden_layer = Init_matrix(hidden_weight.nb_column,input.nb_rows);
		

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
		Mult_mat_1(input,hidden_weight,res_hidden_layer);
		Sum_bias(res_hidden_layer,hidden_bias,res_hidden_layer);
		Sigmo_mat(res_hidden_layer,hidden_layer_output);
		
		Mult_mat_1(hidden_layer_output,output_weight,res_output_layer);
		Sum_bias(res_output_layer,output_bias,res_output_layer);
		Sigmo_mat(res_output_layer,final_res);
		
	//BACKPROPAGATION
		Multip_factor(final_res,(-1),error_multip_factor);
		Sum_weights(error_multip_factor,expected_output,error);	
		Sigmo_mat_derivate(final_res,back_final_res_sigmo);
		Mult_simple(error,back_final_res_sigmo,back_final_res);
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
		Sum_bias(hidden_bias,hidden_bias_back_multfact,hidden_bias);
	
	}
		Pretty_print(final_res);
}



struct Neural_Network
{
	//TODO
};










