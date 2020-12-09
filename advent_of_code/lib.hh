#ifndef ADVENT_OF_CODE_LIB_HH
#define ADVENT_OF_CODE_LIB_HH

#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct solution_not_found : public std::exception
{
};

template <typename TaskA, typename TaskB>
void runTasks(TaskA taskA, TaskB taskB)
{
	std::cout << "Solution A: ";
	try
	{
		std::cout << taskA();
	}
	catch (solution_not_found &)
	{
		std::cout << "ERROR!";
	};
	std::cout << std::endl;
	std::cout << "Solution B: ";
	try
	{
		std::cout << taskB();
	}
	catch (solution_not_found &)
	{
		std::cout << "ERROR!";
	};
	std::cout << std::endl;
}

#define MAIN                        \
	int main(int argc, char **argv) \
	{                               \
		runTasks(taskA, taskB);     \
	}

#define SOLUTION_NOT_FOUND          \
	{                               \
		throw solution_not_found(); \
	};

std::ifstream openInputFile(const std::string &basename)
{
	char *directory = getenv("ADVENT_OF_CODE_DATA_DIR");
	if (!directory)
	{
		directory = ".";
	}

	auto dataPath = std::filesystem::path(directory) / basename;
	std::ifstream infile(dataPath);
	std::string buffer;

	if (!infile.good())
	{
		std::cerr << "File " << absolute(dataPath) << " could not be opened." << std::endl;
		exit(-1);
	}
	return infile;
}

std::vector<std::string> readLines(const std::string &basename)
{
	auto infile = openInputFile(basename);
	std::string buffer;
	std::vector<std::string> lines;
	while (getline(infile, buffer))
	{
		lines.push_back(buffer);
	}
	return lines;
}

std::vector<std::vector<std::string>> readLineGroups(const std::string &basename)
{
	const auto lines = readLines(basename);
	std::vector<std::vector<std::string>> lineGroups;
	std::vector<std::string> currentGroup;

	for (auto &line : lines)
	{
		if (!line.length())
		{
			lineGroups.push_back(currentGroup);
			currentGroup = {};
		}
		else
		{
			currentGroup.push_back(line);
		}
	}
	lineGroups.push_back(currentGroup);
	return lineGroups;
}

std::vector<int> readIntegers(const std::string &basename)
{
	auto infile = openInputFile(basename);
	std::string buffer;
	std::vector<int> numbers;

	while (getline(infile, buffer))
	{
		int current = stoi(buffer);
		numbers.push_back(current);
	}
	return numbers;
}

std::vector<long long> readLongs(const std::string &basename)
{
	auto infile = openInputFile(basename);
	std::string buffer;
	std::vector<long long> numbers;

	while (getline(infile, buffer))
	{
		long long current = stoll(buffer);
		numbers.push_back(current);
	}
	return numbers;
}

std::vector<std::string> split(const std::string &original, char delimiter = ' ', int maxSplits = -1)
{
	std::vector<std::string> result;
	std::string buffer;
	std::istringstream source(original);
	int splits = 0;

	while (getline(source, buffer, delimiter) && splits != maxSplits)
	{
		result.push_back(buffer);
		splits++;
	}
	return result;
}

std::vector<std::string> split(const std::string &original, const std::string &delimiter, int maxSplits = -1)
{
	std::vector<std::string> result;
	int splits = 0;
	size_t offset = 0;

	while (true)
	{
		if (splits == maxSplits)
		{
			break;
		}
		size_t pos = original.find(delimiter, offset);
		if (pos == std::string::npos)
		{
			break;
		}
		result.push_back(original.substr(offset, pos - offset));
		offset = pos + delimiter.length();
	}
	result.push_back(original.substr(offset));
	return result;
}
#endif