#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

int parseSeatId(const std::string& line) {
    int seat = 0;
    for (const auto letter : line) {
        if (letter == 'B' || letter == 'R') {
            seat += 1;
        }
        seat <<= 1;
    }
    seat >>= 1;
    return seat;
}

std::vector<int> readSeats(const std::string& path) {
    const auto lines = readLines(path);
    std::vector<int> seats;
    transform(lines.cbegin(), lines.cend(), std::back_inserter(seats), parseSeatId);
    return seats;
}

int maxSeat(const std::vector<int>& seats) {
    return *max_element(seats.begin(), seats.end());
}

auto taskA()
{
    const auto seats = readSeats("input-05.txt");
    return maxSeat(seats);
}

auto taskB()
{
    std::vector<int> seats = readSeats("input-05.txt");
    // sort(seats.begin(), seats.end());
    const int lastSeat = maxSeat(seats);
    for (int i = 1; i < lastSeat; i++) {
        if (find(seats.cbegin(), seats.cend(), i) == seats.cend()) {
            if (
                (find(seats.cbegin(), seats.cend(), i - 1) != seats.cend())
                &&
                (find(seats.cbegin(), seats.cend(), i + 1) != seats.cend())
                )
            {
                return i;
            }
        }
    }
    SOLUTION_NOT_FOUND;
}

MAIN;