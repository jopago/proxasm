#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <ctime>
#include "AVX/avx_c.h"

void proxL1(Eigen::VectorXf& x, const float lambda)
{
	for(int i = 0; i < x.rows(); i++)
	{
		float absx = fabs(x[i]);
		if(absx >= lambda)
		{
			x[i] = (x[i] >= 0 ? 1 : -1)*fmax(absx - lambda, 0);
		} else {
			x[i] = 0.0f; 
		}
	}
}

void proxL2square(Eigen::VectorXf& x, const float lambda)
{
	for(int i = 0; i < x.rows(); i++)
	{
		x[i] *= 1.0f/(1.0f + lambda);
	}
}

/* Soft thresholding AVX */

void proxL1_eigen_avx(Eigen::VectorXf& t, const float lambda)
{
	const __m256 zero 	= _mm256_setzero_ps();
	const __m256 l 		= _mm256_set1_ps(lambda);
	__m256 x, sign; 
	float* raw_t = t.data();

	for(int i=0; i< t.rows(); i+=8)
	{
		x = _mm256_load_ps(raw_t+i);

		sign = avx_sign_ps(x);

		x = avx_abs_ps(x);

		x = _mm256_sub_ps(x, l); // abs(x) - lambda
		x = _mm256_max_ps(zero, x);

		x = _mm256_mul_ps(x, sign);

		_mm256_storeu_ps(raw_t+i, x);
	}
}

void proxL2square_eigen_avx(Eigen::VectorXf& t, const float lambda)
{
	const __m256 multiplier = _mm256_set1_ps(1.0f/(1.0f+lambda));
	float* raw_t = t.data();
	
	for(int i=0; i<t.rows(); i+=8)
	{
		_mm256_store_ps(raw_t+i, _mm256_mul_ps(_mm256_load_ps(raw_t+i), multiplier));
	}
}


Eigen::VectorXf solve_lasso(const Eigen::MatrixXf& A, const Eigen::VectorXf& b, const float lambda)
{
	int steps = 0;
	const float step_size = 0.01f; // for testing purpose 
	Eigen::VectorXf x = Eigen::VectorXf::Zero(b.rows());  

	while(steps < 300)
	{
		Eigen::VectorXf grad = A.transpose()*(A*x - b);
		proxL1(grad, lambda); 
		x = x - step_size * grad; 
		steps++; 
	}

	return x; 
}

Eigen::VectorXf solve_lasso_avx(const Eigen::MatrixXf& A, const Eigen::VectorXf& b, const float lambda)
{
	int steps = 0;
	const float step_size = 0.01f; // for testing purpose 
	Eigen::VectorXf x = Eigen::VectorXf::Zero(b.rows());  

	while(steps < 300)
	{
		Eigen::VectorXf grad = A.transpose()*(A*x - b);
		proxL1_eigen_avx(grad, lambda); 
		x = x - step_size * grad; 
		steps++; 
	}

	return x; 
}


double elapsed(clock_t begin, clock_t end)
{
	return (double)(end - begin) / CLOCKS_PER_SEC;
}

int main()
{
	std::srand(88); 
	const int n = 800000; 
	Eigen::VectorXf b = Eigen::VectorXf::Random(n);
	Eigen::VectorXf x(b), x_avx(b);
	// Eigen::MatrixXf A = 1e-1f*Eigen::MatrixXf::Random(n,n);

	clock_t begin, end ;
	begin = clock();
	proxL1(x, 0.1f);
	end = clock();

	std::cout << "NO AVX: " << elapsed(begin, end) << std::endl; 

	begin = clock();
	proxL1_eigen_avx(x_avx, 0.1f);
	end = clock();

	std::cout << "AVX: " << elapsed(begin, end) << std::endl; 

	std::cout << "L2 Norm of diff " << (x-x_avx).norm() << std::endl;
	return 0;
}