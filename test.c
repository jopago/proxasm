#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <c_funcs.h>


extern double 	asm_norm2(const double*,const int);
extern double 	asm_norm1(const double*,const int);
extern double 	asm_sum(const double*,const int);
extern void 	asm_normalize(double*,const int); 

extern double 	asm_proxl2square(double*,int,double);
extern double 	asm_proxl1(double*,int,double);


typedef double (*func_vec)(double*,const int);
typedef double (*func_prox)(double*,int,double);


double elapsed(clock_t begin, clock_t end)
{
	return (double)(end - begin) / CLOCKS_PER_SEC;
}

double *rand_array(int N)
{
	double *t = malloc(N*sizeof(double));
	int i;

	for(i=0;i<N;i++)
	{
		t[i] = rand() / (double)RAND_MAX;
	}

	return t;
}

void disp(double* t, int N)
{
	int i;
	for(i=0;i<N;i++) 
	{
		printf("%f ", t[i]);
	}
	printf("\n");
}

void benchmark_vec(func_vec f_c, func_vec f_asm, char name[64], int N)
{
	clock_t begin, end; 
	double *t = rand_array(N);
	double ans;

	// C  

	begin = clock();
	ans = f_c(t,N);
	end = clock();

	printf("\n----\t%s benchmark \t----\n", name);
	printf("----\tC: %fs \t (output: %f)\n", elapsed(begin,end),ans);

	// ASM
	
	begin = clock();
	ans = f_asm(t,N);
	end = clock(); 

	printf("----\tASM: %fs \t (output: %f)\n\n", elapsed(begin, end), ans);
}



int main()
{
	clock_t begin,end;
	int N;
	int i;
	double *t;
	double sum;
	
	N = 6000011;
	srand(time(0));

	benchmark_vec(c_sum, asm_sum, "SUM", N);
	benchmark_vec(c_norm2, asm_norm2, "NORM2",N);
	benchmark_vec(c_normalize, asm_normalize,"NORMALIZE",N);
	benchmark_vec(c_norm1, asm_norm1, "NORM1",N);

	return 0;
}
