#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

// const size_t MASK_LENGTH = 36;

using Memory = array<unsigned long long, 65536>;
using Mask = string;

struct ProgramState {
    Memory memory{};
    Mask mask;
};

long long applyMask(Mask mask, long long number) {
    long long result = 0;
    for (size_t i = 0; i < mask.size(); i++) {
        auto maskBit = mask[mask.size() - i - 1];
        if (maskBit == 'X') {
            result += number & (1ULL << i);
        }
        else if (maskBit == '1') {
            result += (1ULL << i);
        }
    }
    return result;
}

auto taskA() {
    auto lines = readLines("input-14.txt");
    ProgramState state;
    for (auto line : lines) {
        if (line.substr(0, 3) == "mem") {
            auto address_value = split(line.substr(4), "] = ");
            size_t address = stoul(address_value[0]);
            long long value = stoll(address_value[1]);
            long long maskedValue = applyMask(state.mask, value);
            state.memory[address] = maskedValue;
        }
        else if (line.substr(0, 4) == "mask") {
            state.mask = line.substr(7);
        }
    }
    return accumulate(
        state.memory.cbegin(),
        state.memory.cend(),
        0ULL,
        plus<unsigned long long>()
    );
}

auto taskB() {
	NOT_YET_IMPLEMENTED
}

MAIN;