#include <algorithm>
#include <map>

#include "lib.hh"

using namespace std;

auto taskA() {
    const vector<int> input = {20, 0, 1, 11, 6, 3};
    vector<int> history = input;
    for (int turn = input.size(); turn < 2020; turn++) {
        int currentNumber = *history.rbegin();
        auto it = find(history.rbegin() + 1, history.rend(), currentNumber);
        if (it == history.rend()) {
            history.push_back(0);
        } else {
            history.push_back(it - history.rbegin());
        }
    }
    return *history.rbegin();  // 421
}

auto taskB() {
    // Takes some time, hash map would be a more appropriate data structure for history
    const vector<int> input = {20, 0, 1, 11, 6, 3};
    map<int, int> history;
    for (int i = 0; i < input.size() - 1; i++) {
        history[input[i]] = i;
    }
    int prevNumber = *input.rbegin();
    for (int turn = input.size(); turn < 30000000; turn++) {
        int nextNumber = 0;
        if (history.find(prevNumber) != history.end()) {
            nextNumber = turn - history.at(prevNumber) - 1;
        }
        history[prevNumber] = turn - 1;
        prevNumber = nextNumber;
    }
    return prevNumber;
}

MAIN;