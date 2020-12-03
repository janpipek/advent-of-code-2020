#include <iostream>
#include <string>
#include <algorithm>

#include "lib.hh"
#include "day3.hh"

using namespace std;

int main(int argc, char **argv)
{
	auto lines = readLines("input-03.txt");
	Matrix data = parseTrees(lines);
	int trees = countTrees(data, 3, 1);
	cout << trees << " trees" << endl;
}