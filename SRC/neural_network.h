#ifndef neural_network
#define neural_network

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "tools.h"

typedef struct Neural_Network_t
{
	Matrix hidden_weight;
	Matrix output_weight; 
	Matrix hidden_bias ;
	Matrix output_bias; 
	Matrix res_hidden_layer;
	Matrix hidden_layer_output; 
	Matrix res_output_layer;
	Matrix final_res;
	Matrix error_multip_factor;
	Matrix error;
	Matrix back_final_res_sigmo;
	Matrix back_final_res;
	Matrix error_hidden_layer_transp;
	Matrix error_hidden_layer;
	Matrix back_hidden_layer_sigmod;
	Matrix back_hidden_layer;
	Matrix output_weight_back_transp ;
	Matrix output_weight_back_multmath;
	Matrix output_weight_back_multfac;
	Matrix output_bias_back_sumcol;
	Matrix output_bias_back_multifac;
	Matrix hidden_weight_back_transp;
	Matrix hidden_weight_back_multmath;
	Matrix hidden_weight_back_multfac;
	Matrix hidden_bias_back_sumcol;
	Matrix hidden_bias_back_multfact;
}Neural_network;

Neural_network Train_N_n(Neural_network N_n,Matrix input, Matrix expected_output, unsigned long nb_rep,double precision, int a);

Neural_network Init_neural_network(Matrix input, int input_layer_neurons, int hidden_layer_neurons, int output_layer_neurons, int load);

void Free_Neural_network(Neural_network a);

char Evaluate_char(Neural_network N_n,Matrix input);
#endif


