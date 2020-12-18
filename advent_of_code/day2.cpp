#include <string>
#include <algorithm>

#include "aoc/task.hh"

using namespace std;

bool validLineA(const string& line) {
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

bool validLineB(const string& line) {
	auto rule_password = split(line, ':');
	auto positions_letter = split(rule_password[0]);
	auto first_second = split(positions_letter[0], '-');
	int first = stoi(first_second[0]);
	int second = stoi(first_second[1]);
	char letter = positions_letter[1][0];
	string password = rule_password[1]; // offset 1 ;-)
	return (password[first] == letter) != (password[second] == letter);
}

auto taskA() {
	auto lines = readLines("input-02.txt");
	return count_if(lines.begin(), lines.end(), validLineA);
}

auto taskB() {
	auto lines = readLines("input-02.txt");
	return count_if(lines.begin(), lines.end(), validLineB);
}

MAIN;
