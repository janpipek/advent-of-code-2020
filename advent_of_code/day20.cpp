#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>

#include "aoc/task.hh"
#include "aoc/ndarray.hh"
#include "aoc/debug.hh"

using namespace std;

enum side_t : int {
    NONE = 0,

    TOP = 1,
    RIGHT = 2,
    BOTTOM = 3,
    LEFT = 4,
};

vector<string> MONSTER_SOURCE = {
    "                  # ",
    "#    ##    ##    ###",
    " #  #  #  #  #  #   ",
};

side_t opposite(side_t side) {
    switch (side)
    {
    case TOP:
        return BOTTOM;
    case RIGHT:
        return LEFT;
    case BOTTOM:
        return TOP;
    case LEFT:
        return RIGHT;
    default:
        return NONE;
    }
}

struct Tile {
    int id{};
    NDArray<int> data;

    int size() const {
        return data.shape()[0];
    }

    side_t findMatch(const Tile& other) const {
        for (int i = 1; i < 5; i++) {
            side_t side = (side_t)i;
            side_t opp = opposite(side);
            if (getEdge(side) == other.getEdge(opp)) {
                return side;
            }
        }
        return NONE;
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

    /** Turn 90 degrees to the right **/
    Tile rotateLeft() const {
        // TODO: Could be more efficient
        return transform(3, false);
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
    if (!tiles.size() || !tiles[0].size()) {
        cerr << "No tiles" << endl;
        SOLUTION_NOT_FOUND;
    }

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

vector<Tile> findCorners(const Input& input) {
    vector<Tile> result;

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
            result.push_back(input[i]);
        }
    }   
    return result;
}

auto taskA() {
    auto input = readInput();
    auto corners = findCorners(input);

    vector<long long> ids;
    transform(corners.cbegin(), corners.cend(), back_inserter(ids), [](const Tile& tile) { return tile.id; });

    return product(ids);
}

Tile findMatching(const Tile& tile1, const Input& input, side_t side) {
    for (const auto& tile2: input) {
        if (tile1.id == tile2.id) {
            continue;
        }
        for (int rotation = 0; rotation < 4; rotation++) {
            bool flips[] = {false, true};
            for (auto flip : flips) {        
                auto transformed = tile2.transform(rotation, flip);
                if (tile1.findMatch(transformed) == side) {
                    // cout << "Matched " << tile1.id << " and " << tile2.id << endl;
                    return transformed;
                }
            }
        }
    }
    cerr << "Matching tile for " << tile1.id << " not found." << endl;
    SOLUTION_NOT_FOUND;
}

// Output for debugging purposes
ostream& operator<<(ostream& stream, const Tile& tile) {
    cout << "Tile " << tile.id << "(" << tile.data.shape()[0] << "x" << tile.data.shape()[1] << ")" << endl;
    cout << "{" << endl;
    for (int i = 0; i < tile.data.shape()[0]; i++) {
        cout << "  {";
        for (int j = 0; j < tile.data.shape()[1]; j++) {
            cout << tile.data.at({i, j});
            if (j <tile.data.shape()[1] - 1) {
                cout << " ";
            }
        }
        cout << "}" << endl;
    }
    cout << "}" << endl;
}

Tile alignTopLeftCorner(const Tile& tile, const Input& input) {
    Tile result = tile;
    // Rotate the first one
    size_t matching = 0;

    for (const auto& tile: input) {
        if (tile.id == result.id) {
            continue;
        }
        for (int rotation = 0; rotation < 4; rotation++) {
            bool flips[] = {false, true};
            for (auto flip : flips) {
                auto transformed = tile.transform(rotation, flip);
                side_t match = result.findMatch(transformed);
                if (match != NONE) {
                    matching++;
                    if (match == LEFT) {
                        result = result.rotateLeft();
                    }
                    if (match == TOP) {
                        result = result.rotateRight();
                    }
                }
            }
        }
    }

    if (matching != 2) {
        cout << matching << endl;
        SOLUTION_NOT_FOUND;
    }
    return result;
}

Tile assembleBoard(const Tile& topLeft, const Input& input) {
    size_t boardLength = (size_t)sqrt(input.size());
    vector<vector<Tile>> tiles;
    for (int i = 0; i < boardLength; i++) {
        vector<Tile> row;
        // Initial seed
        if (i == 0) {
            row.push_back(topLeft); 
        } else {
            row.push_back(findMatching(tiles[i-1][0], input, BOTTOM));
        }
        tiles.push_back(row);
        vector<Tile>& rowRef = tiles[i];

        for (int j = 1; j < boardLength; j++)  {
            rowRef.push_back(findMatching(tiles[i][j-1], input, RIGHT));
        }
    }
    
    return mergeTiles(tiles);
}

vector<vector<int>> readMonster() {
    vector<vector<int>> result;
    for (auto rowSource: MONSTER_SOURCE) {
        vector<int> row;
        for (auto c: rowSource) {
            row.push_back(c == '#' ? 1: 0);
        }
        result.push_back(row);
    }
    return result;
}

auto taskB() {
    auto input = readInput();
    vector<Tile> corners = findCorners(input);
    Tile topLeft = alignTopLeftCorner(corners[0], input);
    Tile board = assembleBoard(topLeft, input);


    cout << board << endl;

    // 5) Rotate and match the monster
    NOT_YET_IMPLEMENTED
}

MAIN;