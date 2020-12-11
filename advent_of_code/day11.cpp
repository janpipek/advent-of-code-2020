#include <iostream>
#include <string>
#include <algorithm>

#include "boost/multi_array.hpp"

#include "lib.hh"

using namespace std;

enum SeatState {
    empty,
    occupied,
    unavailable
};

using Row = vector<SeatState>;
using Matrix = vector<Row>;

size_t countOccupied(const Matrix& seatPlan) {
    size_t count = 0;
    for (const auto& row : seatPlan) {
        for (auto seat : row) {
            if (seat == SeatState::occupied) {
                count++;
            }
        }
    }
    return count;
}

size_t countAdjacent(const Matrix& seatPlan, size_t row, size_t column) {
    size_t count = 0;

    for (int i = (int)row - 1; i <= row + 1; i++) {
        if ((i < 0) || (i == seatPlan.size())) {
            continue;
        }
        for (int j = (int)column - 1; j <= column + 1; j++) {
            if ((j < 0) || (j == seatPlan[0].size())) {
                continue;
            }
            if ((i == row) && (j == column)) {  // The element itself
                continue;
            }
            if (seatPlan[i][j] == SeatState::occupied) {
                count++;
            }
        }
    }
    return count;
}

Matrix updateSeats(const Matrix& previousPlan) {
    Matrix nextPlan;
    for (size_t i = 0; i < previousPlan.size(); i++) {
        const auto& previousRow = previousPlan[i];
        Row newRow;
        for (size_t j = 0; j < previousRow.size(); j++) {
            SeatState previousState = previousRow[j]; 
            SeatState newState = previousState;
            auto adjacent = countAdjacent(previousPlan, i, j);

            switch (previousState)
            {
            case SeatState::empty:
                if (adjacent == 0) {
                    newState = SeatState::occupied;
                }
                break;

            case SeatState::occupied:
                if (adjacent >= 4) {
                    newState = SeatState::empty;
                }
                break;
            }
            newRow.push_back(newState);
        }
        nextPlan.push_back(newRow);
    }
    return nextPlan;
}

Matrix parseLines(const vector<string>& lines) {
    Matrix result;
    for (const auto& line : lines) {
        Row row;
        for (char c : line) {
            switch (c)
            {
            case 'L':
                row.push_back(SeatState::empty);
                break;

            case '#':
                row.push_back(SeatState::occupied);
                break;

            case '.':
                row.push_back(SeatState::unavailable);
                break;
            
            default:
                throw runtime_error("Invalid character on input.");
            }
        }
        result.push_back(row);
    }
    return result;
}

auto taskA() {
    const auto lines = readLines("input-11.txt");
    Matrix current = parseLines(lines);
    while (true) {
        cout << countOccupied(current) << endl;
        auto next = updateSeats(current);
        if (next == current) {
            break;
        }
        current = next;
    }
    return countOccupied(current);
}

auto taskB() {
	NOT_YET_IMPLEMENTED
}

MAIN;