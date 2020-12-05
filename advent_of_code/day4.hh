#ifndef DAY4_HH
#define DAY4_HH

#include <map>
#include <string>
#include <vector>
#include <regex>

class Passport {
public:
    static const std::vector<std::string> requiredFields;

    bool hasRequiredFields() const {
        for (auto& field : requiredFields) {
            if (_fields.find(field) == _fields.end()) {
                return false;
            }
        }
        return true;
    }

    bool isValid() const {
        if (!hasRequiredFields()) {
            return false;
        }
        // All fields are present, no need to check

        // byr
        const auto byr = getValue("byr");
        if (!_matchesYear(byr, 1920, 2002)) {
            return false;
        }

        // iyr
        const auto iyr = getValue("iyr");
        if (!_matchesYear(iyr, 2010, 2020)) {
            return false;
        }

        // eyr
        const auto eyr = getValue("eyr");
        if (!_matchesYear(eyr, 2020, 2030)) {
            return false;
        }

        // hgt
        const auto hgt = getValue("hgt");
        const std::regex hgtPattern("(\\d+)(cm|in)");
        std::smatch m;
        if (!std::regex_match(hgt, m, hgtPattern)) {
            return false;
        }
        bool metric = (m[2] == "cm");
        int hgtValue = stoi(m[1]);
        if (metric) {
            if ((hgtValue < 150) || (hgtValue > 193)) {
                return false;
            }
        }
        else {
            if ((hgtValue < 59) || (hgtValue > 76)) {
                return false;
            }
        }

        // hcl
        const auto hcl = getValue("hcl");
        std::regex hclPattern("#[0-9a-f]{6}");
        if (!std::regex_match(hcl, hclPattern)) {
            return false;
        }

        // ecl
        const auto ecl = getValue("ecl");
        std::vector<std::string> allowedColors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
        if (std::find(allowedColors.begin(), allowedColors.end(), ecl) == allowedColors.end()) {
            return false;
        }

        // pid
        const std::regex pidPattern("\\d{9}");
        const auto pid = getValue("pid");
        if (!std::regex_match(pid, pidPattern)) {
            return false;
        }
        return true;
    }

    const std::string getValue(const std::string& key) const {
        return _fields.at(key);
    }

    void addField(std::string key, std::string value) {
        _fields[key] = value;
    }

private:
    std::map<std::string, std::string> _fields;

    bool _matchesYear(std::string str, int minYear, int maxYear) const {
        std::regex allDigits("\\d+");
        if (!std::regex_match(str, allDigits)) {
            return false;
        }
        const int year = stoi(str);
        if ((year < minYear) || (year > maxYear)) {
            return false;
        }
        return true;
    }
};

void parseLineInto(const std::string& line, Passport& passport) {
    for (auto& kv_pair : split(line)) {
        auto key_value = split(kv_pair, ':');
        passport.addField(key_value[0], key_value[1]);
    }
}

const std::vector<std::string> Passport::requiredFields = { "iyr", "hcl", "hgt", "eyr", "byr", "ecl", "pid" };

const std::vector<Passport> readPassports(const std::string& path) {
    auto lines = readLines(path);

    std::vector<Passport> passports;
    Passport currentPassport;

    for (auto& line : lines) {
        if (!line.length()) {
            passports.push_back(currentPassport);
            currentPassport = Passport();
        }
        else {
            parseLineInto(line, currentPassport);
        }
    }
    passports.push_back(currentPassport);
    return passports;
}

#endif // !DAY4_HH