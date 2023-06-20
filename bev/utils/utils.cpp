#include "utils.h"
#include <Eigen/Dense>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>
#include <random>

namespace data_utils 
{
	/* 
		Function reads specified column from the CSV in csv_path and outputs an Eigen array (column vector) of corresponding size.
		If header is true, function searches for specific column name, otherwise it uses the specified column
		number. */
	Eigen::ArrayXXd CSVToEigenArray(std::string csv_path, int col_no, bool header, std::string col_name) {
		// Open stream
		std::ifstream csv(csv_path);
		if(!csv) {
			std::cerr << "File could not be opened." << std::endl;
			std::cerr << "Error code: " << std::strerror(errno) << std::endl;
			csv.close();
			std::terminate(); // as yet, unsure if this is really bad. use < throw; > ?
		}
		// if header exists then make sure a column name was inputted, else need a column number
		if (header) {
			assert(col_name != "undefined" && "Insert column name");
			// get first line 
			std::string line;
			std::getline(csv, line);
			std::stringstream lineStream(line);
			// find column count of desired column
			std::string cell;
			std::getline(lineStream, cell, ',');
			int count = 0;
			while (cell != col_name) {
				std::getline(lineStream, cell, ',');
				count++;
			}
			col_no = count;
		} else
			assert(col_no != -1 && "Choose column number, zero indexed");

		std::vector<double> values;
		int nRows = 0;
		std::string line;
		// Populate values vector with value from correct column
		while (std::getline(csv, line)) {
			std::stringstream lineStream(line);
			std::string cell;
			int j = 0;
			while (j <= col_no) {
				std::getline(lineStream, cell, ',');
				j++;
			}
			values.push_back(std::stod(cell));
			nRows++;
		}
		csv.close();
		return Eigen::Map<Eigen::ArrayXXd>(values.data(), nRows, 1);
	}

	/*	Function to generate a GBM sample path with given parameters.
		Fills the column vector/(N+1)x1 array. */
	void GenerateGBMData(Eigen::ArrayXXd& S, double S_0, double mu, double sigma, int T_years, int seed) {
		// If S not initialised with correct number of data points
		if ((S.rows() != (T_years * 252) + 1) || S.cols() != 1)
			S = Eigen::ArrayXXd ((T_years * 252) + 1, 1);
		// First, we set up the generator to sample from the normal distribution
		std::mt19937 generator{seed};
		std::normal_distribution<double> dnorm(0, 1);

		int N = 21*12*T_years; // number of days, assuming 21 business days per month
		double dt = 1.0 / (21.0 * 12.0); // or (double) T/N;
		S(0,0) = 100;
		for (int i = 1; i < N+1; i++)
			S(i, 0) = S(i-1, 0) * std::exp((mu - 0.5 * std::pow(sigma, 2)) * dt + sigma * std::sqrt(dt) * dnorm(generator));
	}
}

namespace norm_dbn_utils
{
	// PDF for normal distribution
	double NormalProbabilityDensityFunction(double x, double mu, double sigma) {
		return (1.0 / (sigma * std::sqrt(2 * math_constants::pi))) * std::exp(-0.5 * std::pow((x - mu) / sigma, 2));
	}
}

namespace bev_utils 
{
	/*
	Finds the root of the function f via the secant method, which approximates the derivative of f using two points. */
	double RootBySecantMethod(std::function<double(double)> f, double x0, double initial_step_size, double xtol, double ftol) {
		double fx0 = f(x0);
		double x1 = x0 - initial_step_size;
		double fx1 = f(x1);
		double m, next_x;

		// two stopping conditions, one for x convergence and then another condition where x-axis is near parallel to f (f asymptotically approaches zero as x->0)
		while((std::abs(x1 - x0) > xtol) && (std::abs(fx1) > ftol)) { 
			fx1 = f(x1);
			m = (fx1 - fx0) / (x1 - x0);
			next_x = x0 - fx0/m;
			x0 = x1;
			fx0 = fx1;
			x1 = next_x;
		}

		return x1;
	}

	/*	Function to print volatility surface to std::cout. */
	void PrintResults(std::vector<double> strikes, std::vector<int> maturities, Eigen::ArrayXXd volatilities, int decimal_precision) {
		int c_width = decimal_precision + 4;
		int c1 = 8, c2 = 3, c3 = 2;
		std::cout << std::setprecision(decimal_precision);
		std::cout << std::left;

		// Row 1
		std::cout << std::setw(c1) << " " << std::setw(c2) << " " << std::setw(c3) << " " << std::setw(c_width) << "Strikes (%)" << '\n';
		// Row 2
		std::cout << std::setw(c1) << " " << std::setw(c2) << " " << std::setw(c3) << " " ;
		for (double k : strikes)
			std::cout << std::setw(c_width) << k*100;
		std::cout << std::endl;
		// Row 3
		std::cout << std::setw(c1) << " " << std::setw(c2) << " " << std::setw(c3) << " " ;
		for (int n = 0; n < (strikes.size() - 1)*c_width + (decimal_precision+2); n++)
			std::cout << "-";
		std::cout << std::endl;
		// Rest of table
		std::cout << std::setw(c1) << "Terms";
		for (int i = 0; i < maturities.size(); i++) {
			if (i > 0)
				std::cout << std::setw(c1) << " ";
			std::cout << std::setw(c2) << maturities[i] << std::setw(c3) << "|";
			for (auto v : volatilities.row(i)) std::cout << std::setw(c_width) << v;
			std::cout << std::endl;
		}
	}
}