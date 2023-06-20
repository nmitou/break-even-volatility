/*
	Calculating the break-even volatility surface for Google/Alphabet's share price from 2018-2023.

	Compile with:
		g++ -I path/to/eigen -I path/to/bev -I path/to/utils -o main main.cpp bev.cpp utils.cpp
	or with cmake.
*/
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "bev.h"
#include "utils.h"

using bev_utils::PrintResults;

int main() {
	// Choose interest rate to apply over period, let's say an average interest of 1.5% applied over the period in the US
	double r = 0.015;
	// Strikes in relative (%) terms as stock price will be rebased in multiple sub-periods, in order to aggregate BEV results
	std::vector<double> strikes = {0.80, 0.85, 0.90, 0.95, 1.00, 1.05, 1.10, 1.15, 1.20};
	// Terms/maturities in months
	std::vector<int> maturities = {1, 2, 3, 4, 5, 6, 9, 12, 15, 18};
	// Create BEV object with data from CSV, specifying we want the Close prices
	bev::BEV GOOG_bev("GOOG.csv", r, strikes, maturities, -1, true, "Close");
	// Compute the volatility surface based on the data entered, this will solve for the break-even volatility which
	// zeroes the average profit/loss (average of sub-periods PnL's) for each maturity-strike combination
	Eigen::ArrayXXd GOOG_surface = GOOG_bev.SolveForBEV();
	std::cout << "\nBEV surface with average PnL zeroed:" << std::endl << std::endl;
	PrintResults(strikes, maturities, GOOG_surface);

	// We can repeat the procedure, choosing the average the break-even volatilities found from each period instead
	GOOG_surface = GOOG_bev.SolveForBEV(false);
	std::cout << "\nBEV surface with BEVs averaged:" << std::endl << std::endl;
	PrintResults(strikes, maturities, GOOG_surface);

	std::cout << "\nNotice the vast differences in shorter-dated skews between the two aggregation methods." << std::endl;

	// If we only want one strike maturity combination
	double strike = 1.05;
	int maturity = 3;
	GOOG_bev.SetStrikes(std::vector<double> {strike});
	GOOG_bev.SetMaturities(std::vector<int> {maturity});
	Eigen::ArrayXXd res = GOOG_bev.SolveForBEV(false); // once again, one can choose whether to average the PnL's or not
	std::cout << "\nBEV result for strike = " << strike << " and maturity = " << 3 << " months: " << res << std::endl << std::endl;

	// If we want all the break-even volatilities from each of the sub-periods included
	Eigen::ArrayXXd path_bevs = GOOG_bev.SolveForBEV(strike, maturity);
	std::cout << "Here are the break-even volatility results for strike = " << strike << " and for each " << maturity << " month period in the data:" << std::endl << std::endl;
	std::cout << path_bevs << std::endl << std::endl;
	std::cout << "Which average to " << path_bevs.mean() << " as above." << std::endl;

	return 0;
}