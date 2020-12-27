#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<int> lowest_highest(std::vector<int> v) {
    std::sort(v.begin(), v.end());
    return {v[0], v[v.size() - 1]};
}

bool value_in_vector(std::vector<int> v, int n) {
    for (auto m: v) {
        if (m == n)
            return true;
    }
    return false;
}

int find_index(std::vector<int> v, int n) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == n) {
            return i;
        }
    }
    return -1;
}

std::vector<int> remove_indexes(std::vector<int> v, std::vector<int> indexes) {
    for (auto index: indexes) {
        v[index] = -1;
    }
    for (auto it = v.begin(); it != v.end();) {
        if (*it == -1) {
            it = v.erase(it);
        } else {
            it++;
        }
    }
    return v;
}

void print_vector(std::vector<int> v) {
    int start_index = find_index(v, 1) + 1;
    for (int i = start_index; i < start_index + v.size() - 1; i++) {
        std::cout << v[i % v.size()];
    }
    std::cout << std::endl;
}

std::vector<int> play(std::vector<int> cups, int rounds) {
    std::vector<int> min_max = lowest_highest(cups);
    int length = cups.size();
    int current_index = 0;
    for (int i = 0; i < rounds; i++) {
        std::vector<int> removed_cups = {cups[(current_index + 1) % length], cups[(current_index + 2) % length],
                                         cups[(current_index + 3) % length]};
        std::vector<int> current_cups = remove_indexes(cups,
                                                       {(current_index + 1) % length, (current_index + 2) % length,
                                                        (current_index + 3) % length});
        int current_cup = cups[current_index % length];
        int destination_cup = current_cup - 1;
        if (destination_cup < min_max[0])
            destination_cup = min_max[1];
        while (value_in_vector(removed_cups, destination_cup)) {
            destination_cup--;
            if (destination_cup < min_max[0]) {
                destination_cup = min_max[1];
            }
        }
        int destination_index = find_index(current_cups, destination_cup) + 1;
        for (auto removed_cup: removed_cups) {
            if (destination_index > length) {
                destination_index %= length;
            }
            current_cups.insert(current_cups.begin() + destination_index, removed_cup);
            destination_index++;
        }
        current_index = find_index(current_cups, current_cup) + 1;
        cups = current_cups;
    }
    return cups;
}

std::vector<int> return_padded_vector(std::vector<int> &v, int limit) {
    std::vector<int> v1 = v;
    int current_max = lowest_highest(v)[1];
    for (int i = current_max + 1; i <= limit; i++) {
        v1.push_back(i);
    }
    return v1;
}

std::vector<int> convert_to_pointer_list(std::vector<int> v) {
    std::vector<int> pointer_list(v.size() + 1);
    pointer_list[0] = 0;
    for (int i = 0; i < v.size(); i++) {
        if (i+1 == v.size())
            pointer_list[v[i]] = v[0];
        else
            pointer_list[v[i]] = v[i+1];
    }
    return pointer_list;
}

std::vector<int> play_with_pointer_list(std::vector<int> pointer_list, int rounds, int starting_cup) {
    int min = 1;
    int max = pointer_list.size()-1;
    int current_cup = starting_cup;
    for (int i = 0; i < rounds; i++) {
        int first_cup = pointer_list[current_cup];
        int second_cup = pointer_list[first_cup];
        int third_cup = pointer_list[second_cup];
        int destination_cup = current_cup;
        do {
            destination_cup--;
            if (destination_cup < min)
                destination_cup = max;
        } while (value_in_vector({first_cup, second_cup, third_cup}, destination_cup));
        int after_third = pointer_list[destination_cup];
        pointer_list[destination_cup] = first_cup;
        pointer_list[current_cup] = pointer_list[third_cup];
        pointer_list[third_cup] = after_third;
        current_cup = pointer_list[current_cup];
    }
    return pointer_list;
}

unsigned long long multiply_first_two_after_one(std::vector<int> v) {
    unsigned long long first = v[1];
    unsigned long long second = v[first];
    return first * second;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string cup_string;
    getline(file, cup_string);
    file.close();
    std::vector<int> cups(cup_string.length());
    for (int i = 0; i < cup_string.length(); i++) {
        cups[i] = cup_string[i] - '0';
    }
    print_vector(play(cups, 100));
    std::vector<int> pointer_list = convert_to_pointer_list(return_padded_vector(cups, 1000000));
    std::cout << multiply_first_two_after_one(play_with_pointer_list(pointer_list, 10000000, cups[0])) << std::endl;
    return 0;
}
