#include <iostream>
#include <string>
#include <algorithm>

#include "aoc/task.hh"

using namespace std;

enum class Action: char {
    North = 'N',
    South = 'S',
    East = 'E',
    West = 'W',
    Right = 'R',
    Left = 'L',
    Forward = 'F'
};

const int NORTH = 0;
const int EAST = 90;
const int SOUTH = 180;
const int WEST = 270;

struct Instruction {
    Action action;
    int value;
};

struct Position {
    int x = 0;
    int y = 0;

    void move(int steps, int azimuth) {
        switch(azimuth) {
            case NORTH:
                y += steps;
                break;

            case EAST:
                x += steps;
                break;

            case SOUTH:
                y -= steps;
                break;

            case WEST:
                x -= steps;
                break;

            default:
                cerr << "Invalid azimuth " << azimuth << endl;
                SOLUTION_NOT_FOUND;
        }
    }

    void move(int steps, const Position wayPoint) {
        x += steps * wayPoint.x;
        y += steps * wayPoint.y;
    }    
};

struct WayPoint: Position {
    void turnLeft(int degrees) {
        int steps = degrees / 90;
        for (int i = 0; i < steps; i++) {
            swap(x, y);
            x *= -1;
        }
    }

    void turnRight(int degrees) {
        int steps = degrees / 90;
        for (int i = 0; i < steps; i++) {
            swap(x, y);
            y *= -1;
        }
    }    
};

struct ShipState {
    Position pos;
    int azimuth;

    void updateAzimuth(int change) {
        azimuth = ((azimuth + change) % 360 + 360) % 360; // Solve negative ones
    }

    void move(int steps, int azimuth) {
        pos.move(steps, azimuth);
    }

    void move(int steps) {
        pos.move(steps, azimuth);
    }
};

void performInstruction(ShipState& shipState, const Instruction instruction) {
    int value = instruction.value;

    switch (instruction.action) {
        case Action::North:
            shipState.move(value, NORTH);
            break;

        case Action::East:
            shipState.move(value, EAST);
            break;

        case Action::West:
            shipState.move(value, WEST);
            break;

        case Action::South:
            shipState.move(value, SOUTH);
            break;

        case Action::Left:
            shipState.updateAzimuth(-value);
            break;

        case Action::Right:
            shipState.updateAzimuth(value);
            break;

        case Action::Forward:
            shipState.move(value);
            break;
    }
}

/**
 * More complicated instructions for Part Two
 */
void performInstruction(Position& ship, WayPoint& wayPoint, const Instruction instruction) {
    int value = instruction.value;

    switch (instruction.action) {
        case Action::North:
            wayPoint.move(value, NORTH);
            break;

        case Action::East:
            wayPoint.move(value, EAST);
            break;

        case Action::West:
            wayPoint.move(value, WEST);
            break;

        case Action::South:
            wayPoint.move(value, SOUTH);
            break;

        case Action::Left:
            wayPoint.turnLeft(value);
            break;

        case Action::Right:
            wayPoint.turnRight(value);
            break;

        case Action::Forward:
            ship.move(value, wayPoint);
            break;
    }
}

Instruction parseInstruction(const string& line) {
    return {
        (Action)line[0],
        stoi(line.substr(1))
    };
}

auto readInstructions() {
    auto lines = readLines("input-12.txt");
    vector<Instruction> result;
    transform(lines.cbegin(), lines.cend(), back_inserter(result), parseInstruction);
    return result;
}

/* DEBUGGING */

ostream& operator<< (ostream& out, const Position& pos) {
    out << "{" << pos.x << ", " << pos.y << "}";
    return out;
}

ostream& operator<< (ostream& out, const Instruction& ins) {
    out << "{" << (char)ins.action << ", " << ins.value << "}";
    return out;
}

/* END */

auto taskA() {
    auto instructions = readInstructions();
    ShipState ship{{0, 0}, EAST};
    for (auto instruction: instructions) {
        performInstruction(ship, instruction);
    }
    return abs(ship.pos.x) + abs(ship.pos.y);
}

auto taskB() {
    auto instructions = readInstructions();
    Position ship{0, 0};
    WayPoint wayPoint{10, 1};
    for (auto instruction: instructions) {
        /* cout << "Position: " << ship << endl;
        cout << "WayPoint: " << wayPoint << endl;
        cout << "Instruction: " << instruction << endl; */
        performInstruction(ship, wayPoint, instruction);
    }
    return abs(ship.x) + abs(ship.y);
}

MAIN;