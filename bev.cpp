#include "bev.h"
#include <Eigen/Dense>

using namespace bev;

/*	Creates an array of subpaths where each row is a sub-sample/path of length 21 (days) * term_in_months
	and the number of rows is dependent on the size of the sample data and the corresponding term 
	parameter entered. */
Eigen::ArrayXXd BEV::GetSubPaths(Eigen::ArrayXXd& path, int term_in_months) {
	int days_to_maturity = term_in_months*21;
	int n_sub_paths = path.size() / (days_to_maturity); // integer division => removes remaining datapoints which are less than full sub path
	return path(seq(0, n_sub_paths*days_to_maturity - 1), all).reshaped<Eigen::RowMajor>(n_sub_paths, days_to_maturity);
}
