#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include <vector>
#include <string>
#include <Eigen/Dense>

Eigen::ArrayXXd CSVToEigenArr(std::string csvPath) ;

int main() {

	Eigen::ArrayXXd spotData1 = CSVToEigenArr("example.csv");
	Eigen::ArrayXXd spotData2 = CSVToEigenArr("example_commas_nodates.csv");
	Eigen::ArrayXXd spotData3 = CSVToEigenArr("example_nodates.csv");

	std::cout << spotData1 << std::endl << '\n';
	std::cout << spotData2 << std::endl << '\n';
	std::cout << spotData3 << std::endl << '\n';

	return 0;
}

Eigen::ArrayXXd CSVToEigenArr(std::string csvPath) {
	std::ifstream csv(csvPath);
	if(!csv) {
		std::cerr << "File could not be opened." << std::endl;
		std::cerr << "Error code: " << std::strerror(errno) << std::endl;
		csv.close();
		// throw;
		std::terminate(); // as yet, unsure if this is really bad
	}

	/*
		Assume csv could have two columns of date, spot prices
		or just one column of spot prices. Need to handle both
		cases.
		Extracting dates for future use/changes if necessary.
	*/

	std::vector<std::string> dates;
	std::vector<double> spots;

	// get first line to check if there's a comma
	std::string line;
	std::getline(csv, line);
	unsigned int nRows = 1;
	
	int commaPos = line.find(",");

	if(commaPos != std::string::npos) { // then csv has dates and spot prices columns
		// extract the first line
		std::stringstream lineStream(line);
		std::string cell;
		std::getline(lineStream, cell, ',');
		dates.push_back(cell);
		std::getline(lineStream, cell);
		spots.push_back(std::stod(cell));
		// do the rest of the lines
		while(std::getline(csv, line)) {
			std::stringstream lineStream(line);
			std::getline(lineStream, cell, ',');
			dates.push_back(cell);
			std::getline(lineStream, cell);
			spots.push_back(std::stod(cell));
			nRows++;
		}
	} else { // then csv only has spot price column
		// again, extract the first line
		spots.push_back(std::stod(line));
		// do the remaining lines
		while(std::getline(csv, line)) {
			spots.push_back(std::stod(line));
			nRows++;
		}
	}

	csv.close();
	return Eigen::Map<Eigen::ArrayXXd>(spots.data(), nRows, 1);
}