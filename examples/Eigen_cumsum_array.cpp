/*
	Writing a cumulative sum function to operate on Eigen arrays, using colwise
	and rowwise methods, and std::partial_sum.
	See:
		https://eigen.tuxfamily.org/dox/group__TutorialSTL.html
		https://en.cppreference.com/w/cpp/algorithm/partial_sum
*/

#include <iostream>
#include <numeric>
#include <Eigen/Dense>

using Eigen::ArrayXXd;

template <typename Derived> void cumsum(Eigen::ArrayBase<Derived>& a, int dim);

int main() {
	ArrayXXd m {{1,  2,  3,  4},
				{5,  6,  7,  8},
				{9, 10, 11, 12}};
	
	std::cout << "m =\n" << m << std::endl;
	
	std::cout << "Row-wise cumsum:" << std::endl;
	cumsum(m, 0);
	std::cout << m << std::endl;

	std::cout << "Followed by col-wise cumsum:" << std::endl;
	cumsum(m, 1);
	std::cout << m;

	return 0;
}

/**
 * Cumulatively sums an array over its rows or columns (specified).
 * 
 * @tparam Derived derived class from ArrayBase
 * @param[out] a array over which the cumulative sum will be performed
 * @param[in] dim dimension over which to sum, 0 for rows or 1 for columns
 */
template <typename Derived>
void cumsum(Eigen::ArrayBase<Derived>& a, int dim) {
	assert(dim==0 || dim==1);
	if (dim) {
		for (auto col : a.colwise())
			std::partial_sum(col.begin(), col.end(), col.begin());
	} else {
		for (auto row : a.rowwise())
			std::partial_sum(row.begin(), row.end(), row.begin());
	}
}