#include <iostream>
#include <Eigen/Dense>
#include "utils.h"

using Eigen::seq; 
using Eigen::all;

Eigen::ArrayXXd GetSubPaths(Eigen::ArrayXXd& path, int term_in_months);

int main() {

	// read csv to eigen array - utils
	Eigen::ArrayXXd path = data_utils::CSVToEigenArray("sampledata.csv");

	std::cout << "Dataset size: " << path.size() << std::endl;

	std::cout << "First 10 data points (transposed):\n" << path(seq(0, 10), all).transpose() << std::endl;

	// path.reshaped(13, 97);

	// std::cout << "Dataset reshaped:\n" << path.reshaped(13, 97);
	
	// int term_in_months = 3;

	// func
	// int days_to_maturity = term_in_months*21;
	// int data_size = path.size();
	// int n_sub_paths = data_size/days_to_maturity;

	// std::cout << "Number of potential sub paths = " << n_sub_paths << std::endl;

	// std::cout << "Paths:\n" << path(seq(0, n_sub_paths*days_to_maturity-1), all).reshaped(n_sub_paths, term_in_months*21) << std::endl;

	// Eigen::ArrayXXd new_paths = path(seq(0, n_sub_paths*days_to_maturity-1), all).reshaped<Eigen::RowMajor>(n_sub_paths, term_in_months*21);
	Eigen::ArrayXXd new_paths = GetSubPaths(path, 1);
	std::cout << "New dataset size: " << new_paths.size() << std::endl;
	std::cout << "new_paths({0, 1, 2}, all) =\n" << new_paths({0, 1, 2}, all) << std::endl;
	std::cout << "changing first three data points..." << std::endl;
	new_paths(0, 0) = 1;
	new_paths(0, 1) = 2;
	new_paths(0, 2) = 3;
	std::cout << "new_paths({0, 1, 2}, all) =\n" << new_paths({0, 1, 2}, all) << std::endl;

	// std::cout << new_paths << std::endl;

	std::cout << "Original dataset:\n" << path(seq(0, 10), all).transpose() << std::endl;

	return 0;

}

/*	Creates a matrix of subpaths where each row is a sub-sample path of length 21 (days) * term_in_months
	and the number of rows is depending on the size of the sample data and the corresponding term 
	parameter entered. */
Eigen::ArrayXXd GetSubPaths(Eigen::ArrayXXd& path, int term_in_months) {
	int days_to_maturity = term_in_months*21;
	int n_sub_paths = path.size() / (days_to_maturity); // integer division => removes remaining datapoints which are less than full sub path

	return path(seq(0, n_sub_paths*days_to_maturity - 1), all).reshaped<Eigen::RowMajor>(n_sub_paths, days_to_maturity);
}