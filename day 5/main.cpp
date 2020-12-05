#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int seat_id(std::string ticket) {
    int row_lower = 0, row_upper = 127;
    int column_lower = 0, column_upper = 7;
    for (char c: ticket) {
        int row_margin = row_upper - row_lower;
        int column_margin = column_upper - column_lower;
        if (c == 'F')
            row_upper -= row_margin / 2 + 1;
        else if (c == 'B')
            row_lower += row_margin / 2 + 1;
        else if (c == 'L')
            column_upper -= column_margin / 2 + 1;
        else if (c == 'R')
            column_lower += column_margin / 2 + 1;
    }
    if (row_upper != row_lower || column_lower != column_upper)
        return -1;
    else
        return (row_lower*8) + column_lower;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    std::string line;
    std::vector<int> seat_ids;
    while (getline(file, line)) {
        seat_ids.push_back(seat_id(line));
    }
    std::sort(seat_ids.begin(), seat_ids.end());
    std::cout << seat_ids[seat_ids.size()-1] << std::endl;
    for (int i=1; i<seat_ids.size(); i++) {
        if (seat_ids[i]-seat_ids[i-1] != 1) {
            std::cout << seat_ids[i] - 1 << std::endl;
            break;
        }
    }
    return 0;
}
