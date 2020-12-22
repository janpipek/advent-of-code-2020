#include <iostream>
#include <string>

#include "aoc/task.hh"

using namespace std;

struct Food {
    vector<string> ingredients;
    vector<string> allergens;
};

using Input = vector<Food>;

Input readInput() {
    Input result;
    auto lines = readLines("input-21-test.txt");
    for (const auto &line : lines) {
        auto ingredients_allergens = split(line, " (contains ");
        result.push_back({
                                 split(ingredients_allergens[0]),
                                 split(ingredients_allergens[1].substr(0, ingredients_allergens[1].size() - 1), ", ")
                         });
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