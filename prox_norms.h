#include <sse.h>
#include <stdlib.h>

double* _call_sse_prox_l2_square(const double *v, int n, double lambda)
{
	double* result = malloc(n*sizeof(double));
	memcpy(result,v,n*sizeof(double));

	sse_proxl2square(result, n, lambda);

	return result;
}

double* c_prox_l2_square(const double *v, int n, double lambda)
{
	double *result = malloc(n*sizeof(double));
	int i;
	
	for(i=0;i<n;i++)
		result[i] = v[i]/(1+lambda);
	
	return result;
}
	
