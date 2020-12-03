#include "lib.hh"

#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;
using namespace std::filesystem;

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

std::vector<std::string> readLines(const std::string& basename) {
	auto infile = openInputFile(basename);
	std::string buffer;
	std::vector<std::string> lines;
	while (getline(infile, buffer)) {
		lines.push_back(buffer);
	}
	return lines;
}

std::vector<int> readIntegers(const std::string& basename) {
	auto infile = openInputFile(basename);
	std::string buffer;
	std::vector<int> numbers;

	while (getline(infile, buffer)) {
		int current = stoi(buffer);
		numbers.push_back(current);
	}
	return numbers;
}

std::vector<std::string> split(const std::string& original, char delimiter) {
	vector<string> result;
	string buffer;
	istringstream source(original);
	while (getline(source, buffer, delimiter)) {
		result.push_back(buffer);
	}
	return result;
}