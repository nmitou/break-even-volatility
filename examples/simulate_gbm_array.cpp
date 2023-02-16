/*
	Simulating Geometric Brownian Motion using Eigen arrays.
*/

#include <iostream>
#include <random>
#include <Eigen/Dense>
#include <cmath>
#include <numeric>

using std::sqrt;
using std::pow;
using Eigen::ArrayXXd;
using Eigen::seq;
using Eigen::all;

template <typename Derived> void cumsum(Eigen::ArrayBase<Derived>& a, int dim);

int main() {
	std::random_device rd{};
	std::mt19937 generator{rd()};
	std::normal_distribution <double> distribution(0, 1);

	// Lets make n_stocks sample paths of GBM with N points each
	int n_stocks = 5, N = 8;
	double S_0 = 100, mu = 0.07, sigma = 0.2, dt = 1.0/N;

	ArrayXXd Z(n_stocks, N);

	for (int i = 0; i < n_stocks; i++)
		for (int j = 0; j < N; j++)
			Z(i, j) = distribution(generator);

	Z = (mu - 0.5*pow(sigma, 2))*dt + sigma*sqrt(dt)*Z;
	// Cumulatively sum over Z's rows 
	cumsum(Z, 0);

	// Array S of stock paths where each row represents a stock price path with N+1 data points (including S_0)
	ArrayXXd S(n_stocks, N+1);
	// Set first column to be initial price
	S(all, 0) = Eigen::MatrixXd::Constant(5,1, S_0);
	// Set remaining points for all paths
	S(all, seq(1, N)) = S_0*(Z.exp());

	std::cout << S;

	return 0;
}

/**
 * Cumulatively sums an array over its rows or columns (specified).
 * 
 * @tparam Derived derived class from ArrayBase
 * @param[out] a array over which the cumulative sum will be performed
 * @param[in] dim dimension over which to sum, 0 for rows or 1 for columns
 */
template <typename Derived>
void cumsum(Eigen::ArrayBase<Derived>& a, int dim) {
	assert(dim==0 || dim==1);
	if (dim) {
		for (auto col : a.colwise())
			std::partial_sum(col.begin(), col.end(), col.begin());
	} else {
		for (auto row : a.rowwise())
			std::partial_sum(row.begin(), row.end(), row.begin());
	}
}