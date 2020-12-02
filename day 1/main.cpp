#include <iostream>
#include <fstream>
#include <vector>

std::vector<int> get_entries() {
    std::vector<int> return_val;
    std::string line;
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    while (std::getline(file, line)) {
        return_val.push_back(std::stoi(line));
    }
    file.close();
    return return_val;
}

int return_multiple_of_sum(std::vector<int> entries) {
    for (int i=0; i<entries.size(); i++){
        for (int j=0; j<entries.size(); j++){
            if (i == j) {
                continue;
            } else if (entries[i]+entries[j] == 2020) {
                return entries[i] * entries[j];
            }
        }
    }
    return -1;
}

int return_multiple_of_sum2(std::vector<int> entries) {
    int size = entries.size();
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (i == j)
                continue;
            for (int k=0; k<size; k++) {
                if (i==k || j==k)
                    continue;
                if (entries[i]+entries[j]+entries[k] == 2020) {
                    return entries[i] * entries[j] * entries[k];
                }
            }
        }
    }
    return -1;
}

int main() {
    std::vector<int> entries = get_entries();
    std::cout << return_multiple_of_sum(entries) << std::endl;
    std::cout << return_multiple_of_sum2(entries) << std::endl;
    return 0;
}
