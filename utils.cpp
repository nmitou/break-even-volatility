/* compile with g++ -I C:\local\eigen-3.4.0 -c utils.cpp */
#include "utils.h"
#include <Eigen/Dense>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <functional>
// #include <exception>

namespace data_utils 
{
	/* 
		Function reads CSV from csv_path and outputs an Eigen array of corresponding size.
		Assumes CSV could have two columns of date, spot prices or just one column of 
		spot prices. Handles both cases and extracts dates from former case for potential
		future use. */
	Eigen::ArrayXXd CSVToEigenArray(std::string csv_path) {
		std::ifstream csv(csv_path);
		if(!csv) {
			std::cerr << "File could not be opened." << std::endl;
			std::cerr << "Error code: " << std::strerror(errno) << std::endl;
			csv.close();
			std::terminate(); // as yet, unsure if this is really bad. use < throw; > ?
		}

		std::vector<std::string> dates;
		std::vector<double> spots;

		// get first line to check if there's a comma
		std::string line;
		std::getline(csv, line);
		unsigned int nRows = 1;
		
		int commaPos = line.find(",");
		
		if(commaPos != std::string::npos) { // then csv has dates and spot prices columns
			// extract the first line
			std::stringstream lineStream(line);
			std::string cell;
			std::getline(lineStream, cell, ',');
			dates.push_back(cell);
			std::getline(lineStream, cell);
			spots.push_back(std::stod(cell));
			// do the rest of the lines
			while(std::getline(csv, line)) {
				std::stringstream lineStream(line);
				std::getline(lineStream, cell, ',');
				dates.push_back(cell);
				std::getline(lineStream, cell);
				spots.push_back(std::stod(cell));
				nRows++;
			}
		} else { // then csv only has spot price column
			// again, extract the first line
			spots.push_back(std::stod(line));
			// do the remaining lines
			while(std::getline(csv, line)) {
				spots.push_back(std::stod(line));
				nRows++;
			}
		}

		csv.close();
		return Eigen::Map<Eigen::ArrayXXd>(spots.data(), nRows, 1);
	}
}

namespace norm_dbn_utils
{
	// PDF for normal distribution
	double NormalProbabilityDensityFunction(double& x, double& mu, double& sigma) {
		return (1.0 / (sigma * std::sqrt(2 * M_PI))) * std::exp(-0.5 * std::pow((x - mu) / sigma, 2));
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