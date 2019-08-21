#ifndef __AVX_C__
#define __AVX_C__

#include <x86intrin.h>

#define AVX_CMP_GT  14
#define AVX_CMP_LT  1

inline __m256d avx_sign_pd(const __m256d x)
{
    const __m256d zero = _mm256_setzero_pd();

    __m256d positive = _mm256_and_pd(_mm256_cmp_pd(x, zero, AVX_CMP_GT), _mm256_set1_pd(1.0));
    __m256d negative = _mm256_and_pd(_mm256_cmp_pd(x, zero, AVX_CMP_LT), _mm256_set1_pd(-1.0));

    return _mm256_or_pd(positive, negative);
}

inline __m256d avx_sign_ps(const __m256d x)
{
    const __m256d zero = _mm256_setzero_ps();

    __m256d positive = _mm256_and_ps(_mm256_cmp_ps(x, zero, AVX_CMP_GT), _mm256_set1_ps(1.0f));
    __m256d negative = _mm256_and_ps(_mm256_cmp_ps(x, zero, AVX_CMP_LT), _mm256_set1_ps(-1.0f));

    return _mm256_or_ps(positive, negative);
}

inline __m256d avx_abs_pd(const __m256d x)
{
	const __m256d zero = _mm256_setzero_pd();

	return _mm256_max_pd(x, _mm256_sub_pd(zero, x));
}

inline __m256d avx_abs_ps(const __m256d x)
{
	const __m256d zero = _mm256_setzero_ps();

	return _mm256_max_ps(x, _mm256_sub_ps(zero, x));
}

inline double avx_hsum_pd(const __m256d x) {
    // https://stackoverflow.com/questions/49941645/get-sum-of-values-stored-in-m256d-with-sse-avx
    const __m128d low  = _mm256_castpd256_pd128(x);
    const __m128d high = _mm256_extractf128_pd(x, 1);
            low  = _mm_add_pd(low, high);

    const __m128d last = _mm_unpackhi_pd(low, low);
    return  _mm_cvtsd_f64(_mm_add_sd(low, last));
}

inline __m256d avx_box(const __m256d x, const __m256d min, const __m256d max)
{
    return _mm256_max_pd(_mm256_min_pd(x, max), min);
}

void avx_projLinf(double *t, const int N, const double min, const double max)
{
    int i;
    const __m256d avx_min, avx_max;

    avx_min = _mm256_set1_pd(min);
    avx_max = _mm256_set1_pd(max);

    for(i=0; i<N; i+=4)
    {
        __m256d x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);

        _mm256_store_pd(t+i, avx_box(x, avx_min, avx_max));
    }
}

void avx_projBox(double *t, const int N, const double r)
{
    avx_projLinf(t, N, -r, r);
}

void avx_proxl1(double *t, const int N, const double lambda)
{
	int i;
	const __m256d zero = _mm256_set1_pd(0.0);
	const __m256d l = _mm256_set1_pd(lambda);
	__m256d x, sign; 

	for(i=0; i<N; i+=4)
	{
		x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);

		sign = avx_sign_pd(x);

		x = avx_abs_pd(x);

		x = _mm256_sub_pd(x, l); // abs(x) - lambda
		x = _mm256_max_pd(zero, x);

		x = _mm256_mul_pd(x, sign);

		_mm256_store_pd(t+i, x);
	}
}

void avx_proxl2square(double *t, const int N, const double lambda)
{
	const __m256d multiplier = _mm256_set1_pd(1.0/(1.0+lambda));
	__m256d x;
	int i;
	for(i=0; i<N; i+=4)
	{
		x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);
		x = _mm256_mul_pd(x, multiplier);
		_mm256_store_pd(t+i, x);
	}
}

double avx_sum(double *t, const int N)
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

double avx_norm2(double *t, const int N)
{
	__m256d sum = _mm256_set1_pd(0.0);
	__m256d x;
	int i;

	for(i=0;i<N;i+=4)
	{
		x = _mm256_set_pd(t[i+3], t[i+2], t[i+1], t[i]);
		x = _mm256_mul_pd(x,x);
		sum = _mm256_add_pd(sum, x);
	}

	return sqrt(avx_hsum_pd(sum));
}

#endif
