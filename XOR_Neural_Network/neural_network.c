#include "neural_network.h"

Neural_network Init_neural_network(Matrix input, int input_layer_neurons, int hidden_layer_neurons, int output_layer_neurons)
{
	Neural_network N_n;

	N_n.hidden_weight = Init_matrix(hidden_layer_neurons,input_layer_neurons);
	Fill_mat_rand(N_n.hidden_weight);
//	Pretty_print(N_n.hidden_weight);
	N_n.output_weight = Init_matrix(output_layer_neurons,hidden_layer_neurons);
	Fill_mat_rand(N_n.output_weight);

	N_n.hidden_bias = Init_matrix(hidden_layer_neurons,1);
	Fill_mat_rand(N_n.hidden_bias);
	
	N_n.output_bias = Init_matrix(output_layer_neurons,1);
	Fill_mat_rand(N_n.output_bias);

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

void Train_N_n(Neural_network N_n,Matrix input, Matrix expected_output, unsigned long nb_rep,double precision)
{
	for(;nb_rep > 0;--nb_rep)
	{
		Mult_mat_1(input, N_n.hidden_weight,N_n.res_hidden_layer);
		Sum_bias(N_n.res_hidden_layer,N_n.hidden_bias,N_n.res_hidden_layer);
		Sigmo_mat(N_n.res_hidden_layer,N_n.hidden_layer_output);
		
		Mult_mat_1(N_n.hidden_layer_output,N_n.output_weight,N_n.res_output_layer);
		Sum_bias(N_n.res_output_layer,N_n.output_bias,N_n.res_output_layer);
		Sigmo_mat(N_n.res_output_layer,N_n.final_res);
		
	//BACKPROPAGATION
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
		
		

		Sum_weights(N_n.output_weight,N_n.output_weight_back_multfac,N_n.output_weight);
		Sum_column(N_n.back_final_res,N_n.output_bias_back_sumcol);
		Multip_factor(N_n.output_bias_back_sumcol,precision,N_n.output_bias_back_multifac);
		Sum_bias(N_n.output_bias,N_n.output_bias_back_multifac,N_n.output_bias);
		
		Transp_mat(input,N_n.hidden_weight_back_transp);
		Mult_mat_1(N_n.hidden_weight_back_transp,N_n.back_hidden_layer,N_n.hidden_weight_back_multmath);
		Multip_factor(N_n.hidden_weight_back_multmath,precision,N_n.hidden_weight_back_multfac);
		Sum_weights(N_n.hidden_weight,N_n.hidden_weight_back_multfac,N_n.hidden_weight);
			
		Sum_column(N_n.back_hidden_layer,N_n.hidden_bias_back_sumcol);
		Multip_factor(N_n.hidden_bias_back_sumcol,precision,N_n.hidden_bias_back_multfact);
		Sum_bias(N_n.hidden_bias,N_n.hidden_bias_back_multfact,N_n.hidden_bias);
	
	}
		Pretty_print(N_n.final_res);
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
