/*
	The first example uses simulated data from a Geometric Brownian Motion process (GBM), i.e. a price process in a Black-Scholes world.
	Naturally, with "enough" data (hence, we'll provide a large amount of simulated data), we expect the break-even volatility computation
	to result in a flat volatility surface (due to the constant volatility assumption).

	Compile with:
		g++ -I path/to/eigen -I path/to/bev -I path/to/utils -o example1 example1_GBMdata.cpp bev.cpp utils.cpp
	or with cmake.
*/

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include "bev.h"
#include "utils.h"

using bev_utils::PrintResults;
using data_utils::GenerateGBMData;

int main() {
	// Let's generate some GBM data
	int T = 5; // number of years => *252 points per year assuming 21 business days per month
	Eigen::ArrayXXd S;
	// Simulate data with function, can reset seed in declaration above
	double sigma = 0.2;
	GenerateGBMData(S, 100, 0.07, sigma, T, 12345);

	std::cout << "\nBreak-even volatility computations using simulated GBM data with volatility = " << sigma << std::endl << std::endl;

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
	std::cout << "BEV estimates based on " << T << " years of data:" << std::endl << std::endl;
	PrintResults(strikes, maturities, volSurface);
	std::cout << std::endl;

	// Let's generate a much longer sample path and see the results
	T = 100; // as an even greater sanity check, make this 1000 - surface flattens out
	GenerateGBMData(S, 100, 0.07, 0.2, T, 12345);
	bevSimulated.SetData(S);
	volSurface = bevSimulated.SolveForBEV();
	std::cout << "BEV estimates based on " << T << " years of data:" << std::endl << std::endl;
	PrintResults(strikes, maturities, volSurface);
	std::cout << std::endl;

	return 0;
}