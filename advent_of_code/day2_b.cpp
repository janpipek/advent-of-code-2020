#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

bool validLine(const string& line) {
	auto rule_password = split(line, ':');
	auto positions_letter = split(rule_password[0]);
	auto first_second = split(positions_letter[0], '-');
	int first = stoi(first_second[0]);
	int second = stoi(first_second[1]);
	char letter = positions_letter[1][0];
	string password = rule_password[1]; // offset 1 ;-)
	return (password[first] == letter) != (password[second] == letter);
}

int main(int argc, char** argv) {
	auto lines = readLines("input-02.txt");
	int validCount = count_if(lines.begin(), lines.end(), validLine);
	cout << "Valid lines count: " << validCount << endl;
}