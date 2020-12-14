#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"
#include <cmath>

int get_memory_adress(std::string memory_str) {
    int start = memory_str.find('[')+1;
    int finish = memory_str.find(']');
    if (start == std::string::npos || finish == std::string::npos)
        return -1;
    return std::stoi(memory_str.substr(start, finish-start));
}

std::string pad_binary(std::string binary, int count) {
    binary.insert(0, count, '0');
    return binary;
}

std::string return_masked_val(std::string val, std::string mask) {
    if (val.length() < mask.length())
        val = pad_binary(val, mask.length()-val.length());
    else if (val.length() > mask.length())
        mask = pad_binary(mask, val.length()-mask.length());
    for (int i=0; i<mask.length(); i++) {
        if (mask[i] == 'X')
            continue;
        else
            val[i] = mask[i];
    }
    return val;
}

std::string decimal_to_binary (unsigned int decimal) {
    if (decimal == 0)
        return "0";
    std::string binary;
    while (decimal > 0) {
        binary.insert(0, std::to_string(decimal%2));
        decimal /= 2;
    }
    return binary;
}

unsigned long long binary_to_decimal(std::string binary) {
    unsigned long long decimal = 0;
    for (int i=0; i<binary.length();i++) {
        decimal += (binary[i]-'0')*(unsigned long long)std::pow(2, binary.length()-i-1);
    }
    return decimal;
}

unsigned long long part1(std::vector<std::string> input) {
    std::unordered_map<int, std::string> memory;
    std::string bitmask;
    for (auto line: input) {
        std::vector<std::string> tokens = split(line, '=');
        if (remove_spaces(tokens[0]) == "mask") {
            bitmask = remove_spaces(tokens[1]);
        } else {
            int adress = get_memory_adress(remove_spaces(tokens[0]));
            std::string val = return_masked_val(decimal_to_binary(
                    std::stoi(remove_spaces(tokens[1]))), bitmask);
            try {
                memory.at(adress) = val;
            } catch (std::out_of_range &e) {
                memory.insert({adress, val});
            }
        }
    }
    unsigned long long total = 0;
    for (auto m: memory) {
        total += binary_to_decimal(m.second);
    }
    return total;
}

void apply_floating_bits(std::unordered_map<unsigned long long, std::string> &memory, std::string bitmask,
                         std::string binary_adress, std::string binary_val)
{
    binary_adress = pad_binary(binary_adress, bitmask.length()-binary_adress.length());
    int x_count = 0;
    for (auto c: bitmask){
        if (c == 'X')
            x_count++;
    }
    for (int i=0; i<pow(2, x_count); i++) {
        std::string current_adress = binary_adress;
        std::string current_binary = decimal_to_binary(i); // for keeping track of what to put for x
        current_binary = pad_binary(current_binary, x_count-current_binary.length());
        int current_binary_index = 0;
        for (int j=0; j<bitmask.length(); j++) {
            if (bitmask[j] == 'X') {
                current_adress[j] = current_binary[current_binary_index];
                current_binary_index++;
            } else if (bitmask[j] == '1') {
                current_adress[j] = '1';
            }
        }
        try {
            memory.at(binary_to_decimal(current_adress)) = binary_val;
        } catch (std::out_of_range &e) {
            memory.insert({binary_to_decimal(current_adress), binary_val});
        }
    }
}

unsigned long long part2(std::vector<std::string> input) {
    std::unordered_map<unsigned long long, std::string> memory;
    std::string bitmask;
    for (auto line: input) {
        std::vector<std::string> tokens = split(line, '=');
        if (remove_spaces(tokens[0]) == "mask") {
            bitmask = remove_spaces(tokens[1]);
        } else {
            std::string binary_adress = decimal_to_binary(
                    get_memory_adress(remove_spaces(tokens[0])));
            std::string val = decimal_to_binary(std::stoi(remove_spaces(tokens[1])));
            apply_floating_bits(memory, bitmask, binary_adress, val);
        }
    }
    unsigned long long total = 0;
    for (auto m: memory) {
        total += binary_to_decimal(m.second);
    }
    return total;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string line;
    std::string bitmask;
    std::vector<std::string> input;
    while (getline (file, line)) {
        input.push_back(line);
    }
    file.close();
    std::cout << part1(input) << std::endl;
    std::cout << part2(input) << std::endl;
    return 0;
}
