#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <regex>
#include "../commonly_used_functions/library.cpp"

typedef struct {
    std::map<std::string, std::string> data = {
            {"byr",  ""},
            {"iyr", ""},
            {"eyr", ""},
            {"hgt", ""},
            {"hcl", ""},
            {"ecl", ""},
            {"pid", ""},
            {"cid", ""}
    };
} user;

bool part_one_valid(user user1){
    for (auto key : user1.data){
            if (key.first == "cid") {
                continue;
            } else if (key.second.empty()) {
                return false;
            }
    }
    return true;
}

/*
byr (Birth Year) - four digits; at least 1920 and at most 2002.
iyr (Issue Year) - four digits; at least 2010 and at most 2020.
eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
hgt (Height) - a number followed by either cm or in:
If cm, the number must be at least 150 and at most 193.
If in, the number must be at least 59 and at most 76.
hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
pid (Passport ID) - a nine-digit number, including leading zeroes.
cid (Country ID) - ignored, missing or not.
*/
bool part_two_valid(user user1) {
    bool valid = true;
    for (auto key: user1.data) {
        std::string attribute = key.first;
        std::string val = key.second;
        if (attribute == "byr")
            valid = val.length() == 4 && std::stoi(val) >= 1920 && std::stoi(val) <= 2020;
        else if (attribute == "iyr")
            valid = val.length() == 4 && std::stoi(val) >= 2010 && std::stoi(val) <= 2020;
        else if (attribute == "eyr")
            valid = val.length() == 4 && std::stoi(val) >= 2020 && std::stoi(val) <= 2030;
        else if (attribute == "hgt") {
            bool cm = false;
            if (val.find("cm") != std::string::npos) {
                cm = true;
            } else if (val.find("in") == std::string::npos) {
                valid = false;
            }

            try {
                int hgt = std::stoi(val.substr(0, val.length()-2));
                if (cm)
                    valid = hgt >= 150 && hgt <= 193;
                else
                    valid = hgt >= 59 && hgt <= 76;
            } catch (std::invalid_argument &e) {
                valid = false;
            }
        }
        else if (attribute == "hcl") {
            std::regex valid_hcl("#([0-9]|[a-f]){6}");
            valid = std::regex_match(val.begin(), val.end(), valid_hcl);
        } else if (attribute == "ecl") {
            valid  = val == "amb" || val == "blu" || val == "brn"
                    || val == "brn" || val=="gry" || val == "grn" || val == "hzl" || val == "oth";
        } else if (attribute == "pid") {
            std::regex valid_pid("[0-9]{9}");
            valid = std::regex_match(val.begin(), val.end(), valid_pid);
        } else if (attribute == "cid") {
            continue;
        } else {
            valid = false;
        }
        
        if (!valid)
            return false;
    }
    return valid;
}

int main() {
    std::fstream file;
    std::string line;
    std::vector<user> users;
    file.open("../input.txt", std::fstream::in);
    int valid_count1 = 0;
    int valid_count2 = 0;
    std::vector<std::string> user_lines;
    while (getline(file, line)) {
        if (line.empty()) { // must have 2 empty lines at the end of input
            user current_user;
            for (auto user_line : user_lines){
                std::vector<std::string> attributes = split(user_line, ' ');
                for (auto attribute : attributes) {
                    std::vector<std::string> type_val = split(attribute, ':');
                    try {
                        current_user.data.at(type_val[0]) = type_val[1];
                    } catch (std::out_of_range &e) {
                        continue;
                    }
                }
            }
            if (part_one_valid(current_user))
                valid_count1++;
            if (part_two_valid(current_user))
                valid_count2++;
            users.push_back(current_user);
            user_lines.clear();
        } else {
            user_lines.push_back(line);
        }
    }
    file.close();
    std::cout << valid_count1 << std::endl;
    std::cout << valid_count2 << std::endl;
    return 0;
}
