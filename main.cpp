/*
compile with g++ -I C:\local\eigen-3.4.0 -o bev_class_testing utils.o bev.o bev_class_testing.cpp 
*/
#include <iostream>
#include "bev.h"
#include <vector>
#include <Eigen/Dense>

int main() {
	double r = 0.065;
	std::vector<double> strikes = {0.80, 0.85, 0.90, 0.95, 1.00, 1.05, 1.10, 1.15, 1.20};
	std::vector<int> maturities = {1, 2, 3, 4, 5, 6, 9, 12, 15, 18};
	bev::BEV bev_obj("C:/Users/Nicolas/Documents/Courses and code/Projects/break-even-volatility/sampledata.csv", r, strikes, maturities);

	Eigen::ArrayXXd bev_output = bev_obj.SolveForBEV();

	std::cout << "\nBEV array (average pnl zeroed) =\n" << std::endl << bev_output << std::endl;

	bev_output = bev_obj.SolveForBEV(false);	

	std::cout << "\nBEV array (average path BEV) =\n" << std::endl << bev_output << std::endl;

	// sometimes gives nans, so lets investigate
	std::cout << "\nLets investigate the nan's:\n" << std::endl;

	std::cout << "Break-even volatility for strike of 100% and 1 month term:" << std::endl;
	std::cout << bev_output(0, 4) << " <- this is fine" << std::endl << std::endl;

	double strike = 0.80;
	int maturity = 1;

	bev_obj.SetStrikes(std::vector<double> {strike});
	bev_obj.SetMaturities(std::vector<int> {maturity});

	Eigen::ArrayXXd bev_80K_1M = bev_obj.SolveForBEV(false);

	std::cout << "Break-even volatility for strike of " << strike << " and " << maturity << " month term:" << std::endl;
	std::cout << "bev_80K_1M = " << bev_80K_1M << std::endl << std::endl;

	Eigen::ArrayXXd path_bevs = bev_obj.SolveForBEV(strike, maturity);

	std::cout << "BEV (strike=" << strike << ", term=" << maturity << " month) for each subpath:" << std::endl;
	std::cout << "path_bevs =\n" << path_bevs.transpose() << std::endl;	
	// nan results for some paths
	// specifically, the 11th result
	Eigen::ArrayXXd paths = bev_obj.GetSubPaths(maturity);
	std::cout << "BEV result for 11th path = " << path_bevs(10, 0) << std::endl;
	std::cout << "11th path =\n" << paths(10, Eigen::all) << std::endl << std::endl;

	std::cout << "Let's double check the BEVs for each path for a strike, maturity combination that doesn't produce nan:\n" <<
					"Path BEVs for example above (strike=100%, term=1 month) =\n" << bev_obj.SolveForBEV(1.0, 1).transpose() << std::endl <<
					"... no nan's to see here." << std::endl << std::endl;

	std::cout << "Back to the result for the 11th path:\n" << 
					"Let's check the PnL for a range of volatilities:" << std::endl;
	// need times to maturity vector/array for pnl calculation
	int days_to_maturity = maturity*21;
	Eigen::Array<double, 1, Eigen::Dynamic> times_to_maturity = Eigen::ArrayXd::LinSpaced(days_to_maturity, (double) days_to_maturity-1, 0.0) * (1.0 / 252.0);
	// linspace variables for sigma range
	int n = 20; // number of steps in sigma range
	double a = 0.01, b = 0.21; // sigma range start/end
	std::cout << "PnL(sigma) = ..." << std::endl;
	for (int i=0; i<n; i++) {
		double sigma = a + i*((b-a)/n);
		std::cout << "PnL(" << sigma << ") = " << bev_obj.ContinuousDHPnL(sigma, paths(10, Eigen::all), strike, times_to_maturity) << std::endl;
	}

	// also test k=105, term=9m



	return 0;
}