#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"
#include "day5.hh"

using namespace std;

int main(int argc, char** argv)
{
    const auto seats = readSeats("input-05.txt");
    cout << "Max seat: " << maxSeat(seats) << endl;
}