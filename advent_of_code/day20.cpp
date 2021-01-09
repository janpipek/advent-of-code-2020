#include <iostream>
#include <string>
#include <algorithm>

#include "aoc/task.hh"
#include "aoc/ndarray.hh"
#include "aoc/debug.hh"

using namespace std;

enum side_t : int {
    TOP = 1,
    RIGHT = 2,
    BOTTOM = 3,
    LEFT = 4,
};

struct Tile {
    int id{};
    NDArray<int> data;

    int size() const {
        return data.shape()[0];
    }

    side_t findMatch(const Tile& other) {
        for (int i = 1; i < 5; i++) {
            if (getEdge(i) == other.getEdge(5 - i));
        }
        return 0;
    }

    vector<int> getEdge(side_t side) const {
        vector<int> result;
        int i;
        if (side == TOP) {
            for (i = 0; i < size(); i++) {
                result.push_back(data.at({0, i}));
            }
        }
        else if (side == RIGHT) {
            for (i = 0; i < size(); i++) {
                result.push_back(data.at({i, size() - 1}));
            }
        }
        else if (side == BOTTOM) {
            for (i = 0; i < size(); i++) {
                result.push_back(data.at({size() - 1, i}));
            }
        }
        else if (side == LEFT) {
            for (i = 0; i < size(); i++) {
                result.push_back(data.at({i, 0}));
            }
        }
        return result;
    }

    vector<vector<int>> getEdges() const {
        vector<vector<int>> result;
        result.push_back(getEdge(TOP));
        result.push_back(getEdge(RIGHT));
        result.push_back(getEdge(BOTTOM));
        result.push_back(getEdge(LEFT));
        return result;
    }

    vector<vector<int>> getFlippedEdges() const {
        auto result = getEdges();
        for (auto &edge : result) {
            reverse(edge.begin(), edge.end());
        }
        return result;
    }

    NDArray<int> stripEdges() const {
        NDArray<int> result({data.shape()[0] - 2, data.shape()[1] - 2});
        for (int i = 1; i < data.shape()[0] - 1; i++) {
            for (int j = 1; j < data.shape()[1] - 1; j++) {
                result.at({i-1, j-1}) = data.at({i, j});
            }
        }
        return result;
    }

    /** Turn 90 degrees to the right **/
    Tile rotateRight() const {
        NDArray<int> newData = data.swapAxes(0, 1).flip(1);
        return {id, newData};
    }

    Tile transform(size_t rotations, bool flip) const {
        Tile result = *this;
        for (size_t i = 0; i < rotations; i++) {
            result = result.rotateRight();
        }
        if (flip) {
            result.data = result.data.flip(0);
        }
        return result;
    }

};

Tile mergeTiles(const vector<vector<Tile>>& tiles) {
    auto tileSize = tiles[0][0].data.shape();
    vector<int> newTileSize = {tileSize[0] - 2, tileSize[1] - 2};

    size_t size0 = tiles.size() * newTileSize[0];
    size_t size1 = tiles[0].size() * newTileSize[1];

    NDArray<int> result({size1, size1});

    int i = 0;
    for (const auto& row: tiles) {
        int j = 0;
        for (const auto& tile: row) {
            auto stripped = tile.stripEdges();
            for (int k = 0; k < newTileSize[0]; k++) {
                for (int l = 0; l < newTileSize[1]; l++) {
                    index_t index = {
                        i * newTileSize[0] + k,
                        j * newTileSize[1] + l
                    };
                    int value = stripped.at({k, l});
                    result.set(index, value);
                }
            }
            j++;
        }
        i++;
    }



    // TODO: Implement
    return {0, result};
}

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

vector<long long> findCorners(const Input& input) {
    vector<long long> result;

    vector<vector<vector<int>>> edges;
    vector<vector<vector<int>>> flipedEdges;
    transform(input.cbegin(), input.cend(), back_inserter(edges), [](auto &tile) { return tile.getEdges(); });
    transform(input.cbegin(), input.cend(), back_inserter(flipedEdges),
              [](auto &tile) { return tile.getFlippedEdges(); });

    for (int i = 0; i < input.size(); i++) {
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
                        sameFound++;
                    }
                }
                auto flippedOther = flipedEdges[j];
                for (const auto &otherEdge : flippedOther) {
                    if (otherEdge == edge) {
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
            result.push_back(input[i].id);
        }
    }   
    return result;
}

auto taskA() {
    auto input = readInput();
    auto corners = findCorners(input);
    return product(corners);
}

auto taskB() {
    auto input = readInput();
    auto corners = findCorners(input);

    // 1) Find first corner tile
    // 2) Find its rotation
    // 3) Match the tiles incrementally
    // 4) Merge the tiles into one tile
    // 5) Rotate and match the monster
    NOT_YET_IMPLEMENTED
}

MAIN;