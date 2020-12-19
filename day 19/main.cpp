#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"
#include <regex>

std::unordered_map<std::string, std::vector<std::string>> combination_cache;
std::vector<std::string> get_combinations(std::unordered_map<std::string, std::string> rules, std::string rule_id) {
    try {
        return combination_cache.at(rule_id);
    } catch (std::out_of_range &e) {
        std::vector<std::string> combinations;
        try {
            int quote_index = rules.at(rule_id).find('\"');
            if (quote_index != std::string::npos) {
                combinations.push_back(rules.at(rule_id).substr(quote_index + 1, 1));
            } else {
                std::vector<std::string> operands = split_by_string(rules.at(rule_id), " | ");
                for (auto operand: operands) {
                    std::vector<std::string> ids = split_by_string(operand, " ");
                    std::vector<std::string> operand_combinations;
                    for (auto id: ids) {
                        std::vector<std::string> child_combinations = get_combinations(rules, id);
                        std::vector<std::string> current_combinations;
                        if (operand_combinations.empty()) {
                            for (auto child_combination: child_combinations) {
                                current_combinations.push_back(child_combination);
                            }
                        } else {
                            for (auto operand_combination: operand_combinations){
                                for (auto child_combination: child_combinations) {
                                    current_combinations.push_back(operand_combination+child_combination);
                                }
                            }
                        }
                        operand_combinations = current_combinations;
                    }
                    combinations.insert(combinations.end(), operand_combinations.begin(), operand_combinations.end());
                }
            }
        } catch (std::out_of_range &e) {
        }
        combination_cache.insert({rule_id, combinations});
        return combinations;
    }
}

std::unordered_map<std::string, bool> map_valid_combinations(std::vector<std::string> combinations) {
    std::unordered_map<std::string, bool> valid_messages;
    for (auto message: combinations) {
        valid_messages.insert({message, true});
    }
    return valid_messages;
}

std::unordered_map<std::string, std::string> regex_cache;
std::string generate_regex(std::unordered_map<std::string, std::string> rules, std::string rule_id, unsigned long long depth) {
    try {
        return regex_cache.at(rule_id);
    } catch (std::out_of_range &e) {
        try {
            if (depth > 10)
                return "";
            int quote_index = rules.at(rule_id).find('\"');
            if (quote_index != std::string::npos) {
                return rules.at(rule_id).substr(quote_index + 1, 1);
            } else {
                std::vector<std::string> operands = split_by_string(rules.at(rule_id), " | ");
                std::string regex_string = "(";
                for (int i=0; i<operands.size(); i++) {
                    std::vector<std::string> ids = split_by_string(operands[i], " ");
                    for (auto id: ids) {
                        regex_string += generate_regex(rules, id, depth+1);
                    }
                    if (i != operands.size()-1){
                        regex_string += "|";
                    }
                }
                regex_string += ")";
                if (regex_string == "(|)")
                    regex_string = "";
                regex_cache.insert({rule_id, regex_string});
                return regex_string;
            }
        } catch (std::out_of_range &e) {
            return "";
        }
    }
}

unsigned long long part2 (std::unordered_map<std::string, std::string> rules, std::vector<std::string> messages) {
    unsigned long long count = 0;
    rules.at("8") = "42 | 42 8";
    rules.at("11") = "42 31 | 42 11 31";
    std::string regex_str = generate_regex(rules, "0", 0);
    std::regex message_regex(regex_str);
    for (auto message: messages) {
        if (std::regex_match(message.begin(), message.end(), message_regex))
            count++;
    }
    return count;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::unordered_map<std::string, std::string> rules;
    std::string line;
    while (getline(file, line)) {
        if (line.empty())
            break;
        std::vector<std::string> tokens = split_by_string(line, ": ");
        rules.insert({tokens[0], tokens[1]});
    }
    std::vector<std::string> messages;
    while (getline(file, line)) {
        messages.push_back(line);
    }
    file.close();
    std::unordered_map<std::string, bool> valid_messages = map_valid_combinations(get_combinations(rules, "0"));
    unsigned long long valid_count_one = 0;
    for (auto message: messages) {
        try {
            if (valid_messages.at(message))
                valid_count_one++;
        } catch (std::out_of_range &e) {

        }
    }
    std::cout << valid_count_one << std::endl;
    std::cout << part2(rules, messages) << std::endl;
    return 0;
}
