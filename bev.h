#ifndef BEV_H
#define BEV_H

#include <Eigen/Dense>
#include <vector>
#include <string>

namespace bev {
	class BEV {
		Eigen::ArrayXXd path_;
		double interest_rate_;
		std::vector<double> strikes_;
		std::vector<int> maturities_;
		double dt_ = 1.0 / 252.0;
		int days_per_month_ = 21;
		/* for future (i.e. date range):
		date from_date_;
		date to_date_; */
		// from and to indices for selecting specific piece out of path
		// int from_idx;
		// int to_idx;

	public:
		BEV() {}; // default constructor
		// constructor with filepath to CSV data
		BEV(std::string csv_path); 
		/*	
		Constructors with more variables set. Usage: one can input one-element vectors (for strikes, maturities) 
		which will give a single BEV result when SolveForBEV is called, or multiple strikes and a one-element maturity vector
		for a single skew, or multiple strikes and maturities for a surface. Note: the maturities vector is a vector of constract 
		terms/times to maturity in months, with a month assumed to hold 21 trading days.*/
		BEV(double interest_rate, std::vector<double> strikes, std::vector<int> maturities);
		// Constructor with all private variables set. See comment above.
		BEV(std::string csv_path, double interest_rate, std::vector<double> strikes, std::vector<int> maturities);

		// Setters, if not set in constructor:
		void SetData(std::string csv_path);
		void SetInterestRate(double interest_rate) { interest_rate_ = interest_rate; };
		void SetStrikes(std::vector<double> strikes) { strikes_ = strikes; };
		void SetMaturities(std::vector<int> maturities) { maturities_ = maturities; };
		// void SetDateRange(); // for future
		// void SetDataIndexRange();
		// Getters:
		double GetInterestRate() { return interest_rate_; };
		std::vector<double> GetStrikes() { return strikes_; };
		std::vector<int> GetMaturities() { return maturities_; };

		// Solving for BEV:
		/*
		Call function to solve for break-even volatilities of given path with the set strikes and maturities. 
		The parameter average_pnls is used to set whether the break-even volatility should be found by zeroing 
		the average of the subpath PnLs (when true), or rather (when false) averaging the break-even volatilities 
		of each subpath for a specific maturity, strike combination. */
		Eigen::ArrayXXd SolveForBEV(bool to_average_pnls = true); 
		Eigen::ArrayXXd SolveForBEV(double strike, double maturity);
		/*
		Daily delta-hedged profit and loss (PnL) function. Less stable/robust than formula derived from continuous delta-hedging. */
		// double DailyDHPnL(double sigma, !!! other params); 
		/*
		Daily delta-hedged profit and loss (PnL) function. */
		double ContinuousDHPnL(double sigma, const Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity);

		// make functions work for single path of matrix/array of paths (decide which dimension for time and path number)
		// use lambda function of sigma when using this in pnl function
		// use reference for paths
		Eigen::ArrayXXd BlackScholesGamma(double sigma, const Eigen::ArrayXXd& paths, double& strike, const Eigen::Array<double, 1, Eigen::Dynamic>& times_to_maturity); // may need to overload for final parameter

		/*	
		Creates an array of subpaths (rows) of length 21 (days) * term_in_months, with the number of rows 
		depending on the size of the sample data and the corresponding term	parameter entered. */
		Eigen::ArrayXXd GetSubPaths(int term_in_months);
	};
}

#endif