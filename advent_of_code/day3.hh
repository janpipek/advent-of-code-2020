#ifndef DAY3_HH
#define DAY3_HH

#include <vector>

class Matrix
{
public:
	Matrix(const std::vector<std::vector<bool>> &data) : _data(data), height(data.size()), width(data[0].size())
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

long countTrees(const Matrix &matrix, int right, int down)
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
#endif