#ifndef __AVX_C__
#define __AVX_C__

#include <x86intrin.h>

#define AVX_CMP_GT  14
#define AVX_CMP_LT  1

inline __m256d avx_sign_pd(__m256d x)
{
    __m256d zero = _mm256_setzero_pd();

    __m256d positive = _mm256_and_pd(_mm256_cmp_pd(x, zero, AVX_CMP_GT), _mm256_set1_pd(1.0));
    __m256d negative = _mm256_and_pd(_mm256_cmp_pd(x, zero, AVX_CMP_LT), _mm256_set1_pd(-1.0));

    return _mm256_or_pd(positive, negative);
}

inline __m256d avx_abs_pd(__m256d x)
{
	__m256d zero = _mm256_setzero_pd();
	__m256d y;

	y = _mm256_sub_pd(zero, x);  // y = -x
	return _mm256_max_pd(x, y);
}

inline double avx_hsum_pd(__m256d x) {
    // https://stackoverflow.com/questions/49941645/get-sum-of-values-stored-in-m256d-with-sse-avx
    __m128d low  = _mm256_castpd256_pd128(x);
    __m128d high = _mm256_extractf128_pd(x, 1);
            low  = _mm_add_pd(low, high);

    __m128d last = _mm_unpackhi_pd(low, low);
    return  _mm_cvtsd_f64(_mm_add_sd(low, last));
}

inline __m256d avx_box(__m256d x, __m256d min, __m256d max)
{
    return _mm256_max_pd(_mm256_min_pd(x, max), min);
}

void avx_projLinf(double *t, int N, double min, double max)
{
    int i;
    __m256d avx_min, avx_max;

    avx_min = _mm256_set1_pd(min);
    avx_max = _mm256_set1_pd(max);

    for(i=0;i<N;i+=4)
    {
        __m256d x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);

        _mm256_store_pd(t+i, avx_box(x, avx_min, avx_max));
    }
}

void avx_projBox(double *t, int N, double r)
{
    avx_projLinf(t, N, -r, r);
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

void avx_proxl2square(double *t, int N, double lambda)
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

double avx_sum(double *t, int N)
{
	__m256d sum = _mm256_set1_pd(0.0);
	int i;

	for(i=0;i<N;i+=4)
	{
		__m256d x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);
		sum = _mm256_add_pd(sum, x);
	}

	return avx_hsum_pd(sum);
}

double avx_norm2(double *t, int N)
{
	__m256d sum = _mm256_set1_pd(0.0);
	int i;

	for(i=0;i<N;i+=4)
	{
		__m256d x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);
		x = _mm256_mul_pd(x,x);
		sum = _mm256_add_pd(sum, x);
	}

	return sqrt(avx_hsum_pd(sum));
}

#endif
