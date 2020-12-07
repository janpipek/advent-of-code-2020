#ifndef ADVENT_OF_CODE_LIB_HH
#define ADVENT_OF_CODE_LIB_HH

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::ifstream openInputFile(const std::string& basename);

std::vector<std::string> readLines(const std::string& basename);

std::vector<std::vector<std::string>>readLineGroups(const std::string& basename);

std::vector<int> readIntegers(const std::string& basename);

std::vector<std::string> split(const std::string& original, char delimiter = ' ', int maxSplits = -1);

std::vector<std::string> split(const std::string& original, const std::string& delimiter, int maxSplits = -1);

#endif