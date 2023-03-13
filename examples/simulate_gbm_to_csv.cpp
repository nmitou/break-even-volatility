/*
	Simulate GBM and output to stream.
*/

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <cmath>

using std::exp;
using std::sqrt;
using std::pow;

// Usage:
// simulate_gbm_to_csv.exe csvname.csv 12345
int main(int argc, char *argv[]) {
	std::string csv_path = argv[1];
	// std::cout << "File name: " << csv_path << std::endl;
	int seed = std::atoi(argv[2]);

	std::ofstream csv_stream(csv_path);

	std::mt19937 generator{seed};
	std::normal_distribution <double> dnorm(0, 1);

	// Lets make a sample path of GBM with N points
	int T = 5; // years
	int N = 21*12*T; // assume 21 business days per month (BEV for option maturities of 1m, 2m, 3m ...)
	double S_0 = 100, mu = 0.07, sigma = 0.2, dt = (double) T/N;

	double gbm_path = S_0;

	csv_stream << gbm_path << '\n';

	for (int i = 0; i < N; i++) {
		gbm_path *= exp((mu - 0.5*pow(sigma, 2))*dt + sigma*sqrt(dt)*dnorm(generator));
		csv_stream << gbm_path << '\n';
	}

	std::cout << "Saved to file: " << csv_path << std::endl;

	csv_stream.close();

	return 0;
}