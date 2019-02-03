#include <sse.h>
#include <stdlib.h>

double* c_prox_l2_square(const double *v, int N, double lambda)
{
	double *result = malloc(N*sizeof(double));
	int i;
	
	for(i=0;i<N;i++)
		result[i] = v[i]/(1+lambda);
	
	return result;
}

double *c_prox_l2(const double *v, int N, double lambda)
{
	
}