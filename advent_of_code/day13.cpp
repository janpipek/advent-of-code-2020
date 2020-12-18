#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

#include "aoc/task.hh"

using namespace std;

const unsigned long long OUT_OF_SERVICE = std::numeric_limits<unsigned long long>::max();
const unsigned long long x = OUT_OF_SERVICE;
const unsigned long long INFINITE_TIME = std::numeric_limits<unsigned long long>::max();

struct Input {
    unsigned long long timestamp;
    vector<unsigned long long> busNumbers;
};

vector<unsigned long long> parseBusNumbers(const string& line) {
    vector<unsigned long long> result;
    for (const auto& bus : split(line, ',')) {
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
        stoull(lines[0]),
        parseBusNumbers(lines[1])
    };
}

inline unsigned long long waitTime(unsigned long long time, unsigned long long busNumber) {
    if (busNumber == OUT_OF_SERVICE) {
        return INFINITE_TIME;
    }
    return busNumber - (time % busNumber);
}

auto taskA() {
    auto input = readInput();
    unsigned long long bus = *min_element(
        input.busNumbers.cbegin(),
        input.busNumbers.cend(),
        [input](auto bus1, auto bus2) {
            return waitTime(input.timestamp, bus1) < waitTime(input.timestamp, bus2); 
        }
    );
    return bus * waitTime(input.timestamp, bus);
}

struct Pattern {
    unsigned long long offset {0};
    unsigned long long factor {1};
};

void updatePattern(Pattern& pattern, unsigned long long bus, unsigned long long order) {
    if (bus == OUT_OF_SERVICE) {
        return;
    }
    unsigned long long modulo = (bus > order) ? (bus - order) % bus : (bus - order % bus);
    while (pattern.offset % bus != modulo) {
        pattern.offset += pattern.factor;
    }
    pattern.factor *= bus;
}

auto taskB() {
    // Note: All are primes
    auto input = readInput();
    Pattern pattern {};

    for (size_t i = 0; i < input.busNumbers.size(); i++) {
        updatePattern(pattern, input.busNumbers[i], (unsigned long long)i);
    }
    return pattern.offset;
}

MAIN;