#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>

#include "lib.hh"

using namespace std;
using namespace std::filesystem;

int main(int argc, char** argv) {
	ifstream infile = openInputFile("input-01.txt");
	string buffer;

	vector<int> numbers;

	while (getline(infile, buffer)) {
		int current = stoi(buffer);
		numbers.push_back(current);

		for (auto i : numbers) {
			if (i + current == 2020) {
				cout << i * current << endl;
			}
		}
	}
}