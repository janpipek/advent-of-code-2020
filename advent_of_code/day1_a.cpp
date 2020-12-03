#include <iostream>

#include "lib.hh"

using namespace std;

int main(int argc, char** argv) {
	vector<int> numbers = readIntegers("input-01.txt");

	for (auto i : numbers) {
		for (auto j: numbers) {
			if (j + i == 2020) {
				cout << i * j << endl;
				exit(0);
			}
		}
	}
}