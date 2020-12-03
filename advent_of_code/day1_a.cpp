#include <iostream>
#include <fstream>

#include "lib.hh"

using namespace std;
using namespace std::filesystem;

int main(int argc, char** argv) {
	vector<int> numbers = integersFromFile("input-01.txt");

	for (auto i : numbers) {
		for (auto j: numbers) {
			if (j + i == 2020) {
				cout << i * j << endl;
				exit(0);
			}
		}
	}
}