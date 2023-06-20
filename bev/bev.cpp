#include "bev.h"
#include "utils.h"
#include <Eigen/Dense>
#include <string>
#include <cmath>
#include <vector>

using namespace bev;

// Function which tests whether data inputted is valid. The Eigen array must be a column vector with enough/adequate points
// corresponding to the terms/maturities entered. 
void BEV::DataValid() {
	assert(path_.cols() == 1 && "Data must be an Eigen array with dimensions (N, 1) i.e. column vector.");
	if (maturities_.size() > 0) { // check if maturities_ has been initialised
		for (int m : maturities_)
			assert(path_.size() >= m*21 && "Insufficient data size for maturity selected."); // otherwise not enough data for maturity which fails this test
	}
}

// CONSTRUCTORS
/*
Construct with filepath to CSV data.
Passes other necessary parameters to CSV parser, i.e. header, column name, column number. */
BEV::BEV(std::string csv_path, int col_no, bool header, std::string col_name) 
	: path_(data_utils::CSVToEigenArray(csv_path, col_no, header, col_name)) 
{
	DataValid();
}
/*
Construct with Eigen array. */
BEV::BEV(Eigen::ArrayXXd path)
	: path_(path)
{
	DataValid();
}
/*	
Constructor with all private variables set. Usage: one can input one-element vectors (for strikes, maturities) 
which will give a single BEV result when SolveForBEV is called, or multiple strikes and a one-element maturity vector
for a single skew, or multiple strikes and maturities for a surface. 
As above, header, col_name and col_no relate to the CSV data and which column is desired. */
BEV::BEV(std::string csv_path, double interest_rate, std::vector<double> strikes, std::vector<int> maturities, int col_no, bool header, std::string col_name)
	: path_(data_utils::CSVToEigenArray(csv_path, col_no, header, col_name))
	, interest_rate_(interest_rate)
	, strikes_(strikes)
	, maturities_(maturities)
{
	DataValid();
}
// Same constructor except with Eigen array instead of filepath to csv
BEV::BEV(Eigen::ArrayXXd path, double interest_rate, std::vector<double> strikes, std::vector<int> maturities)
	: path_(path)
	, interest_rate_(interest_rate)
	, strikes_(strikes)
	, maturities_(maturities)
{
	DataValid();
}

// SETTERS
void BEV::SetData(std::string csv_path, int col_no, bool header, std::string col_name) { 
	path_ = data_utils::CSVToEigenArray(csv_path, col_no, header, col_name);
	DataValid();
}
void BEV::SetData(Eigen::ArrayXXd path) { 
	path_ = path;
	DataValid();
}
void BEV::SetMaturities(std::vector<int> maturities) {
	maturities_ = maturities; 
	if (path_.size() > 0) // check if path has been initialised
		DataValid(); 
}

// SOLVING FOR BEV
/*
Call function to solve for break-even volatilities of the given path/data with the set strikes and maturities.
Naturally, when the path given is longer than the maturity for which the BEV is being computed, the path is broken
up into equal-length subpaths.
The parameter average_pnls is used to set whether the break-even volatility should be found by zeroing 
the average of the subpath PnLs (when true), or rather (when false) averaging the break-even volatilities 
of each subpath for a specific maturity, strike combination. 
Returns an Eigen array where the rows correspond to the maturities and columns to the strikes used, i.e.
each row represents and volatility skew. */
Eigen::ArrayXXd BEV::SolveForBEV(bool average_pnls) {

	Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic> BEV_array(maturities_.size(), strikes_.size());
	int row = 0; 
	int col = 0;

	for (int term_in_months : maturities_) {

		Eigen::ArrayXXd paths = GetSubPaths(term_in_months);
		int days_to_maturity = term_in_months*days_per_month_;
		Eigen::Array<double, 1, Eigen::Dynamic> times_to_maturity = Eigen::ArrayXd::LinSpaced(days_to_maturity, (double) days_to_maturity-1, 0.0) * dt_;

		for (double strike : strikes_) {

			if (average_pnls) {
				auto PnLToZero = [&paths, &strike, &times_to_maturity, this] (double sigma) -> double {
					return this->ContinuousDHPnL(sigma, paths, strike, times_to_maturity);
				};
				BEV_array(row, col) = bev_utils::RootBySecantMethod(PnLToZero, 0.99);
			} else {
				Eigen::Array<double, Eigen::Dynamic, 1> path_BEVs(paths.rows(), 1);
				int n = 0;
				for (auto path : paths.rowwise()) {
					auto PnLToZero = [&path, &strike, &times_to_maturity, this] (double sigma) -> double {
						return this->ContinuousDHPnL(sigma, path, strike, times_to_maturity);
					};
					path_BEVs(n, 0) = bev_utils::RootBySecantMethod(PnLToZero, 0.99);
					n++;
				}
				BEV_array(row, col) = path_BEVs.mean();
			}	
			col++;
		}
		row++;
		col = 0;
	}

	return BEV_array;
}

