#include <iostream>
#include <algorithm>

#include "aoc/task.hh"
#include "aoc/ndarray.hh"

using namespace std;

NDArray<short> readInput(int turns, size_t ndim = 3) {
    auto lines = readLines("input-17.txt");

    vector<size_t> shape(ndim, (size_t) 1 + 2 * turns);
    shape[0] = lines.size() + 2 * turns;
    shape[1] = lines[0].size() + 2 * turns;
    index_t offsets(ndim, -turns);

    NDArray<short> result(shape, offsets);
    for (size_t i = 0; i < lines.size(); i++) {
        auto line = lines[i];
        for (size_t j = 0; j < line.size(); j++) {
            index_t index(ndim, 0);
            index[0] = i;
            index[1] = j;
            switch(line[j]) {
                case '#':
                    result.set(index, 1);
                    break;
                case '.':
                    result.set(index, 0);
                    break;
                default:
                    throw runtime_error("Invalid token.");
            }
        }
    }
    return result;
}

void calculateNeighboursCount(const NDArray<short>& plan, NDArray<short>& target, const index_t& sourceIndex = {}, const index_t& targetIndex = {}) {
    if (targetIndex.size() == plan.ndim()) {
        if (!target.isValidIndex(targetIndex)) {
            return;
        }
        if (targetIndex == sourceIndex) {
            return;
        }
        if (plan.at(sourceIndex)) {
            target.at(targetIndex) += 1;
        }
    } else if (sourceIndex.size() == plan.ndim()) {
        size_t currentDim = targetIndex.size();
        index_t newIndex = targetIndex;
        newIndex.push_back(sourceIndex[currentDim] - 1);
        for (int _ = 0; _ <= 2; _++) {
            calculateNeighboursCount(plan, target, sourceIndex, newIndex);
            newIndex[currentDim]++;
        }
    }
    else {
        size_t currentDim = sourceIndex.size();
        index_t newIndex = sourceIndex;
        newIndex.push_back(plan.offsets()[currentDim]);
        for (int _ = 0; _ < plan.shape()[currentDim]; _++) {
            calculateNeighboursCount(plan, target, newIndex);
            newIndex[currentDim]++;
        }
    }
}

void updatePlan(NDArray<short>& plan, const NDArray<short>& neighbours, const index_t& index = {}) {
    if (index.size() == plan.ndim()) {
        auto n = neighbours.at(index);
        if (plan.at(index) && ((n < 2) || (n > 3))) {
            plan.at(index) = 0;
        }
        else if (!plan.at(index) && (n == 3)) {
            plan.at(index) = 1;
        }
    } else {
        size_t currentDim = index.size();
        for (int i = plan.offsets()[currentDim]; i < plan.offsets()[currentDim] + plan.shape()[currentDim]; i++) {
            index_t newIndex = index;
            newIndex.push_back(i);
            updatePlan(plan, neighbours, newIndex);
        }
    }
}

auto taskA() {
    int turns = 6;
    NDArray<short> plan = readInput(turns);
    for (int turn = 0; turn < turns; turn++) {
        NDArray<short> neighbours(plan.shape(), plan.offsets());
        calculateNeighboursCount(plan, neighbours);
        updatePlan(plan, neighbours);
    }
    return plan.sum();
}

auto taskB() {
    int turns = 6;
    NDArray<short> plan = readInput(turns, 4);
    for (int turn = 0; turn < turns; turn++) {
        NDArray<short> neighbours(plan.shape(), plan.offsets());
        calculateNeighboursCount(plan, neighbours);
        updatePlan(plan, neighbours);
    }
    return plan.sum();
}

MAIN;
