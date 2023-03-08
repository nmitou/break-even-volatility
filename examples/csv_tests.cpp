/*
	To compile:
		Either:
		- first compile utils.cpp in root folder to object file utils.o
			g++ -I path\to\eigen -c utils.cpp
		- then compile with
			g++ -I path\to\eigen -o csv_tests path\to\utilsobject\utils.o csv_tests.cpp
		Or:
		- compile and link all at once
			g++ -I path\to\eigen -o csv_tests path\to\utilssource\utils.cpp csv_tests.cpp
*/
#include <iostream>
#include <Eigen/Dense>
#include "../utils.h"

int main() {

	Eigen::ArrayXXd spotData1 = data_utils::CSVToEigenArray("example.csv");
	Eigen::ArrayXXd spotData2 = data_utils::CSVToEigenArray("example_commas_nodates.csv");
	Eigen::ArrayXXd spotData3 = data_utils::CSVToEigenArray("example_nodates.csv");

	std::cout << spotData1 << std::endl << '\n';
	std::cout << spotData2 << std::endl << '\n';
	std::cout << spotData3 << std::endl << '\n';

	// Eigen::ArrayXXd spotData4 = data_utils::CSVToEigenArray("does_not_exist.csv");

	return 0;
}