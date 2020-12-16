#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

enum class SeatState : char {
    empty = 'L',
    occupied = '#',
    unavailable = '.'
};

using Row = vector<SeatState>;
using Matrix = vector<Row>;

size_t countOccupied(const Matrix &seatPlan) {
    size_t count = 0;
    for (const auto &row : seatPlan) {
        for (auto seat : row) {
            if (seat == SeatState::occupied) {
                count++;
            }
        }
    }
    return count;
}

size_t countAdjacent(const Matrix &seatPlan, size_t row, size_t column) {
    size_t count = 0;

    for (int i = max(0, ((int) row) - 1); i <= min(row + 1, seatPlan.size() - 1); i++) {
        for (int j = max(0, ((int) column) - 1); j <= min(column + 1, seatPlan[i].size() - 1); j++) {
            if ((i == row) && (j == column)) { // The element itself
                continue;
            }
            if (seatPlan[i][j] == SeatState::occupied) {
                count++;
            }
        }
    }
    return count;
}

size_t countInDirection(const Matrix &seatPlan, size_t row, size_t column) {
    const static vector<pair<int, int>> directions = {{-1, -1},
                                                      {-1, 0},
                                                      {-1, 1},
                                                      {0,  -1},
                                                      {0,  1},
                                                      {1,  -1},
                                                      {1,  0},
                                                      {1,  1}};
    size_t count = 0;

    for (auto direction : directions) {
        int i = row;
        int j = column;

        i += direction.first;
        j += direction.second;

        while (
                (i >= 0) && (j >= 0) &&
                (i < seatPlan.size()) &&
                (j < seatPlan[i].size())) {
            auto seat = seatPlan[i][j];
            if (seat == SeatState::unavailable) {
                i += direction.first;
                j += direction.second;
                continue;
            }
            count += (seat == SeatState::occupied);
            break;
        }
    }
    return count;
}

Matrix updateSeats(const Matrix &previousPlan, decltype(countAdjacent) countHandler = countAdjacent,
                   size_t occupiedLimit = 4) {
    Matrix nextPlan;
    for (size_t i = 0; i < previousPlan.size(); i++) {
        const auto &previousRow = previousPlan[i];
        Row newRow;
        for (size_t j = 0; j < previousRow.size(); j++) {
            SeatState previousState = previousRow[j];
            SeatState newState = previousState;
            auto adjacent = countHandler(previousPlan, i, j);

            switch (previousState) {
                case SeatState::empty:
                    if (adjacent == 0) {
                        newState = SeatState::occupied;
                    }
                    break;

                case SeatState::occupied:
                    if (adjacent >= occupiedLimit) {
                        newState = SeatState::empty;
                    }
                    break;

                default:
                    break;
            }
            newRow.push_back(newState);
        }
        nextPlan.push_back(newRow);
    }
    return nextPlan;
}

Matrix parseLines(const vector<string> &lines) {
    Matrix result;
    for (const auto &line : lines) {
        Row row;
        for (char c : line) {
            row.push_back(SeatState(c));
        }
        result.push_back(row);
    }
    return result;
}

/** DEBUGGING **/

ostream &operator<<(ostream &out, const Matrix &seatPlan) {
    for (const auto &row : seatPlan) {
        for (auto seat : row) {
            out << (char) seat;
        }
        out << endl;
    }
    return out;
}

/** END DEBUGGING **/

auto taskA() {
    const auto lines = readLines("input-11.txt");
    Matrix current = parseLines(lines);
    while (true) {
        auto next = updateSeats(current);
        if (next == current) {
            break;
        }
        current = next;
    }
    return countOccupied(current);
}

auto taskB() {
    const auto lines = readLines("input-11.txt");
    Matrix current = parseLines(lines);
    while (true) {
        auto next = updateSeats(current, countInDirection, 5);
        if (next == current) {
            break;
        }
        current = next;
    }
    return countOccupied(current);
}

MAIN;