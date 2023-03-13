#ifndef BEV_H
#define BEV_H

#include <Eigen/Dense>
#include <vector>

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
		BEV(); // constructor
		BEV(Eigen::ArrayXXd path); // constructor with data/path
		BEV(Eigen::ArrayXXd path, double interest_rate, std::vector strikes, std::vector maturities); // for surface, can put one element vectors for single bev result or 1-elm maturity vec for single skew

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

		// creates array/matrix of sub-paths of equal length depending on maturity of specific bev procedure
		Eigen::ArrayXXd GetSubPaths(Eigen::ArrayXXd path, int length);
		// make functions work for single path of matrix/array of paths (decide which dimension for time and path number)
		// use lambda function of sigma when using this in pnl function
		// use reference for paths
		Eigen::ArrayXXd BlackScholesGamma(Eigen::ArrayXXd paths, double strike, double maturity, double interest_rate, double sigma);

		Eigen::ArrayXXd SolveForBEV(); // average pnls? flag; check best way for flag variable in c++
		

	}
}

#endif