/*
This function performs the same procedure as above, except for a specific strike, maturity combination, and returns
an array of break-even volatilities for each subpath. This corresponds to the case above when average_pnls==false, prior 
to averaging the subpaths' BEV estimates for the final result (for that specific strike, maturity combination).*/
Eigen::ArrayXXd BEV::SolveForBEV(double strike, double maturity) {
	// Check there'e enough data for this maturity
	assert(path_.size() >= maturity*21 && "Insufficient data size for maturity selected.");

	Eigen::ArrayXXd paths = GetSubPaths(maturity);
	int days_to_maturity = maturity*days_per_month_;
	Eigen::Array<double, 1, Eigen::Dynamic> times_to_maturity = Eigen::ArrayXd::LinSpaced(days_to_maturity, (double) days_to_maturity-1, 0.0) * dt_;

	Eigen::Array<double, Eigen::Dynamic, 1> path_BEVs(paths.rows(), 1);
	int n = 0;
	for (auto path : paths.rowwise()) {
		auto PnLToZero = [&path, &strike, &times_to_maturity, this] (double sigma) -> double {
			return this->ContinuousDHPnL(sigma, path, strike, times_to_maturity);
		};
		path_BEVs(n, 0) = bev_utils::RootBySecantMethod(PnLToZero, 0.99);
		n++;
	}
	return path_BEVs;
}

/*
Continuously delta-hedged profit and loss (PnL) function discretised into daily timesteps.
Multiple subpaths can be entered (as rows in the Eigen array paths) in which case the average PnL will be returned, or alternatively
a row vector representing a single path can be entered for a single BEV estimate. */
double BEV::ContinuousDHPnL(double sigma, const Eigen::ArrayXXd& paths, double strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity) {
	int T = paths.cols()-1; 
	return ((BlackScholesGamma(sigma, paths(Eigen::all, Eigen::seq(0, T-1)), strike, times_to_maturity(Eigen::seq(0, T-1))) * 												// Gamma_ti
			paths(Eigen::all, Eigen::seq(0, T-1)).pow(2) *																													// S^2_ti
			(sigma*sigma*dt_ - ((paths(Eigen::all, Eigen::seq(1, T)) - paths(Eigen::all, Eigen::seq(0, T-1))) / paths(Eigen::all, Eigen::seq(0, T-1))).pow(2))).rowwise() * // sigma^2 * dt - (dS_ti / S_ti)^2
			((interest_rate_ * times_to_maturity(Eigen::seq(0, T-1))).exp())).rowwise().sum().mean(); 																		// e^(r*(T-ti)) ... sum rows, average pnls from each path
}

/*
Computes the Black Scholes Gamma at each point in time along the path/row. */
Eigen::ArrayXXd BEV::BlackScholesGamma(double sigma, const Eigen::ArrayXXd& paths, double strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity) {
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
	// reshape array where each row holds a subpath
	Eigen::ArrayXXd sub_paths = path_(Eigen::seq(0, n_sub_paths*days_to_maturity - 1), Eigen::all).reshaped<Eigen::RowMajor>(n_sub_paths, days_to_maturity);
	// rebase subpaths by dividing each by starting value
	return sub_paths.colwise() / sub_paths(Eigen::all, 0);
}
