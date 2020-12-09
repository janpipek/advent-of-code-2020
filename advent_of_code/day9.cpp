#include <functional>
#include <iostream>
#include <deque>
#include <numeric>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

const size_t MAX_SIZE = 25;

bool addsTo(long long newNumber, const deque<long long>& previousNumbers) {
    for (long long i : previousNumbers) {
        for (long long j : previousNumbers) {
            if (i == j) {
                continue;
            }
            if (i + j == newNumber) {
                return true;
            }
        }
    }
    return false;
}

std::vector<long long> findContiguous(long long requiredSum, const vector<long long>& numbers) {
    // initialize first iteration
    size_t start = 0;
    size_t current = 0;
    std::vector<long long> contiguous = {};
    long long sum = 0;

    while (true)
    {
        auto currentNumber = numbers[current];
        sum += currentNumber;
        contiguous.push_back(currentNumber);

        if (sum > requiredSum || currentNumber == requiredSum) {
            // reset the loop
            start++;
            if (start == numbers.size()) {
                break;
            }
            current = start;
            contiguous = { };
            sum = 0;
        }
        else if (sum == requiredSum) {
            return contiguous;
        }
        else {
            current++;
        }
    }
    return {};
}

auto taskA() {
    auto numbers = readLongs("input-09.txt");
    deque<long long> numberQueue;
    long long result = 0;

    for (auto number : numbers) {
        if (numberQueue.size() == MAX_SIZE) {
            if (!addsTo(number, numberQueue)) {
                return number;
            }
            numberQueue.pop_front();
        }
        numberQueue.push_back(number);
    }
    SOLUTION_NOT_FOUND
}

auto taskB() {
    auto numbers = readLongs("input-09.txt");
    deque<long long> numberQueue;
    long long result = 0LL;

    for (auto number : numbers) {
        if (numberQueue.size() == MAX_SIZE) {
            if (!addsTo(number, numberQueue)) {
                auto contiguous = findContiguous(number, numbers);
                if (!contiguous.size()) {
                    SOLUTION_NOT_FOUND
                }
                auto solution = minmax_element(contiguous.cbegin(), contiguous.cend());
                return *(solution.first) + *(solution.second);
            }
            numberQueue.pop_front();
        }
        numberQueue.push_back(number);
    }
    SOLUTION_NOT_FOUND
}

MAIN;