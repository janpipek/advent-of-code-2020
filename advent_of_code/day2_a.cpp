#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

bool validLine(const string& line) {
	auto rule_password = split(line, ':');
	auto range_letter = split(rule_password[0]);
	auto lower_upper = split(range_letter[0], '-');
	int lower = stoi(lower_upper[0]);
	int upper = stoi(lower_upper[1]);
	char letter = range_letter[1][0];
	string password = rule_password[1].substr(1);

	int occurrences = count(password.begin(), password.end(), letter);
	return (occurrences >= lower) && (occurrences <= upper);
}

int main(int argc, char** argv) {
	auto lines = readLines("input-02.txt");
	int validCount = count_if(lines.begin(), lines.end(), validLine);
	cout << "Valid lines count: " << validCount << endl;
}