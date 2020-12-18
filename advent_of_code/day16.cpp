#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <algorithm>

#include "aoc/task.hh"

using namespace std;

struct Range {
    int low;
    int high;

    bool matches(int number) const {
        return (number >= low) && (number <= high);
    }
};

struct RangeChoice {
    Range first;
    Range second;

    bool matches(int number) const {
        return first.matches(number) || second.matches(number);
    }
};

using Ticket = vector<int>;

struct Input {
    map<string, RangeChoice> rules;
    Ticket yourTicket;
    vector<Ticket> nearbyTickets;
};

Range parseRange(const string &s) {
    auto parts = split(s, "-");
    return {stoi(parts[0]), stoi(parts[1])};
}

vector<int> parseTicket(const string &line) {
    auto parts = split(line, ',');
    Ticket result;
    transform(parts.cbegin(), parts.cend(), back_inserter(result), [](const auto &part) { return stoi(part); });
    return result;
}

Input readInput() {
    Input result{};

    auto sections = readSections("input-16.txt");

    // Rules
    for (const auto &line: sections[0]) {
        auto name_ranges = split(line, ": ");
        // string name = name_ranges[0]
        auto ranges = split(name_ranges[1], " or ");
        result.rules[name_ranges[0]] = RangeChoice{
                parseRange(ranges[0]),
                parseRange(ranges[1])
        };
    }

    // Your ticket
    result.yourTicket = parseTicket(sections[1][1]);

    // Nearby tickets
    transform(
            sections[2].cbegin() + 1,
            sections[2].cend(),
            back_inserter(result.nearbyTickets),
            parseTicket
    );

    return result;
}

bool isValid(const Ticket &ticket, const map<string, RangeChoice> &rules) {
    for (auto number: ticket) {
        if (!any_of(
                rules.cbegin(),
                rules.cend(),
                [number](const auto &rule) {
                    return rule.second.matches(number);
                }
        )) {
            return false;
        }
    }
    return true;
}

vector<Ticket> allValidTickets(const Input &input) {
    vector<Ticket> result;
    copy_if(
            input.nearbyTickets.cbegin(),
            input.nearbyTickets.cend(), back_inserter(result),
            [input](const auto &ticket) {
                return isValid(ticket, input.rules);
            });
    result.push_back(input.yourTicket);
    return result;
}

auto taskA() {
    auto input = readInput();
    int result = 0;
    for (const auto &ticket : input.nearbyTickets) {
        for (auto number: ticket) {
            if (!any_of(
                    input.rules.cbegin(),
                    input.rules.cend(),
                    [number](const auto &rule) {
                        return rule.second.matches(number);
                    }
            )) {
                result += number;
            }
        }
    }
    return result;
}

auto taskB() {
    auto input = readInput();

    auto validTickets = allValidTickets(input);

    map<int, vector<string>> allMatches;

    // Find all rules that match each ticket position
    for (const auto &name_rule : input.rules) {
        const auto &name = name_rule.first;
        const auto &rule = name_rule.second;

        for (int i = 0; i < input.yourTicket.size(); i++) {
            bool hasMatch = true;
            for (const auto &ticket: validTickets) {
                if (!rule.matches(ticket[i])) {
                    hasMatch = false;
                    break;
                }
            }
            if (hasMatch) {
                allMatches[i].push_back(name);
            }
        }
    }

    map<string, int> fieldOrder;

    // Positions have 1 or more matching rules, iteratively remove the unique ones
    while (fieldOrder.size() != input.yourTicket.size()) {
        for (const auto &kv : allMatches) {
            if (kv.second.size() == 1) {
                string name = kv.second[0];
                fieldOrder[name] = kv.first;

                for (auto &kv2 : allMatches) {
                    const auto& matchingNames = kv2.second;
                    vector<string> newMatchingNames;
                    remove_copy_if(
                            matchingNames.cbegin(),
                            matchingNames.cend(),
                            back_inserter(newMatchingNames),
                            [name](auto s) { return s == name; }
                            );
                    allMatches[kv2.first] = newMatchingNames;
                }
                break;
            }
        }
    }

    long long result = 1;
    for (const auto& kv: fieldOrder) {
        if (kv.first.substr(0, 9) == "departure") {
            result *= input.yourTicket[kv.second];
        }
    }

    return result;
}

MAIN;