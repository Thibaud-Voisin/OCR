#include "neural_network.h"

int main()
{
	srand(time(NULL));	


//	printf("Enter the number of training\n");
	unsigned long nb_rep = 100000;
//	scanf("%lu",&nb_rep);

//	printf("Enter the precision:\n");
	double precision = 4;
//	scanf("%lf",&precision);	

	Matrix input = Init_matrix(2,4);
	double data_input[8] = {0,0,0,1,1,0,1,1};
	Fill_mat_data(input,data_input,8);
	

	Matrix expected_output = Init_matrix(1,4);
	double data_expected_output[4] = {0,1,1,0};
	Fill_mat_data(expected_output,data_expected_output,4);

	int input_layer_neurons = 2;
	int hidden_layer_neurons = 22;
	int output_layer_neurons = 1; 


	Neural_network Xor = Init_neural_network(input,input_layer_neurons,hidden_layer_neurons,output_layer_neurons, 0);
	Train_N_n(Xor,input,expected_output,nb_rep,precision);
	Free_Neural_network(Xor);
	free(input.matrix_data);
	free(expected_output.matrix_data);
	return 1;
}
