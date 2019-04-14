#ifndef __AVX_C__
#define __AVX_C__

#include <x86intrin.h>

static __m256d avx_sign_pd(__m256d x)
{
    __m256d zero = _mm256_setzero_pd();

    __m256d positive = _mm256_and_pd(_mm256_cmp_pd(x, zero, 14), _mm256_set1_pd(1.0));
    __m256d negative = _mm256_and_pd(_mm256_cmp_pd(x, zero, 1), _mm256_set1_pd(-1.0));

    return _mm256_or_pd(positive, negative);
}

static __m256d avx_abs_pd(__m256d x)
{
	__m256d zero = _mm256_setzero_pd(); 
	__m256d y; 

	y = _mm256_sub_pd(zero, x);  // y = -x 
	return _mm256_max_pd(x, y); 
}

void avx_proxl1(double *t, int N, double lambda)
{
	int i;
	__m256d zero = _mm256_set1_pd(0.0); 
	__m256d l = _mm256_set1_pd(lambda); 

	for(i=0;i<N;i+=4)
	{
		__m256d x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);

		__m256d sign = avx_sign_pd(x); 

		x = avx_abs_pd(x); 

		x = _mm256_sub_pd(x, l); // abs(x) - lambda 
		x = _mm256_max_pd(zero, x); 

		x = _mm256_mul_pd(x, sign); 

		_mm256_store_pd(t+i, x);  
	}
}

void avx_proxl2(double *t, int N, double lambda)
{
	__m256d multiplier = _mm256_set1_pd(1.0/(1.0+lambda)); 

	int i;
	for(i=0;i<N;i+=4)
	{
		__m256d x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]); 
		x = _mm256_mul_pd(x, multiplier);
		_mm256_store_pd(t+i, x);
	}
}

#endif 