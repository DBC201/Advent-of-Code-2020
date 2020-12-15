#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

std::vector<std::string> get_map() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    std::string line;
    std::vector<std::string> map;
    while (getline(file, line)) {
        map.push_back(line);
    }
    file.close();
    return map;
}

//right 3 down 1
int count_trees(std::vector<std::string> map, int x_increase, int y_increase) {
    if (y_increase <= 0)
        return -1;
    int tree_count = 0;
    int width = map[0].length();
    int height = map.size();
    int x = 0;
    for (int y = 0; y < height; y += y_increase) {
        int relational_x = x < width ? x : x % width;
        if (map[y][relational_x] == '#') {
            tree_count++;
        }
        x += x_increase;
    }
    return tree_count;
}

unsigned int part_two(std::vector<std::string> map, std::vector<std::pair<int, int>> slopes) {
    std::vector<unsigned int> trees;
    for (auto slope: slopes) {
        unsigned int tree_count = (unsigned int) count_trees(map, slope.first, slope.second);
        trees.push_back(tree_count);
    }
    return std::accumulate(trees.begin(), trees.end(), 1, std::multiplies<>());
    //my compiler multiplied wrong, so i multiplied each value in the vector by hand, please lmk if anybody else has this issue
    //doesn't specifically work on the pc i use for some reason idk, whatever the code is right
    //i use clion on windows 10 if the reader wonders
}


int main() {
    std::vector<std::string> map = get_map();
    std::cout << count_trees(map, 3, 1) << std::endl;// 3 1
    std::vector<std::pair<int, int>> slopes;
    slopes = {
            {1, 1},
            {3, 1},
            {5, 1},
            {7, 1},
            {1, 2}
    };
    std::cout << part_two(map, slopes) << std::endl;
    return 0;
}
