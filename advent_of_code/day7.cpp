#include <iostream>
#include <map>
#include <string>
#include <algorithm>

#include "lib.hh"

using namespace std;

struct BagRule {
    std::string name;
    std::map<string, int> items;
};

bool canContain(const map<string, BagRule>& rules, const BagRule& rule, const string& color) {
    if (rule.items.find(color) != rule.items.end()) {
        return true;
    }
    for (const auto& kv : rule.items) {
        const auto& item = rules.at(kv.first);
        if (canContain(rules, item, color)) {
            return true;
        }
    }
    return false;
}

int totalBags(const map<string, BagRule>& rules, const BagRule& rule) {
    int total = 0;
    for (const auto& item : rule.items) {
        const BagRule& itemRule = rules.at(item.first);
        int itemCount = item.second;
        int itemTotal = itemCount * (1 + totalBags(rules, itemRule));
        total += itemTotal;
    }
    return total;
}

BagRule parseRule(const string& line) {
    auto color_items = split(line, " bags contain ", 1);
    BagRule rule = { color_items[0], {} };
    if (color_items[1] == "no other bags.") {
        return rule;
    }
    else {
        auto items = split(color_items[1], ", ");
        for (const auto& item : items) {
            auto number_rest = split(item, " ");
            int itemCount = stoi(number_rest[0]);
            auto itemColor = number_rest[1] + " " + number_rest[2];
            rule.items[itemColor] = itemCount;
        }
    }
    return rule;
}

map<string, BagRule> readRules() {
    auto lines = readLines("input-07.txt");
    map<string, BagRule> rules;

    for (const auto& line : lines) {
        auto rule = parseRule(line);
        rules[rule.name] = rule;
    }
    return rules;
}

auto taskA() {
    const auto rules = readRules();
    return count_if(rules.begin(), rules.end(), [rules](const auto& kv) { return canContain(rules, kv.second, "shiny gold"); });
}

auto taskB() {
    const auto rules = readRules();
    auto shinyGoldRule = rules.at("shiny gold");
    return totalBags(rules, shinyGoldRule);
}

MAIN;