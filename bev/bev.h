#ifndef BEV_H
#define BEV_H

#include <Eigen/Dense>
#include <vector>
#include <string>

namespace bev {
	class BEV {
		Eigen::ArrayXXd path_; // time series of daily data over which break-even volatility (henceforth BEV) computations will be performed
		double interest_rate_; // constant interest rate over time period of path
		std::vector<double> strikes_; // strike prices for which to find BEV outputs, inputted as percentage moneyness i.e. {0.95, 1.00, 1.05}
		std::vector<int> maturities_; // vector of maturities (in months) for which to find BEV outputs, each month is assumed to hold 21 trading days
		double dt_ = 1.0 / 252.0;
		int days_per_month_ = 21;
		// Checks that data inputted is valid with assertions. The Eigen array must be a column vector with adequate points corresponding to the maturities entered.
		void DataValid();

	public:
		// Default constructor
		BEV() {}; 
		// Constructors with filepath as string that creates path_ from the CSV data or one which directly sets an eigen array (column vector) to path_.
		// USAGE:	If CSV is chosen, one must either set header to true and then set the desired column name. Otherwise, leave the former two parameters as default
		// 			and set the column number.
		BEV(std::string csv_path, int col_no = -1, bool header = false, std::string col_name = "undefined"); 
		BEV(Eigen::ArrayXXd path);
		/*	
		Constructors which sets all of the instance variables above. Usage: one can input one-element vectors (for strikes, maturities) 
		which will give a single BEV result when SolveForBEV is called, or multiple strikes and a one-element maturity vector
		for a single skew, or multiple strikes and maturities for a surface. Note: the maturities vector is a vector of contract 
		terms/times to maturity in months, with a month assumed to hold 21 trading days.
		See USAGE above for setting parameters related to CSV data (header, col_name, col_no). */
		BEV(std::string csv_path, double interest_rate, std::vector<double> strikes, std::vector<int> maturities, int col_no = -1, bool header = false, std::string col_name = "undefined");
		BEV(Eigen::ArrayXXd path, double interest_rate, std::vector<double> strikes, std::vector<int> maturities);

		// Setters, if not set with a constructor:
		void SetData(std::string csv_path, int col_no = -1, bool header = false, std::string col_name = "undefined"); // see USAGE above for setting parameters
		void SetData(Eigen::ArrayXXd path);
		void SetMaturities(std::vector<int> maturities); 
		void SetInterestRate(double interest_rate) { interest_rate_ = interest_rate; };
		void SetStrikes(std::vector<double> strikes) { strikes_ = strikes; };

		// Getters:
		Eigen::ArrayXXd GetPath() { return path_; };
		double GetInterestRate() { return interest_rate_; };
		std::vector<double> GetStrikes() { return strikes_; };
		std::vector<int> GetMaturities() { return maturities_; };

		// Solving for BEV:
		/*
		Call function to solve for break-even volatilities of the given path/data with the set strikes and maturities.
		Naturally, when the path given is longer than the maturity for which the BEV is being computed, the path is broken
		up into equal-length subpaths.
		The parameter average_pnls is used to set whether the break-even volatility should be found by zeroing 
		the average of the subpath PnLs (when true), or rather (when false) averaging the break-even volatilities 
		of each subpath for a specific maturity, strike combination. 
		Returns an Eigen array where the rows correspond to the maturities and columns to the strikes used, i.e.
		each row represents and volatility skew. */
		Eigen::ArrayXXd SolveForBEV(bool average_pnls = true); 
		/*
		This function performs the same procedure as above, except for a specific strike, maturity combination, and returns
		an array of break-even volatilities for each subpath. This corresponds to the case above when average_pnls==false, prior 
		to averaging the subpaths' BEV estimates for the final result (for that specific strike, maturity combination).*/
		Eigen::ArrayXXd SolveForBEV(double strike, double maturity);
		/* To add in future:
		Daily delta-hedged profit and loss (PnL) function. Less stable/robust than formula derived from continuous delta-hedging. */
		// double DailyDHPnL(double sigma, !!! other params); 
		/*
		Continuously delta-hedged profit and loss (PnL) function discretised into daily timesteps. Usage: used in lambda function to create
		a function of sigma only, and returns the associated PnL of a option with given strike, maturity over the corresponding paths.
		Multiple subpaths can be entered (as rows in the Eigen array paths) in which case the average PnL will be returned, or alternatively
		a row vector representing a single path can be entered for a single BEV estimate. */
		double ContinuousDHPnL(double sigma, const Eigen::ArrayXXd& paths, double strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity);

		/*
		Computes the Black Scholes Gamma at each point in time along the path/row. */
		Eigen::ArrayXXd BlackScholesGamma(double sigma, const Eigen::ArrayXXd& paths, double strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity); // may need to overload for final parameter

		/*	
		Creates an array of subpaths (rows) of length 21 (days) * term_in_months, with the number of rows 
		depending on the size of the sample data and the corresponding term	parameter entered. */
		Eigen::ArrayXXd GetSubPaths(int term_in_months);
	};
}

#endif