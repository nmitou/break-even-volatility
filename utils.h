#include <Eigen/Dense>

namespace bev_utils {

	namespace eigen_utils 
	{
		// Cumsum function for arrays over specified dimension (0=rows, 1=columns), modifies given array
		template <typename Derived> void cumsum(Eigen::ArrayBase<Derived>& a, int dim);
		// Cumsum function for matrices over specified dimension (0=rows, 1=columns), returns new matrix
		template <typename Derived> Derived cumsum(const Eigen::MatrixBase<Derived>& m, int dim);
	}

	namespace norm_dbn_utils
	{

	}
}