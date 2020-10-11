#include "neural_network.h"

int main()
{
	
	srand(time(NULL));	
	Matrix matrix = Init_matrix(2,1);
	Fill_mat_rand(matrix);
	Pretty_print(matrix);
	
	Matrix matrix2 = Init_matrix(3,2);
	Fill_mat_rand(matrix2);
	Pretty_print(matrix2);

	Pretty_print(Mult_mat_1(matrix,matrix2));
	double rd = (double) rand() / RAND_MAX;
	printf("%.5f\n",rd);
	double rd2 = (double) rand() / RAND_MAX;
	printf("%.5f\n",rd2);
	double rd3 = (double) rand() / RAND_MAX;
	printf("%.5f\n",rd3);
	double rd4 = (double) rand() / RAND_MAX;
	printf("%.5f\n",rd4);
	return 0;
}



struct Neural_Network
{
	//TODO
};
