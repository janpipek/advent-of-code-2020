#include <string>
#include <istream>

#include "aoc/task.hh"
#include "aoc/mathematics.hh"

using namespace std;

const char NONE = ' ';
const char PLUS = '+';
const char TIMES = '*';
const char LPAREN = '(';
const char RPAREN = ')';

long long applyOperator(char op, long long value1, long long value2) {
    // Apply
    if (op == PLUS) {
        return value1 + value2;
    }
    else if (op == TIMES) {
        return value1 * value2;
    }
    else {
        return value2;
    }
}

long long evalLine(istringstream& stream) {
    long long value = 0;

    char currentOperator = NONE;

    char token;
    while (stream.peek() != EOF) {
        stream.read(&token, 1);

        // Just change operator
        if ((token == PLUS) || (token == TIMES)) {
            currentOperator = token;
        }

        // Enter subexpression
        else if (token == LPAREN) {
            value = applyOperator(currentOperator, value, evalLine(stream));
        }

        // Exit subexpression
        else if (token == RPAREN) {
            return value;
        }

            // Spaces are ignored
        else if (token == ' ') {
            continue;
        }

            // Just the number
        else {
            int number = string("0123456789").find(token);
            if (number != string::npos) {
                value = applyOperator(currentOperator, value, number);
            }
        }
    }
    return value;
}

long long evalLine2(istringstream &stream) {
    long long currentFactor = 0;
    vector<long long> factors{};

    char token;
    while (stream.peek() != EOF) {
        stream.read(&token, 1);

        // Just change operator
        if (token == TIMES) {
            factors.push_back(currentFactor);
            currentFactor = 0;
        }

            // Exit subexpression
        else if (token == RPAREN) {
            break;
        }

            // Spaces are ignored, plus is implicit
        else if ((token == ' ') || (token == PLUS)) {
            continue;
        }

            // Number of subexpression
        else {
            long long number = (token == LPAREN) ? evalLine2(stream) : string("0123456789").find(token);
            if (number == string::npos) {
                SOLUTION_NOT_FOUND
            }
            currentFactor += number;
        }
    }
    if (currentFactor) {
        factors.push_back(currentFactor);
    }
    return product(factors);
}

auto taskA() {
    long long result = 0;
    auto lines = readLines("input-18.txt");
    for (const auto &line: lines) {
        istringstream stream(line);
        result += evalLine(stream);
    }
    return result;
}

auto taskB() {
    long long result = 0;
    auto lines = readLines("input-18.txt");
    for (const auto &line: lines) {
        istringstream stream(line);
        result += evalLine2(stream);
    }
    return result;
}

MAIN;