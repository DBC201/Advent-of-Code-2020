#include <iostream>
#include <fstream>
#include <vector>

int visible_occupied_count(std::vector<std::string> seats, int y, int x, int range) {
    std::vector<std::pair<int, int>> corners = {
            {-1, -1},
            {0,  -1},
            {1,  -1},
            {-1, 0},
            {1,  0},
            {-1, 1},
            {0,  1},
            {1,  1}
    };
    int count = 0;
    for (int i=1; i<=range; i++) {
        std::vector<std::pair<int, int>> visible_corners;
        for (auto corner: corners) {
            int corner_y = y + corner.second*i;
            int corner_x = x + corner.first*i;
            if (corner_y < 0 || corner_y >= seats.size() || corner_x < 0 || corner_x >= seats[y].length())
                continue;
            else if (seats[corner_y][corner_x] == '.')
                visible_corners.push_back(corner);
            else if (seats[corner_y][corner_x] == '#')
                count++;
        }
        corners = visible_corners;
        if (corners.empty())
            break;
    }
    return count;
}

std::vector<std::string> systematic_seats_one(std::vector<std::string> seats) {
    std::vector<std::string> previous_seats = seats;
    while (true) {
        std::vector<std::string> copy_seats = seats;
        for (int i = 0; i < copy_seats.size(); i++) {
            for (int j = 0; j < copy_seats[i].size(); j++) {
                if (copy_seats[i][j] == '.')
                    continue;
                int count = visible_occupied_count(copy_seats, i, j, 1);
                if (copy_seats[i][j] == 'L' && count == 0)
                    seats[i][j] = '#';
                else if (copy_seats[i][j] == '#' && count >= 4)
                    seats[i][j] = 'L';
            }
        }
        if (seats == previous_seats)
            return seats;
        previous_seats = seats;
    }
}

std::vector<std::string> systematic_seats_two(std::vector<std::string> seats) {
    std::vector<std::string> previous_seats = seats;
    while (true) {
        std::vector<std::string> copy_seats = seats;
        for (int i = 0; i < copy_seats.size(); i++) {
            for (int j = 0; j < copy_seats[i].size(); j++) {
                if (copy_seats[i][j] == '.')
                    continue;
                int count = visible_occupied_count(copy_seats, i, j, copy_seats.size());
                if (copy_seats[i][j] == 'L' && count == 0)
                    seats[i][j] = '#';
                else if (copy_seats[i][j] == '#' && count >= 5)
                    seats[i][j] = 'L';
            }
        }
        if (seats == previous_seats)
            return seats;
        previous_seats = seats;
    }
}

int return_occupied_count(std::vector<std::string> seats) {
    int count = 0;
    for (auto row: seats) {
        for (auto seat: row) {
            if (seat == '#')
                count++;
        }
    }
    return count;
}

int main() {
    std::vector<std::string> seats;
    std::string line;
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    while (getline(file, line)) {
        seats.push_back(line);
    }
    file.close();
    std::cout << return_occupied_count(systematic_seats_one(seats)) << std::endl;
    std::cout << return_occupied_count(systematic_seats_two(seats)) << std::endl;
    return 0;
}
