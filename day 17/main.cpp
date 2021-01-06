#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"
#include <chrono>

class Space3d{
private:
    std::unordered_map<std::string, bool> cubes;
public:
    void add_cube(std::vector<int> coordinates) {
        cubes.insert({convert_coord_vector_to_string(coordinates), true});
    }

    static std::vector<int> convert_coord_string_to_vector(std::string coordinate_str) {
        return string_to_int_vector(split_by_string(coordinate_str, ","));
    }

    static std::string convert_coord_vector_to_string(std::vector<int> coords) {
        std::string final;
        for (int i = 0; i < coords.size(); i++) {
            if (i == coords.size() - 1)
                final += std::to_string(coords[i]);
            else
                final += std::to_string(coords[i]) + ",";
        }
        return final;
    }

    int active_count() {
        int count = 0;
        for (auto &cube: cubes) {
            if (cube.second)
                count++;
        }
        return count;
    }

    virtual std::vector<std::string> calculate_neighbors(std::string coordinate) {
        std::vector<int> coordinates = convert_coord_string_to_vector(coordinate);
        std::vector<std::string> neighbors;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                for (int k = -1; k <= 1; k++) {
                    if (i == 0 && j == 0 && k == 0)
                        continue;
                    neighbors.push_back(
                            convert_coord_vector_to_string({coordinates[0] + i, coordinates[1] + j, coordinates[2] + k}));
                }
            }
        }
        return neighbors;
    }

    void simulate_cycles(int cycles) {
        for (int i=0; i<cycles; i++) {
            std::unordered_map<std::string, bool> new_cubes = cubes;
            std::unordered_map<std::string, int> inactives_active_neighbors;
            auto add_to_inactive = [] (std::unordered_map<std::string, int> &inactives, std::string &coordinate) {
                try {
                    inactives.at(coordinate)++;
                } catch (std::out_of_range &e) {
                    inactives.insert({coordinate, 1});
                }
            };
            for (auto &cube: cubes) {
                if (!cube.second)
                    continue;
                int active_neighbor_count = 0;
                std::vector<std::string> neighbors = calculate_neighbors(cube.first);
                for (auto &neighbor: neighbors) {
                    try {
                        if (cubes.at(neighbor))
                            active_neighbor_count++;
                        else
                            add_to_inactive(inactives_active_neighbors, neighbor);
                    } catch (std::out_of_range &e) {
                        add_to_inactive(inactives_active_neighbors, neighbor);
                    }
                }

                if (active_neighbor_count != 2 && active_neighbor_count != 3)
                    new_cubes.at(cube.first) = false;
            }

            for (auto &inactive: inactives_active_neighbors){
                if (inactive.second == 3) {
                    try {
                        new_cubes.at(inactive.first) = true;
                    } catch (std::out_of_range &e) {
                        new_cubes.insert({inactive.first, true});
                    }
                }
            }
            cubes = new_cubes;
        }
    }
};

class Space4d: public Space3d {
    std::vector<std::string> calculate_neighbors(std::string coordinate) override {
        std::vector<int> coordinates = convert_coord_string_to_vector(coordinate);
        std::vector<std::string> neighbors;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                for (int k = -1; k <= 1; k++) {
                    for (int w = -1; w <= 1; w++) {
                        if (i == 0 && j == 0 && k == 0 && w == 0)
                            continue;
                        neighbors.push_back(
                                convert_coord_vector_to_string({coordinates[0] + i, coordinates[1] + j, coordinates[2] + k, coordinates[3] + w}));
                    }
                }
            }
        }
        return neighbors;
    }
};

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
    int y = 0;
    Space3d space3d;
    Space4d space4D;
    while (getline(file, line)) {
        for (int x = 0; x < line.length(); x++) {
            if (line[x] == '#') {
                space3d.add_cube({x, y, 0});
                space4D.add_cube({x, y, 0, 0});
            }
        }
        y++;
    }
    file.close();
    space3d.simulate_cycles(6);
    space4D.simulate_cycles(6);
    std::cout << space3d.active_count() << std::endl;
    std::cout << space4D.active_count() << std::endl;
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> seconds = end-begin;
    std::cout << "Execution time: " << seconds.count() << " seconds" << std::endl;
    return 0;
}
