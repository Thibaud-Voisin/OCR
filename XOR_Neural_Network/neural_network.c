#include "neural_network.h"

int main()
{
	
	srand(time(NULL));	
	Matrix matrix = Init_matrix(5,4);
	Fill_mat_rand(matrix);
	Pretty_print(matrix);
	Matrix res = Sum_column(matrix);
	Pretty_print(res);
	return 0;
}



struct Neural_Network
{
	//TODO
};
