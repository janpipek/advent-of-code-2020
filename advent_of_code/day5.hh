#ifndef DAY5_HH
#define DAY5_HH

#include <string>
#include <vector>
#include <algorithm>

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

const std::vector<int> readSeats(const std::string& path) {
    const auto lines = readLines(path);
    std::vector<int> seats;
    transform(lines.cbegin(), lines.cend(), std::back_inserter(seats), parseSeatId);
    return seats;
}

int maxSeat(const std::vector<int>& seats) {
    return *max_element(seats.begin(), seats.end());
}

#endif