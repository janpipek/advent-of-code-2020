#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"
#include "day3.hh"

using namespace std;

int main(int argc, char** argv)
{
	auto lines = readLines("input-03.txt");
	Matrix data = parseTrees(lines);
	std::vector<std::pair<int, int>> slopes = {
		{1, 1},
		{3, 1},
		{5, 1},
		{7, 1},
		{1, 2}
	};

	__int64 prod = 1;
	for (auto slope : slopes) {
		int trees = countTrees(data, slope.first, slope.second);
		cout << slope.first << ", " << slope.second << " => " << trees << endl;
		prod *= trees;
	}
	cout << prod << endl;
}