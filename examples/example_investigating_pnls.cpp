/* 
	Program highlighting some peculiarities when calculating break-even volatility, namely when aggregating
	break-even volatility estimates by averaging, rather than finding the break-even volatility which 
	zeroes the average PnL.
	
	Compilation: Must include paths to Eigen library, bev and utils. Must link bev.cpp and utils.cpp as well (or their respective object files).
	Compile with (for example, if using GCC):
		g++ -I path/to/eigen -I ../bev -I ../bev/utils -o example_pnls example_investigating_pnls.cpp ../bev/bev.cpp ../bev/utils/utils.cpp
	or with CMake (when building entire repo). */

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "bev.h"
#include "utils.h"

using bev_utils::PrintResults;

int main() {
	// Choose an interest rate, some strikes and maturities
	double r = 0.065;
	std::vector<double> strikes = {0.80, 0.85, 0.90, 0.95, 1.00, 1.05, 1.10, 1.15, 1.20};
	std::vector<int> maturities = {1, 2, 3, 4, 5, 6, 9, 12, 15, 18};
	// Using simulated data (GBM, mu = 0.07, sigma = 0.2, 5 years worth * 252 days)
	bev::BEV bev_obj("sampledata.csv", r, strikes, maturities, 0);

	// BEV surface when averaging the PnLs
	Eigen::ArrayXXd bev_surface = bev_obj.SolveForBEV();
	std::cout << "\nBEV surface with average PnL zeroed:" << std::endl << std::endl;
	PrintResults(strikes, maturities, bev_surface);

	// BEV surface when averaging sub-path BEVs
	bev_surface = bev_obj.SolveForBEV(false);
	std::cout << "\nBEV surface with BEVs averaged:" << std::endl << std::endl;
	PrintResults(strikes, maturities, bev_surface);

	// Let's have a look at a specific break-even volatility
	double strike = 0.80;
	int maturity = 1;
	// Let's see the individual 1-month break-even volatilities
	Eigen::ArrayXXd path_bevs = bev_obj.SolveForBEV(strike, maturity);
	std::cout << "\nSub-path BEVs (strike = " << strike << ", maturity = " << maturity << "):" << std::endl;
	std::cout << path_bevs.transpose() << std::endl;
	// Let's investigate the 11th path
	std::cout << "\nBEV for the 11th path: " << path_bevs(10, 0) << std::endl;
	Eigen::ArrayXXd paths = bev_obj.GetSubPaths(maturity);
	std::cout << "11th path (data rebased) :\n" << paths.row(10) << std::endl;
	// Now let's look at the PnL for a range of volatilities i.e. as we search for the vol which zeros the PnL
	std::cout << "\nFor the path above, calculate the PnL as a function of sigma:" << std::endl;
	// First we need a times-to-maturity vector/array for pnl calculation
	int days_to_maturity = maturity*21;
	Eigen::Array<double, 1, Eigen::Dynamic> times_to_maturity = Eigen::ArrayXd::LinSpaced(days_to_maturity, (double) days_to_maturity-1, 0.0) * (1.0 / 252.0);
	// linspace variables for sigma range
	int n = 20; // number of steps in sigma range
	double a = 0.01, b = 0.21; // sigma range start/end
	std::cout << "PnL(sigma) = ..." << std::endl;
	for (int i=0; i<n; i++) {
		double sigma = a + i*((b-a)/n);
		std::cout << "PnL(" << sigma << ") = " << bev_obj.ContinuousDHPnL(sigma, paths.row(10), strike, times_to_maturity) << std::endl;
	}
	std::cout << "\nAs you can see, the choice of tolerance in finding the root of the PnL function will greatly affect the BEV solution." << std::endl << 
					"In first attempts, when finding the root based on x-axis tolerance, there were issues with this as you can see the PnL is positive for all sigma." << std::endl <<
					"Thus, adding an f-tolerance parameter was necessary which stops the root finding algorithm at sufficiently small PnL value." << std::endl <<
					"While the f-tolerance parameter can affect the final BEV for a specific path (default set at 1e-12 at the time of writing), we find " <<
					"that this is only the case when finding single BEV estimates for a specific path, i.e. not when finding the BEV that zeroes the average PnL." << std::endl;

	return 0;
}
