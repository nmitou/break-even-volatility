/*
compile with g++ -I C:\local\eigen-3.4.0 -o bev_class_testing utils.o bev.o bev_class_testing.cpp 
*/
#include <iostream>
#include "bev.h"
#include <vector>
#include <Eigen/Dense>

int main() {
	double r = 0.065;
	std::vector<double> strikes = {80.0, 85.0, 90.0, 95.0, 100.0, 105.0, 110.0, 115.0, 120.0};
	std::vector<int> maturities = {1, 2, 3, 4, 5, 6, 9, 12, 15, 18};
	bev::BEV bev_obj("sampledata.csv", r, strikes, maturities);

	Eigen::ArrayXXd bev_output = bev_obj.SolveForBEV();

	std::cout << "\nBEV array =\n" << std::endl << bev_output;

	return 0;
}