#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

bool has_2_sum(std::vector<unsigned long long> numbers, int next_num_index, int preamble_length) {
    int begin = next_num_index-preamble_length;
    for (int i=begin; i<next_num_index; i++) {
        for (int j=begin; j<next_num_index; j++) {
            if (numbers[i] + numbers[j] == numbers[next_num_index]){
                return true;
            }
        }
    }
    return false;
}

std::vector<unsigned long long> get_sum_set(std::vector<unsigned long long> numbers, int index) {
    std::vector<unsigned long long> sum_values;
    for (int i=0; i<numbers.size(); i++) {
        if (i == index)
            continue;
        sum_values.push_back(numbers[i]);
        unsigned long long current_sum;
        for (int j=i+1; j<numbers.size(); j++) {
            sum_values.push_back(numbers[j]);
            current_sum = std::accumulate(sum_values.begin(), sum_values.end(), 0);
            if (current_sum > numbers[index] || j == index)
                break;
            else if (current_sum == numbers[index])
                return sum_values;
        }
        sum_values.clear();
    }
    return sum_values;
}

int main() {
    std::fstream file;
    std::vector<unsigned long long> numbers;
    std::string line;
    file.open("../input.txt", std::fstream::in);
    while (getline(file, line)){
        numbers.push_back(std::stoull(line));
    }
    file.close();
    int preamble_length = 25;
    for (int i=preamble_length; i<numbers.size(); i++) {
        if (!has_2_sum(numbers, i, preamble_length)){
            unsigned long long weakness_number = numbers[i];
            std::cout << weakness_number << std::endl;
            std::vector<unsigned long long> sum_set = get_sum_set(numbers, i);
            std::sort(sum_set.begin(), sum_set.end());
            std::cout << sum_set[0]+sum_set[sum_set.size()-1] << std::endl;
            break;
        }
    }
    return 0;
}
