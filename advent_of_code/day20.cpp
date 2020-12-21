#include <iostream>
#include <string>
#include <algorithm>

#include "aoc/task.hh"
#include "aoc/ndarray.hh"
#include "aoc/debug.hh"

using namespace std;

struct Tile {
    int id{};
    NDArray<int> data;

    int size() const {
        return data.shape()[0];
    }

    vector<vector<int>> getEdges() const {
        vector<vector<int>> result;
        int i;
        vector<int> top;
        for (i = 0; i < size(); i++) {
            top.push_back(data.at({0, i}));
        }
        vector<int> right;
        for (i = 0; i < size(); i++) {
            right.push_back(data.at({i, size() - 1}));
        }
        vector<int> bottom;
        for (i = 0; i < size(); i++) {
            bottom.push_back(data.at({size() - 1, i}));
        }
        vector<int> left;
        for (i = 0; i < size(); i++) {
            left.push_back(data.at({i, 0}));
        }

        result.push_back(top);
        result.push_back(right);
        result.push_back(bottom);
        result.push_back(left);

        return result;
    }

    vector<vector<int>> getFlippedEdges() const {
        auto result = getEdges();
        for (auto &edge : result) {
            reverse(edge.begin(), edge.end());
        }
        return result;
    }
};

using Input = vector<Tile>;

Input readInput() {
    Input result;
    auto sections = readSections("input-20.txt");
    for (const auto &section: sections) {
        auto header = section[0];
        Tile tile{
                stoi(header.substr(5, header.size() - 6)),
                NDArray<int>({section.size() - 1, section[1].size()})
        };
        for (int i = 1; i < section.size(); i++) {
            auto line = section[i];
            for (int j = 0; j < line.size(); j++) {
                tile.data.at({i - 1, j}) = (line[j] == '#') ? 1 : 0;
            }
        }
        result.push_back(tile);
    }
    return result;
}

auto taskA() {
    auto input = readInput();
    vector<vector<vector<int>>> edges;
    vector<vector<vector<int>>> flipedEdges;
    transform(input.cbegin(), input.cend(), back_inserter(edges), [](auto &tile) { return tile.getEdges(); });
    transform(input.cbegin(), input.cend(), back_inserter(flipedEdges),
              [](auto &tile) { return tile.getFlippedEdges(); });

    int64_t result = 1;
    for (int i = 0; i < input.size(); i++) {
        // cout << input[i].id << endl;
        int uniqueEdges = 0;
        for (const auto &edge : edges[i]) {
            int sameFound = 0;
            for (int j = 0; j < input.size(); j++) {
                if (i == j) {
                    continue;
                }
                auto other = edges[j];
                for (const auto &otherEdge : other) {
                    if (otherEdge == edge) {
                        // cout << edge << endl;
                        // cout << otherEdge << endl;
                        sameFound++;
                    }
                }
                auto flippedOther = flipedEdges[j];
                for (const auto &otherEdge : flippedOther) {
                    if (otherEdge == edge) {
                        // cout << edge << endl;
                        // cout << otherEdge << endl;
                        sameFound++;
                    }
                }
            }
            if (sameFound > 1) {
                SOLUTION_NOT_FOUND;
            }
            if (!sameFound) {
                uniqueEdges++;
            }
        }
        if (uniqueEdges == 2) {
            result *= input[i].id;
        }
    }
    return result;
}

auto taskB() {
    NOT_YET_IMPLEMENTED
}

MAIN;