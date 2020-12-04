#include <iostream>
#include <map>
#include <string>
#include <algorithm>

#include "lib.hh"
#include "day4.hh"

using namespace std;


int main(int argc, char **argv)
{
    auto passports = readPassports("input-04.txt");
    int validPassports = count_if(passports.begin(), passports.end(), [](auto& p) { return p.isValid(); });
    cout << "Valid passports: " << validPassports << endl;
}