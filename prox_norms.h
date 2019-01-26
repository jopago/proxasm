#include "prox_norms.h"

extern double asm_proxl2_square(double*,int,double); 

double* prox_l2square(double *v, int32_t n, double lambda)
{
	double* result = malloc(n*sizeof(double));
	memcpy(res,v,n*sizeof(double));

	asm_proxl2_square(result, n, lambda);

	return result;
}

