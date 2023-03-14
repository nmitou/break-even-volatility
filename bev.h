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
		BEV(std::string csv_path); // constructor with filepath to csv data
		BEV(std::string csv_path, double interest_rate, std::vector strikes, std::vector maturities); // for surface, can put one element vectors for single bev result or 1-elm maturity vec for single skew

		// Setters:
		void SetData(); 
		// void SetDateRange(); // for future
		void SetDataIndexRange();
		void SetInterestRate();
		void SetStrikes();
		void SetMaturities();
		// Getters:
		/// !!!

		// for solving for bev:

		// double DailyDHPnL(double sigma, !!! other params); 
		// double ContinuousDHPnL(double sigma, !!! other params); 

		/*	
		Creates an array of subpaths (rows) of length 21 (days) * term_in_months, with the number of rows 
		depending on the size of the sample data and the corresponding term	parameter entered. */
		Eigen::ArrayXXd GetSubPaths(Eigen::ArrayXXd& path, int term_in_months);

		// make functions work for single path of matrix/array of paths (decide which dimension for time and path number)
		// use lambda function of sigma when using this in pnl function
		// use reference for paths
		Eigen::ArrayXXd BlackScholesGamma(Eigen::ArrayXXd paths, double strike, double maturity, double interest_rate, double sigma);

		Eigen::ArrayXXd SolveForBEV(); // average pnls? flag; check best way for flag variable in c++
		

	}
}

#endif