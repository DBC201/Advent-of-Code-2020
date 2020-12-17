#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"

class Cube {
protected:
    std::vector<int> coordinates;
    bool active;
    std::vector<std::string> neighbor_strings;
    std::vector<std::string> calculate_neighbor_strings() {
        std::vector<std::string> neighbors;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                for (int k = -1; k <= 1; k++) {
                    if (i == 0 && j == 0 && k == 0)
                        continue;
                    neighbors.push_back(
                            convert_coords_to_string({coordinates[0] + i, coordinates[1] + j, coordinates[2] + k}));
                }
            }
        }
        return neighbors;
    }
public:
    Cube(std::vector<int> coordinates, bool active) {
        this->coordinates = coordinates;
        this->active = active;
        this->neighbor_strings = calculate_neighbor_strings();
    }

    Cube(std::string coordinate_str, bool active) {
        coordinates = convert_string_to_coords(coordinate_str);
        this->active = active;
        this->neighbor_strings = calculate_neighbor_strings();
    }

    static std::vector<int> convert_string_to_coords(std::string coordinate_str) {
        return string_to_int_vector(split_by_string(coordinate_str, ","));
    }

    static std::string convert_coords_to_string(std::vector<int> coords) {
        std::string final;
        for (int i = 0; i < coords.size(); i++) {
            if (i == coords.size() - 1)
                final += std::to_string(coords[i]);
            else
                final += std::to_string(coords[i]) + ",";
        }
        return final;
    }

    std::string get_coordinate_string() {
        return convert_coords_to_string(coordinates);
    }

    std::vector<std::string> get_neighbor_strings() {
        return neighbor_strings;
    }

    void change_status() {
        active = !active;
    }

    bool is_active() {
        return active;
    }
};

class Cube4d : public Cube {
public:
    Cube4d(std::vector<int> coordinates, bool active) : Cube(coordinates, active) {
        this->coordinates = coordinates;
        this->active = active;
        this->neighbor_strings=calculate_neighbor_strings();
    }

    Cube4d(std::string coordinate_str, bool active) : Cube(coordinate_str, active) {
        coordinates = convert_string_to_coords(coordinate_str);
        this->active = active;
        this->neighbor_strings=calculate_neighbor_strings();
    }
private:
    std::vector<std::string> calculate_neighbor_strings() {
        std::vector<std::string> neighbors;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                for (int k = -1; k <= 1; k++) {
                    for (int w = -1; w <= 1; w++) {
                        if (i == 0 && j == 0 && k == 0 && w == 0)
                            continue;
                        neighbors.push_back(
                                convert_coords_to_string({coordinates[0] + i, coordinates[1] + j, coordinates[2] + k,
                                                          coordinates[3] + w}));
                    }
                }
            }
        }
        return neighbors;
    }

};

class Space {
private:
    std::unordered_map<std::string, Cube> active_cubes;
    std::unordered_map<std::string, std::vector<std::string>> neighbor_cache;
    void
    activate(Cube cube, std::unordered_map<std::string, Cube> copy, std::unordered_map<std::string, Cube> &visited) {
        if (!cube.is_active())
            return;
        visited.insert({cube.get_coordinate_string(), cube});
        if (!should_be_active(copy, cube))
            active_cubes.erase(cube.get_coordinate_string());
        std::vector<std::string> neighbor_coordinates;
        try {
            neighbor_coordinates = neighbor_cache.at(cube.get_coordinate_string());
        } catch (std::out_of_range &e) {
            neighbor_coordinates = cube.get_neighbor_strings();
            neighbor_cache.insert({cube.get_coordinate_string(), neighbor_coordinates});
        }
        for (auto coord: neighbor_coordinates) {
            try {
                Cube curr = active_cubes.at(coord);
                try {
                    visited.at(curr.get_coordinate_string());
                } catch (std::out_of_range &e) {
                    activate(curr, copy, visited);
                }
            } catch (std::out_of_range &e) {
                Cube curr(coord, false);
                if (should_be_active(copy, curr)) {
                    curr.change_status();
                    active_cubes.insert({curr.get_coordinate_string(), curr});
                }
            }
        }
    }

public:
    void add_cube(std::vector<int> coordinates) {
        Cube cube(coordinates, true);
        active_cubes.insert({cube.get_coordinate_string(), cube});
    }

