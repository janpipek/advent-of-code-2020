#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

class Matrix
{
public:
	Matrix(const std::vector<std::vector<bool>>& data) : _data(data), height(data.size()), width(data[0].size())
	{
		// We do not check that all rows are of equal length
	}

	bool isTree(int row, int col) const
	{
		if (row >= height)
		{
			throw "Invalid position";
		}
		return _data[row][col % width];
	}

	const size_t height;
	const size_t width;

private:
	std::vector<std::vector<bool>> _data;
};

long countTrees(const Matrix& matrix, int right, int down)
{
	int col = 0;
	int row = 0;
	int trees = 0;

	while (row < matrix.height)
	{
		if (matrix.isTree(row, col))
		{
			trees++;
		}
		col += right;
		row += down;
	}
	return trees;
}

Matrix parseTrees(const std::vector<std::string> lines)
{
	std::vector<std::vector<bool>> data;
	for (const auto& line : lines)
	{
		std::vector<bool> rowData;
		for (auto symbol : line)
		{
			rowData.push_back(symbol == '#');
		}
		data.push_back(rowData);
	}
	return Matrix(data);
}

auto taskA() {
	auto lines = readLines("input-03.txt");
	Matrix data = parseTrees(lines);
	int trees = countTrees(data, 3, 1);
	return trees;
}

auto taskB() {
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
		// cout << slope.first << ", " << slope.second << " => " << trees << endl;
		prod *= trees;
	}
	return prod;
}

MAIN;