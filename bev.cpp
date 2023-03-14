#include "bev.h"
#include "utils.h"
#include <Eigen/Dense>
#include <string>

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
BEV::BEV(double interest_rate, std::vector strikes, std::vector maturities) 
	: interest_rate_(interest_rate)
	, strikes_(strikes)
	, maturities_(maturities)
	{}
/*
Constructor with all private variables set. See comment above. */
BEV::BEV(std::string csv_path, double interest_rate, std::vector strikes, std::vector maturities)
	: path_(data_utils::CSVToEigenArray(csv_path))
	, interest_rate_(interest_rate)
	, strikes_(strikes)
	, maturities_(maturities)
	{}

// SETTERS
void BEV::SetData(std::string csv_path) { path_ = data_utils::CSVToEigenArray(csv_path); }

// SOLVING FOR BEV

double BEV::ContinuousDHPnL(double sigma, Eigen::ArrayXXd& paths, double strike, double maturity) {
	// test whether paths has single row or column

}

/*	
Creates an array of subpaths where each row is a sub-sample/path of length 21 (days) * term_in_months
and the number of rows is dependent on the size of the sample data and the corresponding term 
parameter entered. */
Eigen::ArrayXXd BEV::GetSubPaths(int term_in_months) {
	int days_to_maturity = term_in_months*21;
	int n_sub_paths = path_.size() / (days_to_maturity); // integer division => removes remaining datapoints which are less than a full sub path
	return path_(seq(0, n_sub_paths*days_to_maturity - 1), all).reshaped<Eigen::RowMajor>(n_sub_paths, days_to_maturity);
}
