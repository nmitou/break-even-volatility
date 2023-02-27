/*
	Writing a cumulative sum function to operate on Eigen arrays, using colwise
	and rowwise methods, and std::partial_sum.
	See:
		https://eigen.tuxfamily.org/dox/group__TutorialSTL.html
		https://en.cppreference.com/w/cpp/algorithm/partial_sum
*/

#include <iostream>
#include <Eigen/Dense>
#include "../utils.h"

using Eigen::ArrayXXd;

int main() {
	ArrayXXd m {{1,  2,  3,  4},
				{5,  6,  7,  8},
				{9, 10, 11, 12}};
	
	std::cout << "m =\n" << m << std::endl;
	
	std::cout << "Row-wise cumsum:" << std::endl;
	eigen_utils::cumsum(m, 0);
	std::cout << m << std::endl;

	std::cout << "Followed by col-wise cumsum:" << std::endl;
	eigen_utils::cumsum(m, 1);
	std::cout << m;

	return 0;
}