#include "utils.h"
#include <Eigen/Dense>
#include <string>

namespace data_utils {
	/* 
		Function reads CSV from csvPath and outputs an Eigen array of corresponding size.
		Assumes CSV could have two columns of date, spot prices or just one column of 
		spot prices. Handles both cases and extracts dates from former case for potential
		future use. */
	Eigen::ArrayXXd CSVToEigenArr(std::string csvPath) {
		std::ifstream csv(csvPath);
		if(!csv) {
			std::cerr << "File could not be opened." << std::endl;
			std::cerr << "Error code: " << std::strerror(errno) << std::endl;
			csv.close();
			std::terminate(); // as yet, unsure if this is really bad. use < throw; > ?
		}

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
}