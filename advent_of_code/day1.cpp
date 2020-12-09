#include <iostream>

#include "lib.hh"

using namespace std;

auto taskA() {
	vector<int> numbers = readIntegers("input-01.txt");
	for (auto i : numbers) {
		for (auto j: numbers) {
			if (j + i == 2020) {
				return i * j;
			}
		}
	}
	SOLUTION_NOT_FOUND;
}

auto taskB() {
	vector<int> numbers = readIntegers("input-01.txt");
	sort(numbers.begin(), numbers.end());

	for (int i: numbers) {
		for (int j : numbers) {
			auto expected = 2020 - (i + j);
			auto k_candidate = lower_bound(numbers.begin(), numbers.end(), expected);
			if (k_candidate == numbers.end()) {
				continue;
			}
			if (*k_candidate == expected) {
				return i * j * expected;
			}
		}
	}
	SOLUTION_NOT_FOUND;
}

MAIN;