    static bool should_be_active(std::unordered_map<std::string, Cube> active_cubes, Cube cube) {
        std::vector<std::string> neighbor_coordinates = cube.get_neighbor_strings();
        int active_n_count = 0;
        for (auto coord: neighbor_coordinates) {
            try {
                active_cubes.at(coord);
                active_n_count++;
            } catch (std::out_of_range &e) {

            }
        }
        if (cube.is_active() && (active_n_count != 2 && active_n_count != 3))
            return false;
        else if (!cube.is_active() && active_n_count == 3)
            return true;
        else
            return cube.is_active();
    }

    int return_active_count(int cycle) {
        std::cout << "Calculating 3d for " << cycle << " cycles" << std::endl;
        for (int i = 0; i < cycle; i++) {
            std::unordered_map<std::string, Cube> copy = active_cubes;
            std::unordered_map<std::string, Cube> visited_cache;
            for (auto cube: active_cubes) {
                activate(cube.second, copy, visited_cache);
            }
            std::cout << "Cycle " << i+1 << " active count: " << active_cubes.size() << std::endl;
        }
        return active_cubes.size();
    }
};

class Space4d {
private:
    std::unordered_map<std::string, Cube4d> active_cubes;
    std::unordered_map<std::string, std::vector<std::string>> neighbor_cache;

    void
    activate(Cube4d cube, std::unordered_map<std::string, Cube4d> copy, std::unordered_map<std::string, Cube4d> &visited) {
        if (!cube.is_active())
            return;
        visited.insert({cube.get_coordinate_string(), cube});
        if (!should_be_active(copy, cube))
            active_cubes.erase(cube.get_coordinate_string());
        std::vector<std::string> neighbor_coordinates;
        try {
            neighbor_coordinates = neighbor_cache.at(cube.get_coordinate_string());
        } catch (std::out_of_range &e) {
            neighbor_coordinates = cube.get_neighbor_strings();
            neighbor_cache.insert({cube.get_coordinate_string(), neighbor_coordinates});
        }
        for (auto coord: neighbor_coordinates) {
            try {
                Cube4d curr = active_cubes.at(coord);
                try {
                    visited.at(curr.get_coordinate_string());
                } catch (std::out_of_range &e) {
                    activate(curr, copy, visited);
                }
            } catch (std::out_of_range &e) {
                Cube4d curr(coord, false);
                if (should_be_active(copy, curr)) {
                    curr.change_status();
                    active_cubes.insert({curr.get_coordinate_string(), curr});
                }
            }
        }
    }

public:
    void add_cube(std::vector<int> coordinates) {
        Cube4d cube(coordinates, true);
        active_cubes.insert({cube.get_coordinate_string(), cube});
    }

    static bool should_be_active(std::unordered_map<std::string, Cube4d> active_cubes, Cube4d cube) {
        std::vector<std::string> neighbor_coordinates = cube.get_neighbor_strings();
        int active_n_count = 0;
        for (auto coord: neighbor_coordinates) {
            try {
                active_cubes.at(coord);
                active_n_count++;
            } catch (std::out_of_range &e) {

            }
        }
        if (cube.is_active() && (active_n_count != 2 && active_n_count != 3))
            return false;
        else if (!cube.is_active() && active_n_count == 3)
            return true;
        else
            return cube.is_active();
    }

    int return_active_count(int cycle) {
        std::cout << "Calculating 4d for " << cycle << " cycles" << std::endl;
        for (int i = 0; i < cycle; i++) {
            std::unordered_map<std::string, Cube4d> copy = active_cubes;
            std::unordered_map<std::string, Cube4d> visited_cache;
            for (auto cube: active_cubes) {
                activate(cube.second, copy, visited_cache);
            }
            std::cout << "Cycle " << i+1 << " active count: " << active_cubes.size() << std::endl;
        }
        return active_cubes.size();
    }
};

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string line;
    int y = 0;
    Space space;
    Space4d space4d;
    while (getline(file, line)) {
        for (int x = 0; x < line.length(); x++) {
            if (line[x] == '#') {
                space.add_cube({x, y, 0});
                space4d.add_cube({x, y, 0, 0});
            }
        }
        y++;
    }
    file.close();
    std::cout << space.return_active_count(6) << std::endl;
    std::cout << space4d.return_active_count(6) << std::endl;
    return 0;
}
