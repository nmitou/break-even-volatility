/*
	Writing a cumulative sum function to operate on Eigen matrices, using simple
	matrix multiplication.
*/

#include <iostream>
#include <Eigen/Dense>
#include "../utils.h"

using Eigen::Matrix3d;
using Eigen::Vector3d;
using Eigen::RowVector3d;
using Eigen::MatrixXd;

int main() {
	Matrix3d m {{1,2,3},
				{4,5,6},
				{7,8,9}};
	Matrix3d r = Matrix3d::Ones().triangularView<Eigen::Upper>(); 
				//{{1,1,1},
				// {0,1,1},
				// {0,0,1}};
	Matrix3d c = Matrix3d::Ones().triangularView<Eigen::Lower>();
				// {{1,0,0},
				//  {1,1,0},
				//  {1,1,1}};

	std::cout << "m =\n" << m << std::endl;
	std::cout << "r =\n" << r << std::endl;
	std::cout << "c =\n" << c << std::endl;

	std::cout << "Row-wise cumsum:" << std::endl;
	std::cout << "m * r =\n" << m*r << std::endl;

	std::cout << "Column-wise cumsum:" << std::endl;
	std::cout << "c * m=\n" << c*m << std::endl;

	std::cout << "\nNow with non-square matrices:" << std::endl;
	MatrixXd p {{1,2,3,4,5},
				{6,7,8,9,10},
				{11,12,13,14,15}};
	std::cout << "p =\n" << p << std::endl;
	std::cout << "Row cumsum p =\n" << 
					p*MatrixXd::Ones(5,5).triangularView<Eigen::Upper>() << std::endl;
	std::cout << "Column cumsum p =\n" <<
					MatrixXd::Ones(3,3).triangularView<Eigen::Lower>()*p << std::endl;

	std::cout << "\nAnd now with vectors:" << std::endl;
	Vector3d cv {{1,2,3}};
	RowVector3d rv {{4,5,6}};
	std::cout << "cv =\n" << cv << std::endl;
	std::cout << "rv =\n" << rv << std::endl;

	std::cout << "cv.transpose() * r=\n" << cv.transpose()*r << std::endl;
	std::cout << "c * cv =\n" << c*cv << std::endl;
	std::cout << "rv * r =\n" << rv*r << std::endl;

	std::cout << "Using cumsum function:" << std::endl;
	std::cout << "Row cumsum m =\n" << eigen_utils::cumsum(m, 0) << std::endl;
	std::cout << "Col cumsum m =\n" << eigen_utils::cumsum(m, 1) << std::endl;
	std::cout << "Row cumsum p =\n" << eigen_utils::cumsum(p, 0) << std::endl;
	std::cout << "Col cumsum p =\n" << eigen_utils::cumsum(p, 1) << std::endl;
	std::cout << "Row cumsum cv =\n" << eigen_utils::cumsum(cv, 0) << std::endl;
	std::cout << "Col cumsum cv =\n" << eigen_utils::cumsum(cv, 1) << std::endl;
	std::cout << "Row cumsum rv =\n" << eigen_utils::cumsum(rv, 0) << std::endl;
	std::cout << "Col cumsum rv =\n" << eigen_utils::cumsum(rv, 1) << std::endl;
	// Testing assertion
	// std::cout << "Col cumsum rv =\n" << cumsum(rv, 2) << std::endl;

	return 0;
}