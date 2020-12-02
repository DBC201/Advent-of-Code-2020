#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<std::string> split(std::string str, char delimeter) {
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    while (getline(ss, token, delimeter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool part_one_valid(std::string password, std::vector<std::string> range_string, char letter) {
    int min = std::stoi(range_string[0]);
    int max = std::stoi(range_string[1]);
    int count = 0;
    for (auto c : password) {
        if (c == letter) {
            count++;
        }
    }
    return count >= min && count <= max;
}

int part_one() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    std::string line;
    int valid_count = 0;
    while (getline(file, line)) {
        std::vector<std::string> tokens = split(line, ' ');
        std::vector<std::string> range_string = split(tokens[0], '-');
        char letter = tokens[1][0];
        std::string password = tokens[2];
        if (part_one_valid(password, range_string, letter)) {
            valid_count += 1;
        }
    }
    file.close();
    return valid_count;
}

bool part_two_valid(std::string password, std::vector<std::string> range_string, char letter) {
    int index1 = std::stoi(range_string[0])-1;
    int index2 = std::stoi(range_string[1])-1;
    return (password[index1] == letter || password[index2] == letter)
                                        && (password[index1] != letter || password[index2] != letter);
}

int part_two() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    std::string line;
    int valid_count = 0;
    while (getline(file, line)) {
        std::vector<std::string> tokens = split(line, ' ');
        std::vector<std::string> index_string = split(tokens[0], '-');
        char letter = tokens[1][0];
        std::string password = tokens[2];
        if (part_two_valid(password, index_string, letter)) {
            valid_count += 1;
        }
    }
    file.close();
    return valid_count;
}

int main() {
    std::cout << part_one() << std::endl;
    std::cout << part_two() << std::endl;
    return 0;
}
