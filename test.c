#include <stdio.h>

extern double norm2(const double*,const int);
extern double sum(const double*,const int);


extern double asm_proxl2square(double*,int,double);
extern double asm_proxl1(double*,int,double);


int main()
{
	double t[6] = {1,1, 0, 1, -1, 103};	

	// printf("%f\n", norm2(t,6));
	// printf("%f\n", sum(t,6));

	

	// printf("%f\n", asm_proxl2square(t,6,2.1));
	// printf("%f\n",t[5]);

	asm_proxl1(t,6,1)	;
	printf("%lf\n", t[2]);

	return 0;
}