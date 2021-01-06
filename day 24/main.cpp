#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"
#include <chrono>

std::string parse_directions(std::string &directions) {
    int coordinates[2] = {0, 0};
    for (int i=0; i<directions.length(); i++) {
        std::string destination = directions.substr(i, 2);
        if (destination == "ne") {
            coordinates[0]++;
            coordinates[1]++;
            i++;
        } else if (destination == "nw") {
            coordinates[0]--;
            coordinates[1]++;
            i++;
        } else if (destination == "se") {
            coordinates[0]++;
            coordinates[1]--;
            i++;
        } else if (destination == "sw") {
            coordinates[0]--;
            coordinates[1]--;
            i++;
        } else if (destination[0] == 'e') {
            coordinates[0] += 2;
        } else if (destination[0] == 'w') {
            coordinates[0] -= 2;
        } else {
            std::cout << "unknown direction " << destination << std::endl;
        }
    }
    return std::to_string(coordinates[0]) + ',' + std::to_string(coordinates[1]);
}

int count_blacks(std::unordered_map<std::string, bool> &tiles) {
    int count = 0;
    for (auto &p: tiles) {
        if (p.second)
            count++;
    }
    return count;
}

std::vector<std::string> calculate_neighbors(std::string coordinate) {
    std::vector<int> coordinates = string_to_int_vector(split_by_string(coordinate, ","));
    return {
            std::to_string(coordinates[0]+1) + ',' + std::to_string(coordinates[1]+1), //ne
            std::to_string(coordinates[0]+2) + ',' + std::to_string(coordinates[1]),   //e
            std::to_string(coordinates[0]+1) + ',' + std::to_string(coordinates[1]-1), //se
            std::to_string(coordinates[0]-1) + ',' + std::to_string(coordinates[1]-1), //sw
            std::to_string(coordinates[0]-2) + ',' + std::to_string(coordinates[1]),   //w
            std::to_string(coordinates[0]-1) + ',' + std::to_string(coordinates[1]+1)  //nw
    };
}

void flip_for_days(std::unordered_map<std::string, bool> &tiles, int days) {
    for (int i=0; i<days; i++) {
        std::unordered_map<std::string, bool> new_tiles = tiles;
        std::unordered_map<std::string, int> adjacent_whites;
        auto update_whites = [] (std::unordered_map<std::string, int> &adjacent_whites, std::string &coord) {
            try {
                adjacent_whites.at(coord)++;
            } catch (std::out_of_range &e) {
                adjacent_whites.insert({coord, 1});
            }
        };
        for (auto &tile: tiles) {
            if (!tile.second)
                continue;
            int black_neighbor_count = 0;
            for (auto &neighbor_coordinate: calculate_neighbors(tile.first)) {
                try {
                    if (tiles.at(neighbor_coordinate))
                        black_neighbor_count++;
                    else
                        update_whites(adjacent_whites, neighbor_coordinate);
                } catch (std::out_of_range &e) {
                    update_whites(adjacent_whites, neighbor_coordinate);
                }
            }
            if (black_neighbor_count == 0 || black_neighbor_count > 2) {
                try {
                    new_tiles.at(tile.first) = false;
                } catch (std::out_of_range &e) {
                    new_tiles.insert({tile.first, false});
                }
            }
        }

        for (auto &p: adjacent_whites) {
            if (p.second == 2) {
                try {
                    new_tiles.at(p.first) = true;
                } catch (std::out_of_range &e) {
                    new_tiles.insert({p.first, true});
                }
            }
        }
        tiles = new_tiles;
    }
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> begin, end;
    begin = std::chrono::system_clock::now();
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string line;
    std::unordered_map<std::string, bool> tiles; // true means black
    while (getline(file, line)) {
        std::string coordinate = parse_directions(line);
        try {
            bool *temp = &tiles.at(coordinate);
            *temp = !*temp;
        } catch (std::out_of_range &e) {
            tiles.insert({coordinate, true});
        }
    }
    file.close();
    std::cout << count_blacks(tiles) << std::endl;
    flip_for_days(tiles, 100);
    std::cout << count_blacks(tiles) << std::endl;
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> seconds = end - begin;
    std::cout << "Execution time: " << seconds.count() << std::endl;
    return 0;
}
