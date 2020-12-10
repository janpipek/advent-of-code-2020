#include <functional>
#include <iterator>
#include <iostream>
#include <deque>
#include <map>
#include <numeric>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

const vector<int> getDiffs(const vector<int>& joltages) {
    vector<int> result;
    adjacent_difference(joltages.cbegin(), joltages.cend(), back_inserter(result));
    return result;
}

long long ways(int from, const vector<int>& joltages) {
    static map<int, long long> cache;
    if (cache.find(from) != cache.end()) {
        return cache.at(from);
    }
    if (!binary_search(joltages.cbegin(), joltages.cend(), from)) {
        return 0;
    }
    if (from == joltages[joltages.size() - 1]) {
        return 1;
    }

    long long sum = 0;
    for (int i = from + 1; i <= from + 3; i++) {
        sum += ways(i, joltages);
    }
    cache[from] = sum;
    return sum;
}

const vector<int> getSortedJoltages() {
    vector<int> joltages = readIntegers("input-10.txt");
    sort(joltages.begin(), joltages.end());
    joltages.insert(joltages.begin(), 0);
    joltages.push_back(joltages[joltages.size() - 1] + 3);
    return joltages;
}

auto taskA() {
    auto joltages = getSortedJoltages();
    auto diffs = getDiffs(joltages);
    int oneDiffs = count_if(diffs.cbegin(), diffs.cend(), [](int element) { return element == 1; });
    int threeDiffs = count_if(diffs.cbegin(), diffs.cend(), [](int element) { return element == 3; });
    return oneDiffs * threeDiffs;
}

auto taskB() {
    return ways(0, getSortedJoltages());
}

MAIN;