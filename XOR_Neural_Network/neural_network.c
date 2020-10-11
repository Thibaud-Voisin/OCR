#include "neural_network.h"

int main()
{
	
	srand(time(NULL));	
	Matrix matrix = Init_matrix(4,4);
	Fill_mat_rand(matrix);
	Pretty_print(matrix);
	Sigmo_mat(matrix);
	Pretty_print(matrix);

	return 0;
}



struct Neural_Network
{
	//TODO
};
