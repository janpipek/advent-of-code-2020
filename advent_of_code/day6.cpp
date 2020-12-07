#include <iostream>
#include <set>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

void taskA() {
    auto lineGroups = readLineGroups("input-06.txt");

    size_t total = 0;

    for (const auto& group : lineGroups) {
        set<char> characters;
        for (const auto& line: group) {
            for (char character : line) {
                characters.insert(character);
            }
        }
        total += characters.size();
    }
    cout << "Total A: " << total << endl;
}

void taskB() {
    auto lineGroups = readLineGroups("input-06.txt");

    size_t total = 0;

    for (const auto& group : lineGroups) {
        if (group.size() < 1) {
            continue;
        }
        string groupIntersect = group[0];

        for (auto it = group.cbegin() + 1; it != group.cend(); it++) {
            string newResult = "";
            for (auto c : *it) {
                auto pos = groupIntersect.find(c);
                if (pos != string::npos) {
                    newResult.push_back(c);
                }
            }
            groupIntersect = newResult;
        }
        total += groupIntersect.size();
    }
    cout << "Total B: " << total << endl;
}

int main(int argc, char** argv)
{
    taskA();
    taskB();
}