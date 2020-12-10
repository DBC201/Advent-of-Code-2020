#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

std::map<int,unsigned long long> cache;
unsigned long long distinct_count(std::vector<int> adapters, int last_index) {
    if (adapters.size() == last_index || adapters[adapters.size()-1]-adapters[last_index] <= 3)
        return 1;
    unsigned long long total_count = 0;
    for (int i=last_index+1; i<adapters.size(); i++) {
        int margin = adapters[i] - adapters[last_index];
        if (margin > 3)
            break;
        try {
            total_count += cache.at(i);
        } catch (std::out_of_range &e) {
            unsigned long long current = distinct_count(adapters, i);
            cache.insert({i, current});
            total_count += current;
        }
    }
    return total_count;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string line;
    std::vector<int> adapters;
    adapters.push_back(0);//starting outlet
    while (getline(file, line)) {
        adapters.push_back(std::stoi(line));
    }
    file.close();
    std::sort(adapters.begin(), adapters.end());
    adapters.push_back(adapters[adapters.size()-1]+3);//finishing outlet
    int one_diff=0, three_diff=0;
    for (int i=1; i<adapters.size(); i++) {
        if (adapters[i]-adapters[i-1] == 1)
            one_diff++;
        else if(adapters[i]-adapters[i-1] == 3)
            three_diff++;
    }
    std::cout << one_diff*three_diff << std::endl;
    std::cout << distinct_count(adapters, 0) << std::endl;
    return 0;
}
