#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <vector>
#include "utils.h"


using Eigen::seq; 
using Eigen::all;

Eigen::ArrayXXd GetSubPaths(Eigen::ArrayXXd& path, int term_in_months);
Eigen::ArrayXXd BlackScholesGamma(double sigma, const Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity);
double ContinuousDHPnL(double sigma, Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity);

int main() {

	// read csv to eigen array - utils
	Eigen::ArrayXXd path = data_utils::CSVToEigenArray("sampledata.csv");

	std::cout << "Dataset size: " << path.size() << std::endl;

	std::cout << "First 10 data points (transposed):\n" << path(seq(0, 10), all).transpose() << std::endl;

	Eigen::ArrayXXd new_paths = GetSubPaths(path, 1);
	std::cout << "New dataset size: " << new_paths.size() << std::endl;
	std::cout << "new_paths({0, 1, 2}, all) =\n" << new_paths({0, 1, 2}, all) << std::endl;
	// std::cout << "changing first three data points..." << std::endl;
	// new_paths(0, 0) = 1;
	// new_paths(0, 1) = 2;
	// new_paths(0, 2) = 3;
	// std::cout << "new_paths({0, 1, 2}, all) =\n" << new_paths({0, 1, 2}, all) << std::endl;
	
	std::cout << "Number of rows = " << new_paths.rows() << std::endl;
	std::cout << "Number of cols = " << new_paths.cols() << std::endl;
	// int N;
	// int dim_to_sum = new_paths.cols()==1 ? (N = 1, 1) : (N = new_paths.rows(), 0);
	// std::cout << "dim_to_sum = " << dim_to_sum << std::endl;
	// std::cout << "N = " << N << std::endl;

	Eigen::ArrayXXd m { {1, 2}, 
						{3, 4} };
	Eigen::ArrayXd v {{1, 2}};

	std::cout << "m =\n" << m << std::endl;
	std::cout << "v =\n" << v << std::endl;

	std::cout << "m.colwise() * v =\n" << m.colwise()*v << std::endl;
	std::cout << "m.rowwise() * v.transpose() =\n" << m.rowwise()*v.transpose() << std::endl;
	// std::cout << "v.transpose() * m.rowwise() =\n" << (v.transpose())*(m.rowwise()) << std::endl; // cannot do

	// std::cout << "Column 0 of m =\n" << m.col(0) << std::endl;
	// std::cout << "Row 1 of m =\n" << m.row(1) << std::endl;

	// std::cout << "m(all, seq(0,1)) =\n" << m(all, seq(0, 1)) << std::endl; 
	// std::cout << "Number of columns of m = " << m.cols() << std::endl;

	std::cout << "\nLinspace tests:" << std::endl;
	double dt = 1.0 / 252.0;
	Eigen::Array<double, 1, Eigen::Dynamic> times_to_maturity = Eigen::ArrayXd::LinSpaced(21, 21.0-1.0, 0.0) * dt;

	// std::cout << times_to_maturity << std::endl;
	// std::cout << "exp(T - t_i) =\n" << times_to_maturity.exp() << std::endl;

	// Eigen::ArrayXXd num = ((new_paths({0, 1, 2}, all)/100).log().rowwise() + (0.08 + 0.5*0.2*0.2)*times_to_maturity).rowwise() / (0.2*(times_to_maturity.sqrt()));
	// std::cout << "log(S / K) + (r + 0.5 * sigma^2)*t_vec =\n" << num << std::endl;
	double strike = 80.0;
	// double sigma = 0.2;
	// Eigen::ArrayXXd num = BlackScholesGamma(0.2, new_paths(all, seq(0, 20-1)), strike, times_to_maturity(seq(0, 20-1)));
	// std::cout << "BlackScholesGamma =\n" << num({0, 1, 2}, all) << std::endl;

	// std::cout << "pow test: \n" << num.pow(2)({0, 1, 2}, all) << std::endl;

	// std::vector<int> maturities{1,2,3,4};

	// std::cout << "Vector size =" << maturities.size() << std::endl;


	// int T = new_paths.cols()-1;

	// Eigen::ArrayXXd S2_ti = new_paths(Eigen::all, Eigen::seq(0, T-1)).pow(2);

	// Eigen::ArrayXXd Gamma_ti  = BlackScholesGamma(sigma, new_paths(Eigen::all, Eigen::seq(0, T-1)), strike, times_to_maturity(Eigen::seq(0, T-1)));

	// Eigen::ArrayXXd Squared_returns = ((new_paths(Eigen::all, Eigen::seq(1, T)) - new_paths(Eigen::all, Eigen::seq(0, T-1))) / new_paths(Eigen::all, Eigen::seq(0, T-1))).pow(2);

	// double PnL = (((Gamma_ti*S2_ti*(sigma*sigma*dt - Squared_returns)).rowwise()) * (0.08*times_to_maturity(Eigen::seq(0, T-1))).exp()).rowwise().sum().mean();
	// // // Eigen::ArrayXd PnL = ((Gamma_ti*S2_ti*(sigma*sigma*dt - Squared_returns)).rowwise()*((0.08*times_to_maturity(Eigen::seq(0, T-1))).exp())).rowwise().sum();

	// std::cout << "PnL =\n" << PnL << std::endl;



	auto PnLToZero = [&new_paths, &strike, &times_to_maturity] (double sigma) -> double {
				return ContinuousDHPnL(sigma,
										new_paths,
										strike,
										times_to_maturity);
	};

	std::cout << "calling PnLToZero with sigma = 0.2: " << PnLToZero(0.2);

	double ans = bev_utils::RootBySecantMethod(PnLToZero, 0.99);

	std::cout << "Root = " << ans << std::endl;

	std::cout << "testign array assignment:\n";
	int row = 0, col = 0;
	Eigen::ArrayXXd an_array;
	for (; row < 12; row++) {
		for (; col < 9; col++) {
			an_array(row,col) = 1.123;
		}
	}

	std::cout << an_array << std::endl;	





	// std::cout << "Original dataset:\n" << path(seq(0, 10), all).transpose() << std::endl;

	return 0;

}

