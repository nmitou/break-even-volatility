/*
	Example program creating a matrix with the Eigen library.
		https://eigen.tuxfamily.org/index.php?title=Main_Page
	Quick ref manual:
		https://eigen.tuxfamily.org/dox/group__QuickRefPage.html
	Compile with:
		g++ -I /path/to/eigen/ -o Eigen_test_01 Eigen_test_01.cpp
	My path to Eigen:
		C:\local\eigen-3.4.0
*/

#include <iostream>
#include <Eigen/Dense>

int main() {
	Eigen::MatrixXd m(2,2);
	m(0,0) = 3;
	m(0,1) = -1;
	m(1,0) = 2.5;
	m(1,1) = m(1,0) + m(0,1);

	std::cout << m << std::endl;

	return 0;
}