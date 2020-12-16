#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"
#include <algorithm>

std::pair<int, int> return_int_pair(std::string pstr) {
    std::vector<std::string> tokens = split_by_string(pstr, "-");
    std::pair<int, int> range;
    range.first = std::stoi(tokens[0]);
    range.second = std::stoi(tokens[1]);
    return range;
}

std::vector<int> convert_vector(std::vector<std::string> v) {
    std::vector<int> to_convert;
    for (auto i: v)
        to_convert.push_back(std::stoi(i));
    return to_convert;
}

bool valid_attribute_value(std::pair<std::pair<int, int>, std::pair<int, int>> attribute, int val) {
    int range_begin_one = attribute.first.first;
    int range_end_one = attribute.first.second;
    int range_begin_two = attribute.second.first;
    int range_end_two = attribute.second.second;
    if ((val >= range_begin_one && val <= range_end_one)
        || (val >= range_begin_two && val <= range_end_two))
        return true;
    else
        return false;
}

bool value_valid(std::unordered_map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> attributes,
                 int val) {
    for (auto attribute: attributes) {
        if (valid_attribute_value(attribute.second, val))
            return true;
    }
    return false;
}

int invalid_ticket_values_sum(
        std::unordered_map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> attributes,
        std::vector<int> ticket) {
    int sum = 0;
    for (auto num: ticket) {
        if (!value_valid(attributes, num))
            sum += num;
    }
    return sum;
}

std::unordered_map<std::string, int>
attribute_order(std::unordered_map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> attributes,
                std::vector<std::vector<int>> tickets) {
    std::vector<std::pair<std::string, std::vector<int>>> matching_indexes;
    for (auto attribute: attributes) {
        std::vector<int> valid_indexes;
        for (int i = 0; i < tickets[0].size(); i++) {
            bool valid;
            for (auto ticket: tickets) {
                valid = valid_attribute_value(attribute.second, ticket[i]);
                if (!valid)
                    break;
            }
            if (valid) {
                valid_indexes.push_back(i);
            }
        }
        matching_indexes.push_back({attribute.first, valid_indexes});
    }
    auto sort_by_least_index = [] (std::pair<std::string, std::vector<int>> a, std::pair<std::string, std::vector<int>> b) {
        return a.second.size() < b.second.size();
    };
    std::sort(matching_indexes.begin(), matching_indexes.end(), sort_by_least_index);
    std::unordered_map<std::string, int> sorted;
    std::unordered_map<int, bool> taken_indexes;
    while (taken_indexes.size() != tickets[0].size()) {
        for (auto &index: matching_indexes) {
            if (index.second.size() == 1) {
                taken_indexes.insert({index.second[0], true});
                sorted.insert({index.first, index.second[0]});
                index.second.erase(index.second.begin());
            }
            for (int i=0; i<index.second.size(); i++) {
                try {
                    if (taken_indexes.at(index.second[i]))
                        index.second.erase(index.second.begin()+i);
                } catch (std::out_of_range &e) {
                }
            }
        }
    }
    return sorted;
}

unsigned long long
location_mult(std::unordered_map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> attributes,
              std::vector<std::vector<int>> tickets, std::vector<int> ticket) {
    std::unordered_map<std::string, int> order = attribute_order(attributes, tickets);
    int i1 = order.at("departure location");
    int i2 = order.at("departure station");
    int i3 = order.at("departure platform");
    int i4 = order.at("departure track");
    int i5 = order.at("departure date");
    int i6 = order.at("departure time");
    return (unsigned long long) ticket[i1] * (unsigned long long) ticket[i2] *
           (unsigned long long) ticket[i3] * (unsigned long long) ticket[i4] * (unsigned long long) ticket[i5] *
           (unsigned long long) ticket[i6];
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string line;
    std::unordered_map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> attributes;
    while (getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::vector<std::string> tokens = split_by_string(line, ":");
        std::string type = tokens[0];
        tokens[1] = remove_spaces(tokens[1]);
        tokens = split_by_string(tokens[1], "or");
        std::string first_range = remove_spaces(tokens[0]);
        std::string second_range = remove_spaces(tokens[1]);
        attributes.insert({type, {return_int_pair(first_range), return_int_pair(second_range)}});
    }
    getline(file, line);
    getline(file, line);
    std::string ticket_string = line;
    getline(file, line);
    getline(file, line);
    int invalid_sum = 0;
    std::vector<std::vector<int>> valid_tickets;
    while (getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::vector<int> ticket = convert_vector(split_by_string(line, ","));
        int current_sum = invalid_ticket_values_sum(attributes, ticket);
        invalid_sum += current_sum;
        if (current_sum == 0)
            valid_tickets.push_back(ticket);
    }
    std::cout << invalid_sum << std::endl;
    std::cout << location_mult(attributes, valid_tickets, convert_vector(split_by_string(ticket_string, ",")))
              << std::endl;
    return 0;
}
