#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <Eigen/Dense>

int main() {
	// std::ifstream csv;
	// csv.open("example.csv");
	// or can do:
	// std::ifstream csv("example.csv");

	// std::ifstream csv("example_commas_nodates.csv");
	std::ifstream csv("example_nodates.csv");

	if(!csv) {
		std::cout << "CSV failed to open.";
		return 1;
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
		}
	} else { // then csv only has spot price column
		// again, extract the first line
		spots.push_back(std::stod(line));
		// do the remaining lines
		while(std::getline(csv, line))
			spots.push_back(std::stod(line));
	}

	Eigen::VectorXd S = Eigen::Map<Eigen::VectorXd>(spots.data(), 4, 1);

	std::cout << S;
	
	csv.close();

	return 0;
}