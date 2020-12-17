#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"
#include "ndarray.hh"

using namespace std;

NDArray<short> readInput(int turns) {
    auto lines = readLines("input-17.txt");
    NDArray<short> result(
            {lines.size() + 2 * turns, lines[0].size() + 2 * turns, (size_t)1 + 2 * turns},
            {-turns, -turns, -turns});
    for (size_t i = 0; i < lines.size(); i++) {
        auto line = lines[i];
        for (size_t j = 0; j < line.size(); j++) {
            switch(line[j]) {
                case '#':
                    result.set({i, j, 0}, 1);
                    break;
                case '.':
                    result.set({i, j, 0}, 0);
                    break;
                default:
                    throw runtime_error("Invalid token.");
            }
        }
    }
    return result;
}

NDArray<short> getNeighbourCounts(const NDArray<short>& plan) {
    NDArray<short> result(plan.shape(), plan.offsets());
    for (int i = plan.offsets()[0]; i < plan.offsets()[0] + plan.shape()[0]; i++) {
        for (int j = plan.offsets()[1]; j < plan.offsets()[1] + plan.shape()[1]; j++) {
            for (int k = plan.offsets()[2]; k < plan.offsets()[2] + plan.shape()[2]; k++) {
                if (plan.at({i, j, k})) {
                    for (int l = i - 1; l <= i + 1; l++) {
                        for (int m = j - 1; m <= j + 1; m++) {
                            for (int n = k - 1; n <= k + 1; n++) {
                                if ((l == i) && (m == j) && (n == k)) {
                                    continue;
                                }
                                if (plan.isValidIndex({l, m, n})) {
                                    result.at({l, m, n}) += 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

auto taskA() {
    int turns = 6;
    NDArray<short> plan = readInput(turns);
    for (int turn = 0; turn < turns; turn++) {
        auto neighbours = getNeighbourCounts(plan);
        for (int i = plan.offsets()[0]; i < plan.offsets()[0] + plan.shape()[0]; i++) {
            for (int j = plan.offsets()[1]; j < plan.offsets()[1] + plan.shape()[1]; j++) {
                for (int k = plan.offsets()[2]; k < plan.offsets()[2] + plan.shape()[2]; k++) {
                    vector<int64> index = {i, j, k};
                    auto n = neighbours.at(index);
                    if (plan.at(index) && ((n < 2) || (n > 3))) {
                        plan.at(index) = 0;
                    }
                    else if (!plan.at(index) && (n == 3)) {
                        plan.at(index) = 1;
                    }
                }
            }
        }
    }
    return plan.sum();
}

auto taskB() {
    NOT_YET_IMPLEMENTED
}

MAIN;
