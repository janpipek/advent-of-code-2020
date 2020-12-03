#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
	ifstream infile("data\\input_01.txt");
	string buffer;

	while (getline(infile, buffer)) {
		cout << buffer << endl;
		cout << "." << endl;
	}
}