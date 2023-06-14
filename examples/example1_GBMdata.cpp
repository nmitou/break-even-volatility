/*
	The first example uses simulated data from a Geometric Brownian Motion process (GBM), i.e. a price process in a Black-Scholes world.
	Naturally, with "enough" data (hence, we'll provide a large amount of simulated data), we expect the break-even volatility computation
	to result in a flat volatility surface (due to the constant volatility assumption).

	Compile with:
		g++ -I path/to/eigen path/to/utils path/to/bev -o example1_GBMdata example1_GBMdata.cpp bev.cpp utils.cpp
	or with cmake.
*/

#include <iostream>
#include <random>
#include <Eigen/Dense>
#include <cmath>
#include <vector>
#include "bev.h"

using std::sqrt;
using std::pow;
using std::exp;

void GenerateGBMData(Eigen::ArrayXXd& S, double S_0, double mu, double sigma, int T_years, int seed = 12345);

int main() {
	// Let's generate some GBM data
	int T = 5; // number of years => *252 points per year assuming 21 business days per month
	Eigen::ArrayXXd S((T * 252) + 1, 1); // initialize array with correct number of points
	// Simulate data with function, can reset seed in declaration above
	GenerateGBMData(S, 100, 0.07, 0.2, T);
	// Create BEV object:
	// Using randomly chosen interest rate which differs to drift of process
	double r = 0.065;
	// Set (relative) strikes and maturities (in months)
	std::vector<double> strikes = {0.85, 0.90, 0.95, 1.00, 1.05, 1.10, 1.15};
	std::vector<int> maturities = {1, 2, 3, 6, 9, 12};
	bev::BEV bevSimulated(S, r, strikes, maturities);
	// Create volatility surface by solving for the break-even volatilities for each strike/maturity combination
	// i.e. each row is a volatility skew
	Eigen::ArrayXXd volSurface = bevSimulated.SolveForBEV();
	// We would expect the surface to be flat and approach the simulated volatility as we increase the data size
	std::cout << "BEV estimates based on " << T << " years of data:\n" << volSurface << std::endl;
	// Let's generate a much longer sample path and see the results
	T = 100;
	S = Eigen::ArrayXXd ((T * 252) + 1, 1);
	GenerateGBMData(S, 100, 0.07, 0.2, T);
	bevSimulated.SetData(S);
	volSurface = bevSimulated.SolveForBEV();
	std::cout << "BEV estimates based on " << T << " years of data:\n" << volSurface << std::endl;

	return 0;
}

/*	Function to generate a GBM sample path with given parameters.
	Fills the column vector/1x(N+1) array. */
void GenerateGBMData(Eigen::ArrayXXd& S, double S_0, double mu, double sigma, int T_years, int seed) {
	// First, we set up the generator to sample from the normal distribution
	std::mt19937 generator{seed};
	std::normal_distribution<double> dnorm(0, 1);

	int N = 21*12*T_years; // number of days, assuming 21 business days per month
	double dt = 1.0 / (21.0 * 12.0); // or (double) T/N;
	S(0,0) = 100;
	for (int i = 1; i < N+1; i++)
		S(i, 0) = S(i-1, 0) * exp((mu - 0.5 * pow(sigma, 2)) * dt + sigma * sqrt(dt) * dnorm(generator));
}