#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../commonly_used_functions/library.cpp"

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

bool part_two_valid(std::string password, std::vector<std::string> range_string, char letter) {
    int index1 = std::stoi(range_string[0])-1;
    int index2 = std::stoi(range_string[1])-1;
    return (password[index1] == letter || password[index2] == letter)
                                        && (password[index1] != letter || password[index2] != letter);
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    std::vector<std::string> lines;
    std::string line;
    int part_one_valid_count = 0, part_two_valid_count = 0;
    while (getline(file, line)) {
        std::vector<std::string> tokens = split(line, ' ');
        std::vector<std::string> range_string = split(tokens[0], '-');
        char letter = tokens[1][0];
        std::string password = tokens[2];
        if (part_one_valid(password, range_string, letter))
            part_one_valid_count++;
        if (part_two_valid(password, range_string, letter))
            part_two_valid_count++;
    }
    file.close();
    std::cout << part_one_valid_count << std::endl;
    std::cout << part_two_valid_count << std::endl;
    return 0;
}
