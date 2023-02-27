/*
	Simulating Geometric Brownian Motion.
*/

#include <iostream>
#include <random>
#include <Eigen/Dense>
#include <cmath>
#include "../utils.h"

using std::sqrt;
using std::pow;
using Eigen::MatrixXd;
using Eigen::seq;
using Eigen::all;

int main() {
	std::random_device rd{};
	std::mt19937 generator{rd()};
	std::normal_distribution <double> distribution(0, 1);

	// Lets make n_stocks sample paths of GBM with N points each
	int n_stocks = 5, N = 8;
	double S_0 = 100, mu = 0.07, sigma = 0.2, dt = 1.0/N;

	MatrixXd m(n_stocks, N);

	for (int i = 0; i < n_stocks; i++)
		for (int j = 0; j < N; j++)
			m(i, j) = distribution(generator);

	// May be better to work with array's going forward, as we might do more
	// scalar + matrix operations and other element-wise operations than
	// matrix multiplication...
	m = ((mu - 0.5*pow(sigma, 2))*dt + sigma*sqrt(dt)*(m.array())).matrix();
	m = S_0*(eigen_utils::cumsum(m, 0).array().exp()).matrix();

	// matrix S of stock paths where each row represents a stock price path with N+1 data points (including S_0)
	MatrixXd S(n_stocks, N+1);
	// Set first column to be initial price
	S(all, 0) = Eigen::MatrixXd::Constant(5,1, S_0);
	// Set remaining points for all paths
	S(all, seq(1, N)) = m;

	std::cout << S;

	return 0;
}