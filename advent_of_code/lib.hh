#ifndef ADVENT_OF_CODE_LIB_HH
#define ADVENT_OF_CODE_LIB_HH

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::ifstream openInputFile(const std::string& basename);

std::vector<std::string> readLines(const std::string& basename);

std::vector<int> readIntegers(const std::string& basename);

std::vector<std::string> split(const std::string& original, char delimiter = ' '); 

#endif