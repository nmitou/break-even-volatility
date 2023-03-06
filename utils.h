#ifndef BEV_UTILS
#define BEV_UTILS

#include <Eigen/Dense>
#include <numeric>

namespace eigen_utils 
{
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
	/**
	 * Cumulatively sums a matrix over its rows or columns (specified).
	 * 
	 * @tparam Derived derived class from MatrixBase
	 * @param[in] m matrix over which the cumulative sum will be performed
	 * @param[in] dim dimension over which to sum, 0 for rows or 1 for columns
	 * 
	 * @returns Derived matrix class with same shape as input matrix, with cumulative
	 * 			sum performed over its rows/columns
	 */
	template <typename Derived>
	Derived cumsum(const Eigen::MatrixBase<Derived>& m, int dim) {
		assert(dim==0 || dim==1);
		if (dim) {
			int nr = m.rows();
			return Eigen::MatrixXd::Ones(nr,nr).triangularView<Eigen::Lower>() * m;
		} else {
			int nc = m.cols();
			return m * Eigen::MatrixXd::Ones(nc,nc).triangularView<Eigen::Upper>();
		}
	}
}

namespace norm_dbn_utils
{

}

namespace data_utils 
{

}

#endif