#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


extern double asm_norm2(const double*,const int);
extern double asm_sum(const double*,const int);


extern double asm_proxl2square(double*,int,double);
extern double asm_proxl1(double*,int,double);

double csum(double*t,int N)
{
	double sum=0.0f;
	int i;

	for(i=0;i<N;i++) {
		sum+=t[i];
	}

	return sum;
}

int main()
{
	srand(time(0));
	int N = 6000011;
	int i;
	double *t = malloc(N*sizeof(double));


	/* random array */

	for(i=0;i<N;i++) {
		t[i] = rand() / (double)RAND_MAX;
		// printf("%f\n",t[i]);
	}


	clock_t begin = clock();
	double sum = 0.0f;

  	for(i=0;i<N;i++) {
		sum += t[i]*t[i];
	}

	sum=sqrt(sum);

  	clock_t end = clock();

 	double elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;

 	printf("N = %d\n", N);
	printf("C: L2-norm : %f  -- Elapsed: %fs\n", sum, elapsed_secs);


	begin = clock();
	
	sum = asm_norm2(t,N);

  	end = clock();
 	elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;

 	printf("ASM: L2-norm : %f -- Elapsed: %fs\n", sum, elapsed_secs);

	return 0;
}
