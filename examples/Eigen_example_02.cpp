/*
	Example using Eigen library to create a random matrix and a vector.
*/

#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

using Eigen::Matrix3d;
using Eigen::Vector3d;

int main() {
	// Using arbitrary sized matrices and vectors
	// matrix m and vector v sizes set at run time
	MatrixXd m = MatrixXd::Random(3,3);
	m = (m + MatrixXd::Constant(3,3, 1.2))*50;
	std::cout << "m = " << std::endl << m << std::endl;
	VectorXd v(3);
	v << 1, 2, 3;
	std::cout << "m * v = " << std::endl << m*v << std::endl << '\n';

	// Using fixed-sized matrices and vectors
	// matrix p and vector x sizes set at compile time
	Matrix3d p = Matrix3d::Random();
	p = (p + Matrix3d::Constant(1.2))*50;
	std::cout << "p =" << std::endl << p << std::endl;
	Vector3d x(1, 2, 3);
	std::cout << "p * x =" << std::endl << p*x << std::endl;

	return 0;
}