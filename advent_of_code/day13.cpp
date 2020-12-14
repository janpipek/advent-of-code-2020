#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

const int OUT_OF_SERVICE = -1;
const int INFINITE_TIME = std::numeric_limits<int>::max();

struct Input {
    int timestamp;
    vector<int> busNumbers;
};

vector<int> parseBusNumbers(const string& line) {
    vector<int> result;
    for (auto bus : split(line, ',')) {
        if (bus == "x") {
            result.push_back(OUT_OF_SERVICE);
        } else {
            result.push_back(stoi(bus));
        }
    }
    return result;
}

Input readInput() {
    auto lines = readLines("input-13.txt");
    return {
        stoi(lines[0]),
        parseBusNumbers(lines[1])
    };
}

inline int waitTime(int time, int busNumber) {
    if (busNumber == OUT_OF_SERVICE) {
        return INFINITE_TIME;
    }
    return busNumber - (time % busNumber);
}

auto taskA() {
    auto input = readInput();
    int bus = *min_element(
        input.busNumbers.cbegin(),
        input.busNumbers.cend(),
        [input](auto bus1, auto bus2) {
            return waitTime(input.timestamp, bus1) < waitTime(input.timestamp, bus2); 
        }
    );
    return bus * waitTime(input.timestamp, bus);
}

auto taskB() {
    auto input = readInput();
    std::vector<long long> busNumber = asType<int, long long>(input.busNumbers);
    NOT_YET_IMPLEMENTED
}

MAIN;