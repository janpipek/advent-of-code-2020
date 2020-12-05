#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"
#include "day5.hh"

using namespace std;

int main(int argc, char** argv)
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
                cout << "Your seat: " << i << endl;
                exit(0);
            }
        }
    }
    cerr << "Not found." << endl;
    exit(-1);
}