#include <deque>
#include <iostream>
#include <string>
#include <algorithm>

#include "aoc/task.hh"

using namespace std;

using Card = uint16_t;
using Deck = deque<Card>;
using Input = pair<Deck, Deck>;

const uint8_t PLAYER1 = 1;
const uint8_t PLAYER2 = 2;

using GameLog = vector<pair<Deck, Deck>>;

void playTurn(Deck& deck1, Deck& deck2) {
    auto card1 = deck1[0];
    auto card2 = deck2[0];
    
    debug << deck1.size() << " : " << deck2.size() << " --- " << card1 << " - " << card2 << endl;

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

uint8_t playGame(Deck &deck1, Deck &deck2) {
    while (true) {
        playTurn(deck1, deck2);
        if (deck1.empty()) {
            return PLAYER2;
        }
        if (deck2.empty()) {
            return PLAYER1;
        }
    }
}

uint8_t playRecursiveGame(Deck &deck1, Deck &deck2, size_t game = 1) {
    static size_t maxGame = 1;
    if (game > maxGame) {
        maxGame = game;
    }

    debug << endl << "=== Game " << game << " ===" << endl << endl;

    GameLog log{};

    int round = 0;

    while (true) {
        debug << endl;

        round++;
        debug << "-- Round " << round << " (Game " << game << ") --" << endl;
        debug << "Player 1's deck: " << deck1 << endl;
        debug << "Player 2's deck: " << deck2 << endl;

        Card& card1 = deck1[0];
        Card& card2 = deck2[0];

        debug << "Player 1 plays: " << card1 << endl;
        debug << "Player 2 plays: " << card2 << endl;

        // Check for the same situation before
        pair<deque<Card>, deque<Card>> currentDecks{deck1, deck2};
        auto it = find(log.cbegin(), log.cend(), currentDecks);
        if (it != log.cend()) {
            debug << "Same as round " << (it - log.cbegin() + 1) << endl;
            return PLAYER1;
        }

        /*
        // With swapped cards;
        pair<deque<Card>, deque<Card>> revDecks{deck2, deck1};
        it = find(log.cbegin(), log.cend(), revDecks);
        if (it != log.cend()) {
            debug << "Same as round " << (it - log.cbegin() + 1) << endl;
            return PLAYER1;
        } */       

        log.push_back(currentDecks);

        deck1.pop_front();
        deck2.pop_front();

        uint8_t turnWinner;

        if ((deck1.size() >= card1) && (deck2.size() >= card2)) {
            // Sub-game
            debug << "Playing a sub-game to determine the winner..." << endl;

            Deck subdeck1;
            Deck subdeck2;
            copy(deck1.begin(), deck1.begin() + card1, back_inserter(subdeck1));
            copy(deck2.begin(), deck2.begin() + card2, back_inserter(subdeck2));

            turnWinner = playRecursiveGame(subdeck1, subdeck2, maxGame+1);

            debug << "...anyway, back to game " << game << "." << endl;
        }
        else {
            // Standard
            turnWinner = (card1 > card2) ? PLAYER1 : PLAYER2; 
        }

        if (turnWinner == PLAYER1) {
            debug << "Player 1 wins round " << round << " of game " << game << "!" << endl;
            deck1.push_back(card1);
            deck1.push_back(card2);
        } else {
            debug << "Player 2 wins round " << round << " of game " << game << "!" << endl;
            deck2.push_back(card2);
            deck2.push_back(card1);
        }

        // Final winner check
        if (deck1.empty()) {
            debug << "The winner of game " << game << " is player 2!" << endl;
            return PLAYER2;
        }
        if (deck2.empty()) {
            debug << "The winner of game " << game << " is player 1!" << endl;
            return PLAYER1;
        }       
    }
}

uint64_t getScore(const Deck &deck) {
    uint64_t result = 0ULL;
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

    auto winner = playGame(input.first, input.second);
    if (winner == PLAYER1) {
        return getScore(input.first);
    } else {
        return getScore(input.second);
    }
}

auto taskB() {
    // Note: We should probably cache the game results.
    //   Otherwise, it is computation-heavy.
    //   Compiled with GCC in WSL, the program fails with bad_alloc
    //   Compiled with VC++, it runs for a couple of minutes.

    auto input = readInput();

    auto winner = playRecursiveGame(input.first, input.second);

    debug << "== Post-game results ==" << endl;
    debug << "Player 1's deck: " << input.first << endl;
    debug << "Player 2's deck: " << input.second << endl;

    if (winner == PLAYER1) {
        return getScore(input.first);
    } else {
        return getScore(input.second);
    }
}

MAIN;