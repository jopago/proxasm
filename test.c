#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <c_funcs.h>

#include <SSE/sse_c.h>
#include <AVX/avx_c.h>
#include <sse.h>

#define ALIGN_SSE	16
#define ALIGN_AVX	32

typedef double (*func_vec)(double*,const int);
typedef void (*func_prox)(double*,int,double);

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
		t[i] -= 0.5;
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
	double ans_c, ans_asm;

	// C

	begin = clock();
	ans_c = f_c(t,N);
	end = clock();

	printf("\n----\t%s benchmark \t----\n", name);
	printf("----\tC: %fs \t (output: %f)\n", elapsed(begin,end),ans_c);

	// ASM

	begin = clock();
	ans_asm = f_asm(t,N);
	end = clock();
	printf("----\tASM: %fs \t (output: %f)\n\n", elapsed(begin, end), ans_asm);

	// assert(fabs(ans_c - ans_asm) < 1e-6);
}

void benchmark_prox(func_prox f_c, func_prox f_asm, char name[64], int N,
	int align)
{
	clock_t begin, end;
	double *t = rand_array(N);
	double *ans_c, *ans_asm; 

	ans_c 	= (double*) malloc(N*sizeof(double));
	ans_asm = (double*) _mm_malloc(N*sizeof(double),align);

	if(!ans_c || !ans_asm) exit(EXIT_FAILURE);

	memcpy(ans_c, t, N*sizeof(double));
	memcpy(ans_asm, t, N*sizeof(double)); 

	// C

	begin = clock();
	f_c(ans_c,N, 0.2);
	end = clock();

	printf("\n----\t%s benchmark \t----\n", name);
	printf("----\tC: %fs \n", elapsed(begin,end));

	// ASM

	begin = clock();
	f_asm(ans_asm,N,0.2);
	end = clock();

	assert(array_equality(ans_c, ans_asm, N));

	printf("----\t%s: %fs \n", align==ALIGN_SSE?"SSE":"AVX",elapsed(begin, end));
	free(ans_c);
	_mm_free(ans_asm);
}


int main()
{
	int N;
	N = 10000000;
	//srand(time(0));

	benchmark_vec(c_sum, sse_sum, "SSE SUM", N);
	benchmark_vec(c_norm2, sse_norm2, "SSE NORM2",N);
	benchmark_vec(c_normalize, sse_normalize,"SSE NORMALIZE",N);
	benchmark_vec(c_norm1, sse_norm1, "SSE NORM1",N);
	benchmark_prox(c_proxl1, sse_intrin_proxl1, "SSE PROXL1",N,ALIGN_SSE);

	benchmark_vec(c_sum, avx_sum, "AVX SUM", N);
	benchmark_vec(c_norm2, avx_norm2, "AVX NORM2", N);
	benchmark_prox(c_proxl1, avx_proxl1, "AVX PROXL1",N,ALIGN_AVX);
	benchmark_prox(c_proxl2, avx_proxl2, "AVX PROXL2", N, ALIGN_AVX); 

	return 0;
}
