#ifndef BEV_UTILS_H
#define BEV_UTILS_H

#include <Eigen/Dense>
#include <numeric>
#include <string>
#include <functional>

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

	// Probability density function for normal distribution applied element-wise to array
	template <typename Derived>
	Derived NormPDF(const Eigen::ArrayBase<Derived>& a, const double& mu = 0, const double& sigma = 1) {
		return (1 / (sigma * std::sqrt(2*M_PI))) * ((-0.5 * (((a - mu) / sigma).pow(2))).exp());
	}
}

namespace norm_dbn_utils
{
	// PDF for normal distribution
	double NormalProbabilityDensityFunction(double& x, double& mu, double& sigma);
}

namespace data_utils 
{
	// Parses CSV to Eigen array, capabable of handling single column of price data
	// or two columns of (date, price).
	Eigen::ArrayXXd CSVToEigenArray(std::string csv_path);
}

namespace bev_utils {
	// like newtons but uses approx derivative
	double RootBySecantMethod(std::function<double(double)> f, double x0, double initial_step_size = 0.01, double xtol = 1e-12, double ftol = 1e-12);

	// double RootByNewtonsMethod() // need derivative of pnl function wrt sigma
}

#endif