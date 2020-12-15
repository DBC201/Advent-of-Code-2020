#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"

int return_index(std::unordered_map<int, int> numbers, int count, int last_num) {
    for (int i=numbers.size()+1; i<=count; i++) {
        try {
            int last_prev = numbers.at(last_num);
            int new_last = i - 1 - last_prev;
            numbers.at(last_num) = i-1;
            last_num = new_last;
        } catch (std::out_of_range &e) {
            numbers.insert({last_num, i-1});
            last_num = 0;
        }
    }
    return last_num;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string line;
    getline(file, line);
    std::unordered_map<int, int> numbers;
    std::vector<std::string> num_strings = split(line, ',');
    int last_num;
    for (int i=0; i<num_strings.size(); i++) {
        last_num = std::stoi(num_strings[i]);
        numbers.insert({last_num, i+1});
    }
    std::cout << return_index(numbers, 2020, last_num) << std::endl;
    std::cout << return_index(numbers, 30000000, last_num) << std::endl;
    return 0;
}
