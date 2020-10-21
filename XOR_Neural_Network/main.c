#include "neural_network.h"

int main()
{
	printf("Enter the number of training\n");
	unsigned long nb_rep = 100000;
	scanf("%lu",&nb_rep);

	printf("Enter the precision:\n");
	double precision = 4;
	scanf("%lf",&precision);	

	Neural_network(nb_rep,precision);

	return 1;
}
