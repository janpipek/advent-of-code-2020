#include <iostream>
#include <algorithm>
#include "lib.hh"

using namespace std;

int main(int argc, char** argv) {
	vector<int> numbers = integersFromFile("input-01.txt");
	sort(numbers.begin(), numbers.end());

	for (int i: numbers) {
		for (int j : numbers) {
			auto expected = 2020 - (i + j);
			auto k_candidate = lower_bound(numbers.begin(), numbers.end(), expected);
			if (k_candidate == numbers.end()) {
				continue;
			}
			if (*k_candidate == expected) {
				cout << i << " + " << j << +" + " << expected << " = 2020" << endl;
				cout << i * j * expected << endl;
				exit(0);
			}
		}
	}
	cerr << "Not found." << endl;
	exit(-2);
}