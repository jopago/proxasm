#ifndef __SSE_C__
#define __SSE_C__

#include <x86intrin.h>

inline __m128d sign_pd(__m128d x)
{
    __m128d zero = _mm_setzero_pd();

    __m128d positive = _mm_and_pd(_mm_cmpgt_pd(x, zero), _mm_set1_pd(1.0));
    __m128d negative = _mm_and_pd(_mm_cmplt_pd(x, zero), _mm_set1_pd(-1.0));

    return _mm_or_pd(positive, negative);
}

inline __m128d abs_pd(__m128d x)
{
	__m128d zero = _mm_setzero_pd(); 
	__m128d y; 

	y = _mm_sub_pd(zero, x);  // y = -x 
	y = _mm_max_pd(x, y); 

	return y; 
}

void sse_intrin_proxl1(double *t, int N, double lambda)
{
	int i;
	__m128d zero = _mm_set1_pd(0.0); 
	__m128d l = _mm_set1_pd(lambda); 

	for(i=0;i<N;i+=2)
	{
		__m128d x = _mm_set_pd(t[i+1], t[i]);

		__m128d sign = sign_pd(x); 

		x = abs_pd(x); 

		x = _mm_sub_pd(x, l); // abs(x) - lambda 
		x = _mm_max_pd(zero, x); 

		x = _mm_mul_pd(x, sign); 

		_mm_store_pd(t+i, x);  
	}
}


#endif 