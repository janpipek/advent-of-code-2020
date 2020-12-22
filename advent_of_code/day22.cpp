#include <iostream>
#include <string>
#include <algorithm>

#include "aoc/task.hh"

using namespace std;

using Card = uint16_t;
using Deck = vector<Card>;
using Input = pair<Deck, Deck>;

Input readInput() {
    auto sections = readSections("input-22.txt");
    Input result;
    for (auto line = sections[0].cbegin() + 1; line < sections[0].cend(); line++) {
        result.first.push_back(stoi(*line));
    }
    for (auto line = sections[1].cbegin() + 1; line < sections[1].cend(); line++) {
        result.second.push_back(stoi(*line));
    }
    return result;
}

auto taskA() {
    auto input = readInput();
    NOT_YET_IMPLEMENTED
}

auto taskB() {
    NOT_YET_IMPLEMENTED
}

MAIN;