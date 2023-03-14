#ifndef BEV_H
#define BEV_H

#include <Eigen/Dense>
#include <vector>
#include <string>

namespace bev {
	class BEV {
		Eigen::ArrayXXd path_;
		double interest_rate_;
		std::vector strikes_;
		std::vector maturities_;
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
		Another constructor. Usage: one can input one-element vectors (for strikes, maturities) 
		which will give a single BEV result, or multiple strikes and a one-element maturity vector
		for a single skew, or multiple strikes and maturities for a surface. */
		BEV(double interest_rate, std::vector strikes, std::vector maturities);
		// Constructor with all private variables set. See comment above.
		BEV(std::string csv_path, double interest_rate, std::vector strikes, std::vector maturities);

		// Setters:
		void SetData(std::string csv_path);
		void SetInterestRate(double interest_rate) { interest_rate_ = interest_rate; }
		void SetStrikes(std::vector strikes) { strikes_ = strikes; }
		void SetMaturities(std::vector maturities) { maturities_ = maturities; }
		// void SetDateRange(); // for future
		// void SetDataIndexRange();
		// Getters:
		double GetInterestRate() { return interest_rate_; }
		std::vector GetStrikes() { return strikes_; }
		std::vector GetMaturities { return maturities_; }

		// Solving for BEV:

		Eigen::ArrayXXd SolveForBEV(); // average pnls? flag; check best way for flag variable in c++

		// double DailyDHPnL(double sigma, !!! other params); 
		double ContinuousDHPnL(double sigma, Eigen::ArrayXXd& paths, double strike, double maturity); 

		/*	
		Creates an array of subpaths (rows) of length 21 (days) * term_in_months, with the number of rows 
		depending on the size of the sample data and the corresponding term	parameter entered. */
		Eigen::ArrayXXd GetSubPaths(int term_in_months);

		// make functions work for single path of matrix/array of paths (decide which dimension for time and path number)
		// use lambda function of sigma when using this in pnl function
		// use reference for paths
		Eigen::ArrayXXd BlackScholesGamma(Eigen::ArrayXXd paths, double strike, double maturity, double interest_rate, double sigma);

		
		

	}
}

#endif