#include "bev.h"
#include "utils.h"
#include <Eigen/Dense>
#include <string>
#include <cmath>

using namespace bev;

// CONSTRUCTORS
/*
Construct with filepath to CSV data. */
BEV::BEV(std::string csv_path) 
	: path_(data_utils::CSVToEigenArray(csv_path)) {}// path_ = data_utils::CSVToEigenArray(csv_path);}
/*	
Another constructor. Usage: one can input one-element vectors (for strikes, maturities) which will give 
a single BEV result, or multiple strikes and a one-element maturity vector for a single skew, or 
multiple strikes and maturities for a surface. */
BEV::BEV(double interest_rate, std::vector<double> strikes, std::vector<int> maturities) 
	: interest_rate_(interest_rate)
	, strikes_(strikes)
	, maturities_(maturities)
	{}
/*
Constructor with all private variables set. See comment above. */
BEV::BEV(std::string csv_path, double interest_rate, std::vector<double> strikes, std::vector<int> maturities)
	: path_(data_utils::CSVToEigenArray(csv_path))
	, interest_rate_(interest_rate)
	, strikes_(strikes)
	, maturities_(maturities)
	{}

// SETTERS
void BEV::SetData(std::string csv_path) { path_ = data_utils::CSVToEigenArray(csv_path); }

// SOLVING FOR BEV

Eigen::ArrayXXd BEV::SolveForBEV(bool average_pnls) {
	// if average_pnls = false, then must redo loop to only use 1 path at a time and then average, adds an extra loop
	Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic> BEV_array(maturities_.size(), strikes_.size());
	int row = 0; 
	int col = 0;

	for (auto term_in_months : maturities_) {

		Eigen::ArrayXXd paths = GetSubPaths(term_in_months);
		int days_to_maturity = term_in_months*days_per_month_;
		Eigen::Array<double, 1, Eigen::Dynamic> times_to_maturity = Eigen::ArrayXd::LinSpaced(days_to_maturity, (double) days_to_maturity-1, 0.0) * dt_;

		for (auto strike : strikes_) {

			auto PnLToZero = [&paths, &strike, &times_to_maturity, this] (double sigma) -> double {
				return this->ContinuousDHPnL(sigma,
											paths,
											strike,
											times_to_maturity);
			};

			BEV_array(row, col) = bev_utils::RootBySecantMethod(PnLToZero, 0.99);

			col++;
		}
		row++;
		col = 0;
	}

	return BEV_array;
}

double BEV::ContinuousDHPnL(double sigma, Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity) {
	
	int T = paths.cols()-1; 

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

/*
Computes the Black Scholes Gamma at each point in time along the path/row. */
Eigen::ArrayXXd BEV::BlackScholesGamma(double sigma, const Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity) {
	Eigen::ArrayXXd d1 = ((paths/strike).log().rowwise() + (interest_rate_ + 0.5*(sigma*sigma))*times_to_maturity).rowwise() / (sigma*(times_to_maturity.sqrt()));
	return eigen_utils::NormPDF(d1) / (sigma*(paths.rowwise()*(times_to_maturity.sqrt())));
}

/*	
Creates an array of subpaths where each row is a sub-sample/path of length 21 (days) * term_in_months
and the number of rows is dependent on the size of the sample data and the corresponding term 
parameter entered. */
Eigen::ArrayXXd BEV::GetSubPaths(int term_in_months) {
	int days_to_maturity = term_in_months*days_per_month_;
	int n_sub_paths = path_.size() / (days_to_maturity); // integer division => removes remaining datapoints which are less than a full sub path
	return path_(Eigen::seq(0, n_sub_paths*days_to_maturity - 1), Eigen::all).reshaped<Eigen::RowMajor>(n_sub_paths, days_to_maturity);
}
