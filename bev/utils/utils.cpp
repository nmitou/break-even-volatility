/* compile with g++ -I C:\local\eigen-3.4.0 -c utils.cpp */
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
}