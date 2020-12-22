#ifndef ADVENT_OF_CODE_TASK_HH
#define ADVENT_OF_CODE_TASK_HH

#include <chrono>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "debug.hh"

template <typename Task> void runTask(const std::string& name, Task task)
{
	auto start = std::chrono::steady_clock::now();
	try
	{
		auto result = task();
		auto stop = std::chrono::steady_clock::now();
		std::cout << "Task " << name << ": ";
		std::cout << result << std::endl;
		std::cout << "(took " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << " us)" << std::endl;
	}
	catch (std::exception& exc)
	{
		std::cout << "Task " << name << ": ";
		std::cout << exc.what() << std::endl;
	};

	std::cout << std::endl;
}

template <typename TaskA, typename TaskB>
void runTasks(TaskA taskA, TaskB taskB)
{
	runTask("A", taskA);
	runTask("B", taskB);
}

#define MAIN                        \
	int main(int argc, char **argv) \
	{                               \
		runTasks(taskA, taskB);     \
	}

#define SOLUTION_NOT_FOUND                     \
	{                                          \
		throw std::runtime_error("NOT FOUND");     \
	};

#define NOT_YET_IMPLEMENTED                          \
	{                                                \
		throw std::runtime_error("NOT IMPLEMENTED"); return -1;    \
	};

std::ifstream openInputFile(const std::string &basename)
{
#ifdef _MSC_VER
    size_t size;
    char directory[512];
    _dupenv_s((char **) &directory, &size, "ADVENT_OF_CODE_DATA_DIR");
#else
    const char* directory = getenv("ADVENT_OF_CODE_DATA_DIR");
    size_t size = strlen(directory);
#endif
    std::filesystem::path dataPath;
    if (size) {
        dataPath = std::filesystem::path(std::string(directory)) / basename;
    } else {
        dataPath = std::filesystem::path(basename);
    }
    std::ifstream infile(dataPath);
    std::string buffer;

    if (!infile.good()) {
        std::cerr << "File " << std::filesystem::absolute(dataPath) << " could not be opened." << std::endl;
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

std::vector<std::vector<std::string>> readSections(const std::string &basename) {
    auto lines = readLines(basename);
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> currentSection;
    for (const auto& line: lines) {
        if (line.empty()) {
            result.push_back(currentSection);
            currentSection = {};
        }
        else {
            currentSection.push_back(line);
        }
    }
    result.push_back(currentSection);
    return result;
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