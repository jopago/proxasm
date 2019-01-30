#ifndef __C_FUNCS
#define __C_FUNCS 

#include <math.h>

#define max(a,b) (a < b) ? b : a

double c_sum(double *t, int N)
{
	int i;
	double sum=0.0f;
	for(i=0;i<N;i++)
	{
		sum += t[i];
	}

	return sum;
}

double c_norm2(double *t, int N)
{
	int i;
	double norm2=0.0f;
	for(i=0;i<N;i++)
	{
		norm2 += t[i]*t[i];
	}

	return sqrt(norm2);
}

double c_normalize(double *t, int N)
{
	int i;
	double norm2 = c_norm2(t,N);

	for(i=0;i<N;i++) 
	{
		t[i] /= norm2;
	}

	return norm2;
}

double c_norm1(double *t, int N)
{
	int i;
	double norm1=0.f;

	for(i=0;i<N;i++)
	{
		norm1 += max(t[i],-t[i]);
	}

	return norm1;
}

#endif 
