#include <iostream>
#include <fstream>

class Ship {
private:
    int x;
    int y;
    int angle;
    int waypoint[2];

    void rotate_waypoint(int angle) {
        angle = simplify_angle(angle);
        int repetitions = angle / 90;
        for (int i = 0; i < repetitions; i++) {
            int new_x = -1 * waypoint[1];
            int new_y = waypoint[0];
            waypoint[0] = new_x;
            waypoint[1] = new_y;
        }
    }

    bool has_waypoint() {
        return waypoint[0] != 0 || waypoint[1] != 0;
    }

    void move_forward(int distance) {
        if (has_waypoint()) {
            x += waypoint[0] * distance;
            y += waypoint[1] * distance;
        } else {
            switch (angle) {
                case 0:
                    move_east(distance);
                    break;

                case 90:
                    move_north(distance);
                    break;

                case 180:
                    move_west(distance);
                    break;

                case 270:
                    move_south(distance);
                    break;
            }
        }
    }

    void move_north(int distance) {
        y += distance;
    }

    void move_south(int distance) {
        y -= distance;
    }

    void move_east(int distance) {
        x += distance;
    }

    void move_west(int distance) {
        x -= distance;
    }

    static int simplify_angle(int angle) {
        while (angle <= -360)
            angle += 360;
        while (angle >= 360)
            angle -= 360;
        if (angle < 0)
            return 360 + angle;
        else
            return angle;
    }

    void parse_instructions(std::string line) {
        int value = std::stoi(line.substr(1));
        if (line[0] == 'N')
            move_north(value);
        else if (line[0] == 'S')
            move_south(value);
        else if (line[0] == 'E')
            move_east(value);
        else if (line[0] == 'W')
            move_west(value);
        else if (line[0] == 'L')
            angle += value;
        else if (line[0] == 'R')
            angle -= value;
        else if (line[0] == 'F')
            move_forward(value);
        angle = simplify_angle(angle);
    }

    void parse_waypoint_instructions(std::string line) {
        int value = std::stoi(line.substr(1));
        if (line[0] == 'N')
            waypoint[1] += value;
        else if (line[0] == 'S')
            waypoint[1] -= value;
        else if (line[0] == 'E')
            waypoint[0] += value;
        else if (line[0] == 'W')
            waypoint[0] -= value;
        else if (line[0] == 'L')
            rotate_waypoint(value);
        else if (line[0] == 'R')
            rotate_waypoint(-1*value);
        else if (line[0] == 'F')
            move_forward(value);
    }

public:
    Ship(int waypoint_x, int waypoint_y) {
        x = 0;
        y = 0;
        angle = 0; // east is 0
        waypoint[0] = waypoint_x;
        waypoint[1] = waypoint_y;
    }

    void parse(std::string line) {
        if (has_waypoint())
            parse_waypoint_instructions(line);
        else
            parse_instructions(line);
    }

    int manhattan_distance() {
        return std::abs(x) + std::abs(y);
    }
};

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "Unable to open file!" << std::endl;
        return -1;
    }
    std::string line;
    Ship ship(0, 0);
    Ship ship2(10, 1);
    while (getline(file, line)) {
        ship.parse(line);
        ship2.parse(line);
    }
    file.close();
    std::cout << ship.manhattan_distance() << std::endl;
    std::cout << ship2.manhattan_distance() << std::endl;
    return 0;
}
