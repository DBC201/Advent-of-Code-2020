#include <iostream>
#include <fstream>
#include <vector>
#include "../commonly_used_functions/library.cpp"

std::vector<unsigned int> parse_bus_times_one(std::string raw) {
    std::vector<std::string> raw_times(split(raw,','));
    std::vector<unsigned int> bus_times;
    for (auto time: raw_times) {
        if (time == "x")
            continue;
        bus_times.push_back(std::stoi(time));
    }
    return bus_times;
}

unsigned int return_wait_time(std::vector<unsigned int> bus_times, unsigned int time) {
    unsigned int smallest = -1;
    unsigned int earliest_id;
    for (auto bus_time: bus_times) {
        unsigned int current_id = bus_time;
        while (bus_time < time) {
            bus_time += current_id;
        }
        if (smallest == -1){
            smallest = bus_time;
            earliest_id = current_id;
        }
        else if (bus_time < smallest){
            smallest = bus_time;
            earliest_id = current_id;
        }
    }
    return (smallest-time)*earliest_id;
}

std::vector<std::pair<unsigned long long, unsigned long long>> parse_bus_times_two(std::string raw) {
    std::vector<std::string> raw_times(split(raw,','));
    std::vector<std::pair<unsigned long long, unsigned long long>>  bus_times;
    for (int i=0; i<raw_times.size(); i++) {
        if (raw_times[i] == "x")
            continue;
        else
            bus_times.push_back({std::stoull(raw_times[i]), i});
    }
    return bus_times;
}

unsigned long long crt_timestamp(std::vector<std::pair<unsigned long long , unsigned long long>> times) {
    unsigned long long scm = 1;
    unsigned long long timestamp = 0;
    for (auto time: times) {
        scm *= time.first;
    }
    auto modulo_inverse = [=] (unsigned long long a, unsigned long long b) {
        a = a % b;
        for (int x = 1; x < b; x++)
            if ((a * x) % b == 1)
                return x;
    };
    for (auto time: times) {
        unsigned long long scm_by_num = scm/time.first;
        timestamp += (time.first-time.second)*scm_by_num*modulo_inverse(scm_by_num, time.first);
    }
    return timestamp % scm;
}

unsigned long long naive_timestamp(std::vector<std::pair<unsigned long long, unsigned long long>> times) {
    unsigned long long first = times[0].first;
    while (1) {
        bool valid = true;
        for (auto pair: times) {
            if ((first + pair.second)%pair.first != 0) {
                valid = false;
                break;
            }
        }
        if (valid)
            return first;
        first += times[0].first;
    }
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
    unsigned int earliest_time = std::stoi(line);
    getline(file, line);
    file.close();
    std::vector<unsigned int> bus_times = parse_bus_times_one(line);
    std::cout << return_wait_time(bus_times, earliest_time) << std::endl;
    //std::cout << naive_timestamp(parse_bus_times_two(line)) << std::endl;
    std::cout << crt_timestamp(parse_bus_times_two(line)) << std::endl;
    return 0;
}
