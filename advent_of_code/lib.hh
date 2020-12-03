#ifndef ADVENT_OF_CODE_LIB_HH
#define ADVENT_OF_CODE_LIB_HH

#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>

std::ifstream openInputFile(const std::string& basename) {
	char* directory = getenv("ADVENT_OF_CODE_DATA_DIR");
	if (!directory) {
		directory = ".";
	}

	auto dataPath = std::filesystem::path(directory) / basename;
	std::ifstream infile(dataPath);
	std::string buffer;

	if (!infile.good()) {
		std::cerr << "File " << absolute(dataPath) << " could not be opened." << std::endl;
		exit(-1);
	}
	return infile;
}

std::vector<int> integersFromFile(const std::string& basename) {
	auto infile = openInputFile(basename);
	std::string buffer;
	std::vector<int> numbers;

	while (getline(infile, buffer)) {
		int current = stoi(buffer);
		numbers.push_back(current);
	}
	return numbers;
}
#endif