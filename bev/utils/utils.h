#ifndef BEV_UTILS_H
#define BEV_UTILS_H

#include <Eigen/Dense>
#include <numeric>
#include <string>
#include <functional>
#include <cmath>
#include <cassert>
#include <vector>

namespace math_constants 
{
	const double pi = 3.14159265358979323846;
}

namespace eigen_utils 
{
	/* 
	Utility functions for operating on Eigen datatypes, declared and defined in this header as they are template functions. 
	*/
	
	// Cumulatively sums an Eigen array over its rows or columns (specified as dim, 0 for rows or 1 for columns).
	template <typename Derived>
	void cumsum(Eigen::ArrayBase<Derived>& a, int dim) {
		assert(dim==0 || dim==1 && "Must choose dimension as 0 (row-wise) or 1 (col-wise).");
		if (dim) {
			for (auto col : a.colwise())
				std::partial_sum(col.begin(), col.end(), col.begin());
		} else {
			for (auto row : a.rowwise())
				std::partial_sum(row.begin(), row.end(), row.begin());
		}
	}
	/*
	Cumulatively sums an Eigen matrix over its rows or columns (specified as dim, 0 for rows or 1 for columns). 
	Returns a matrix of same shape/size.
	*/
	template <typename Derived>
	Derived cumsum(const Eigen::MatrixBase<Derived>& m, int dim) {
		assert(dim==0 || dim==1 && "Must choose dimension as 0 (row-wise) or 1 (col-wise).");
		if (dim) {
			int nr = m.rows();
			return Eigen::MatrixXd::Ones(nr,nr).triangularView<Eigen::Lower>() * m;
		} else {
			int nc = m.cols();
			return m * Eigen::MatrixXd::Ones(nc,nc).triangularView<Eigen::Upper>();
		}
	}

	// Probability density function for normal distribution applied element-wise to array
	template <typename Derived>
	Derived NormPDF(const Eigen::ArrayBase<Derived>& a, double mu = 0, double sigma = 1) {
		return (1 / (sigma * std::sqrt(2*math_constants::pi))) * ((-0.5 * (((a - mu) / sigma).pow(2))).exp());
	}
}

namespace data_utils 
{
	/*
	Function for reading in data

	Reads in a specific column from the CSV specified in csv_path and outputs an Eigen array (column vector) of corresponding size.
	USAGE:	Set header to true and input the desired column name in the CSV, can leave col_no as default.
			Otherwise, if header is set to false then input the desired column number, leaving col_name as default.
	*/

	Eigen::ArrayXXd CSVToEigenArray(std::string csv_path, int col_no = -1, bool header = false, std::string col_name = "undefined");
}

namespace norm_dbn_utils
{
	/*
	Utility functions providing functionality on normally distributed random variables.
	*/

	// PDF for normal distribution
	double NormalProbabilityDensityFunction(double x, double mu, double sigma);
}

namespace bev_utils {
	/*
	Utility functions for the break-even volatility methodology and the optimisation processes it requires.
	*/
	// double RootByNewtonsMethod() // need derivative of pnl function wrt sigma

	// Function returns the root of the inputted function f (usage with lambda function) via the secant method,
	// like Newton's method but uses approx. derivative.
	// Parameters: x0 = starting point, initial_step_size to calculate first secant, xtol/ftol for convergence/stopping criteria
	double RootBySecantMethod(std::function<double(double)> f, double x0, double initial_step_size = 0.01, double xtol = 1e-12, double ftol = 1e-12);

	/*	Function to print volatility surface to std::cout. */
	void PrintResults(std::vector<double> strikes, std::vector<int> maturities, Eigen::ArrayXXd volatilities, int decimal_precision = 4);
}
#endif