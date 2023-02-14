/*
	Performing linear algebraic operations (matrix mult. etc.) and coefficient-wise/
	element-wise operations.

	Eigen's Matrix class has a .array() method which "converts" the matrix into an array
	so that element-wise operations can be performed. Conversely, the Array class has a
	.matrix() method which converts an array to a matrix. Thus, one can work with either
	class and have access to the other's operations.
	
	However, converting a matrix to an array for element-wise operations, namely matrix
	mult., is so common, Eigen provides a method for doing such:
	The result of m.array() * n.array(), where m and n are matrices, would be the same
	as m.cwiseProduct(n).
*/

#include <iostream>
#include <Eigen/Dense>

using Eigen::Matrix2d;
using Eigen::Array22d;

int main() {
	Matrix2d m {{1, 2},
				{3, 4}};
	Matrix2d n {{5, 6},
				{7, 8}};
	Array22d a {{5, 6},
			   	{7, 8}};

	std::cout << "matrix m =\n" << m << std::endl;
	std::cout << "matrix n =\n" << n << std::endl;
	std::cout << "array a =\n" << a << std::endl;

	std::cout << "Matrix multiplication:" << std::endl;
	std::cout << "result of m * n =\n" << m*n << std::endl;
	std::cout << "result of m * a.matrix() =\n" << m*a.matrix() << std::endl;

	std::cout << "Element/coefficient-wise multiplication:" << std::endl;
	std::cout << "result of m.array() * n.array() =\n" << m.array()*n.array() << std::endl;
	std::cout << "result of m.array() * a =\n" << m.array()*a << std::endl;
	std::cout << "result of m.cwiseProduct(n) =\n" << m.cwiseProduct(n) << std::endl;

	return 0;
}