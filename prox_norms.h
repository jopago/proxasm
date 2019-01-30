#include "prox_norms.h"

extern double asm_proxl2_square(double*,int,double); // in-place

double* _asm_prox_l2_square(const double *v, int32_t n, double lambda)
{
	double* result = malloc(n*sizeof(double));
	memcpy(res,v,n*sizeof(double));

	asm_proxl2_square(result, n, lambda);

	return result;
}

double* c_prox_l2_square(const *double *v, int32_t n, double lambda)
{
	double *result = malloc(n*sizeof(double));
	int i;
	
	for(i=0;i<n;i++)
		result[i] = t[i]/(1+lambda);
	
	return result;
}
	
