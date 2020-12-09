#include <iostream>
#include <map>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

struct Instruction {
    std::string code;
    int number;
};

Instruction parseInstruction(const string& line) {
    auto code_number = split(line);
    return { code_number[0], stoi(code_number[1]) };
}

vector<Instruction> readInstructions(const string& path) {
    auto lines = readLines(path);
    vector<Instruction> instructions;
    for (const auto& line : lines) {
        instructions.push_back(parseInstruction(line));
    }
    return instructions;
}

pair<bool, int> runProgram(const vector<Instruction>& instructions) {
    int accumulator = 0;
    int position = 0;
    vector<bool> visited(instructions.size(), false);

    while ((position < instructions.size()) && !visited[position]) {
        visited[position] = true;
        const auto& instruction = instructions[position];
        if (instruction.code == "nop") {
            position++;
        }
        else if (instruction.code == "acc") {
            accumulator += instruction.number;
            position++;
        }
        else if (instruction.code == "jmp") {
            position += instruction.number;
        }
    }
    return { position == instructions.size(), accumulator };
}

auto taskA() {
    auto instructions = readInstructions("input-08.txt");
    auto result = runProgram(instructions);
    return result.second;
}

auto taskB() {
    auto instructions = readInstructions("input-08.txt");
    pair<bool, int> result;

    for (int toChange = 0; toChange < instructions.size(); toChange++) {
        vector<Instruction> copy = instructions;
        if (copy[toChange].code == "jmp") { copy[toChange].code = "nop"; }
        else if (copy[toChange].code == "nop") { copy[toChange].code = "jmp"; }
        else { continue; }

        result = runProgram(copy);
        if (result.first) {
            break;
        }
    }
    return result.second;
}

MAIN;