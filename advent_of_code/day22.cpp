#include <deque>
#include <iostream>
#include <string>
#include <algorithm>

#include "aoc/task.hh"

using namespace std;

using Card = uint16_t;
using Deck = deque<Card>;
using Input = pair<Deck, Deck>;

using GameLog = vector<pair<Deck, Deck>>;

void playTurn(Deck& deck1, Deck& deck2) {
    auto card1 = deck1[0];
    auto card2 = deck2[0];
    
    cout << deck1.size() << " : " << deck2.size() << " --- " << card1 << " - " << card2 << endl;

    deck1.pop_front();
    deck2.pop_front();

    if (card1 > card2) {
        deck1.push_back(card1);
        deck1.push_back(card2);
    } else {
        deck2.push_back(card2);
        deck2.push_back(card1);
    }
}

void playTurnWithRecursion(Deck& deck1, Deck& deck2, GameLog& log) {
    // TODO: Implement
}

u_int8_t playRecursiveGame(Deck& deck1, Deck& deck2) {
    // TODO: Implement
    return 0;
}

u_int64_t getScore(const Deck& deck) {
    u_int64_t result = 0ULL;
    for (size_t i = 0; i < deck.size(); i++) {
        result += (deck.size() - i) * deck[i];
    }
    return result;
}

Input readInput() {
    auto sections = readSections("input-22.txt");
    Input result;
    for (auto line = sections[0].cbegin() + 1; line < sections[0].cend(); line++) {
        result.first.push_back(stoi(*line));
    }
    for (auto line = sections[1].cbegin() + 1; line < sections[1].cend(); line++) {
        result.second.push_back(stoi(*line));
    }
    return result;
}

auto taskA() {
    auto input = readInput();

    while (true) {
        playTurn(input.first, input.second);
        if (input.first.empty()) {
            return getScore(input.second);
        }
        if (input.second.empty()) {
            return getScore(input.first);
        }
    }
}

auto taskB() {
    NOT_YET_IMPLEMENTED
}

MAIN;