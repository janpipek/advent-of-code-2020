#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

using Memory = map<unsigned long long, unsigned long long>;
using Mask = string;

struct ProgramState {
    Memory memory{};
    Mask mask;
};

unsigned long long sumMemory(const Memory& memory) {
    return accumulate(
        memory.cbegin(),
        memory.cend(),
        0ULL,
        [](auto left, auto& right) {
            return left + right.second;
        }
    );
}

unsigned long long applyMask(Mask mask, long long number) {
    unsigned long long result = 0;
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
    return sumMemory(state.memory);
}

vector<unsigned long long> decodeAddresses(unsigned long long address, const Mask& mask) {
    vector<unsigned long long> result { address };
    for (size_t i = 0; i < mask.size(); i++) {
        auto maskBit = mask[mask.size() - i - 1];
        if (maskBit == 'X') {
            size_t m = result.size();
            for (size_t j = 0; j < m; j++) {
                auto& current = result[j];
                result.push_back(current ^ (1ULL << i));
            }            
        }
        else if (maskBit == '1') {
            size_t m = result.size();
            for (size_t j = 0; j < m; j++) {
                result[j] |= (1ULL << i);
            }
        }
    }
    return result;    
}

auto taskB() {
    auto lines = readLines("input-14.txt");
    ProgramState state;
    for (auto line : lines) {
        if (line.substr(0, 3) == "mem") {
            auto address_value = split(line.substr(4), "] = ");
            unsigned long long address = stoull(address_value[0]);
            unsigned long long value = stoll(address_value[1]);
            for (unsigned long long address : decodeAddresses(address, state.mask)) {
                state.memory[address] = value;
            }
        }
        else if (line.substr(0, 4) == "mask") {
            state.mask = line.substr(7);
        }
    }
    return sumMemory(state.memory);
}

MAIN;