double ContinuousDHPnL(double sigma, Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity) {
	std::cout << "Inside pnl func" << std::endl; // !!!
	
	int T = paths.cols()-1; 

	double dt_ = 1.0 / 252.0;

	double interest_rate_ = 0.08;

	// necessary to have gamma function? !!!

	// Eigen::ArrayXXd S2_ti = paths(Eigen::all, Eigen::seq(0, T-1)).pow(2);

	// Eigen::ArrayXXd Gamma_ti  = BlackScholesGamma(sigma, paths(Eigen::all, Eigen::seq(0, T-1)), strike, times_to_maturity(Eigen::seq(0, T-1)));

	// Eigen::ArrayXXd Squared_returns = ((paths(Eigen::all, Eigen::seq(1, T)) - paths(Eigen::all, Eigen::seq(0, T-1))) / paths(Eigen::all, Eigen::seq(0, T-1))).pow(2);

	// double PnL = ((Gamma_ti*S2_ti*(sigma*sigma*dt_ - Squared_returns)).rowwise()*((interest_rate_*times_to_maturity(Eigen::seq(0, T-1))).exp())).rowwise().sum().mean();

	return ((BlackScholesGamma(sigma, paths(Eigen::all, Eigen::seq(0, T-1)), strike, times_to_maturity(Eigen::seq(0, T-1))) * 												// Gamma_ti
			paths(Eigen::all, Eigen::seq(0, T-1)).pow(2) *																													// S2_ti
			(sigma*sigma*dt_ - ((paths(Eigen::all, Eigen::seq(1, T)) - paths(Eigen::all, Eigen::seq(0, T-1))) / paths(Eigen::all, Eigen::seq(0, T-1))).pow(2))).rowwise() * // sigma^2 * dt - (dS_ti / S_ti)^2
			((interest_rate_ * times_to_maturity(Eigen::seq(0, T-1))).exp())).rowwise().sum().mean(); 																		// e^(r*(T-ti)) ... sum rows, average pnls from each path
}

/*	Creates a matrix of subpaths where each row is a sub-sample path of length 21 (days) * term_in_months
	and the number of rows is depending on the size of the sample data and the corresponding term 
	parameter entered. */
Eigen::ArrayXXd GetSubPaths(Eigen::ArrayXXd& path, int term_in_months) {
	int days_to_maturity = term_in_months*21;
	int n_sub_paths = path.size() / (days_to_maturity); // integer division => removes remaining datapoints which are less than full sub path

	return path(seq(0, n_sub_paths*days_to_maturity - 1), all).reshaped<Eigen::RowMajor>(n_sub_paths, days_to_maturity);
}

Eigen::ArrayXXd BlackScholesGamma(double sigma, const Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity) {
	double r = 0.08;
	Eigen::ArrayXXd d1 = ((paths/strike).log().rowwise() + (r + 0.5*(sigma*sigma))*times_to_maturity).rowwise() / (sigma*(times_to_maturity.sqrt()));
	return eigen_utils::NormPDF(d1) / (sigma*(paths.rowwise()*(times_to_maturity.sqrt())));
}