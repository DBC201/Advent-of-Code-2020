#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"
#include <set>

class Tile {
private:
    std::string id;
    std::vector<std::string> raw_data;
    std::vector<Tile *> neighbors;

public:
    Tile(std::string id, std::vector<std::string> &raw_data) {
        this->id = id;
        this->raw_data = raw_data;
        neighbors = {
                nullptr,
                nullptr,
                nullptr,
                nullptr
        };
    }

    std::string get_upper_side() {
        return raw_data[0];
    }

    std::string get_lower_side() {
        return raw_data[raw_data.size() - 1];
    }

    std::string get_left_side() {
        std::string side;
        for (int i = 0; i < raw_data.size(); i++) {
            side += raw_data[i][0];
        }
        return side;
    }

    std::string get_right_side() {
        std::string side;
        for (int i = 0; i < raw_data.size(); i++) {
            side += raw_data[i][raw_data[i].length() - 1];
        }
        return side;
    }

    std::string get_id() {
        return id;
    }

    void flip_90() {
        std::vector<std::string> rows = raw_data;
        for (int j = 0; j < raw_data[0].length(); j++) {
            std::string temp = flip(rows[j]);
            for (int i = 0; i < raw_data.size(); i++) {
                raw_data[i][j] = temp[i];
            }
        }
    }

    void flip_by_x() {
        for (int j = 0; j < raw_data[0].length(); j++) {
            for (int i = 0; i < raw_data.size() / 2; i++) {
                char temp = raw_data[i][j];
                raw_data[i][j] = raw_data[raw_data.size() - i - 1][j];
                raw_data[raw_data.size() - i - 1][j] = temp;
            }
        }
        Tile *temp = neighbors[0];
        neighbors[0] = neighbors[1];
        neighbors[1] = temp;
    }

    void flip_by_y() {
        for (auto &s: raw_data) {
            s = flip(s);
        }
        Tile *temp = neighbors[2];
        neighbors[2] = neighbors[3];
        neighbors[3] = temp;
    }

    void add_neighbor(Tile *tile, int direction_index) {
        neighbors[direction_index] = tile;
    }

    int get_side_index(std::string side) {
        if (side == get_upper_side())
            return 0;
        else if (side == get_lower_side())
            return 1;
        else if (side == get_left_side())
            return 2;
        else if (side == get_right_side())
            return 3;
        else
            return -1;
    }

    std::string get_side(int index) {
        if (index == 0)
            return get_upper_side();
        else if (index == 1)
            return get_lower_side();
        else if (index == 2)
            return get_left_side();
        else if (index == 3)
            return get_right_side();
        else
            return "";
    }

    Tile *get_neighbor(int index) {
        return neighbors[index];
    }

    static int opposite_index(int index) {
        if (index % 2 == 0)
            return index + 1;
        else
            return index - 1;
    }

    void print() {
        std::cout << std::endl;
        for (auto &s: raw_data) {
            std::cout << s << std::endl;
        }
        std::cout << std::endl;
    }
};

class Image {
private:
    std::unordered_map<std::string, std::set<std::string>> side_matches;
    std::unordered_map<std::string, Tile *> tiles;

    void add_side(std::string &id, std::string &side) {
        try {
            side_matches.at(side).insert(id);
        } catch (std::out_of_range &e) {
            side_matches.insert({side, {id}});
        }
    }

    void
    recursive_link(Tile *previous, std::string side, std::unordered_map<std::string, bool> &visited_cache) {
        try {
            if (visited_cache.at(side))
                return;
        } catch (std::out_of_range &e) {
            visited_cache.insert({side, true});
        }
        try {
            std::set<std::string> matching_ids = side_matches.at(side);
            if (matching_ids.size() != 2) // edges
                return;
            Tile *match = nullptr;
            for (auto &id: matching_ids) {
                try {
                    Tile *curr = tiles.at(id);
                    if (curr != previous) {
                        match = curr;
                        break;
                    }
                } catch (std::out_of_range &e) {

                }
            }
            if (match == nullptr)
                return;
            bool side_flipped = (previous->get_side_index(side) == -1);
            int prev_index;
            if (side_flipped)
                prev_index = previous->get_side_index(flip(side));
            else
                prev_index = previous->get_side_index(side);
            bool equal_index = (prev_index == match->get_side_index(side));
            if (!side_flipped && equal_index) {
                if (prev_index < 2)
                    match->flip_by_x();
                else
                    match->flip_by_y();
            } else if (side_flipped && equal_index) {
                match->flip_by_x();
                match->flip_by_y();
            } else if (!side_flipped) {
                bool is_opposite = (Tile::opposite_index(prev_index) == match->get_side_index(side));
                int pre_flip_index = match->get_side_index(side);
                if (!is_opposite) {
                    match->flip_90();
                    if (pre_flip_index < 2)
                        match->flip_by_x();
                    else
                        match->flip_by_y();
                }
            } else {
                bool is_opposite = (Tile::opposite_index(prev_index) == match->get_side_index(side));
                if (is_opposite) {
                    if (match->get_side_index(side) < 2)
                        match->flip_by_y();
                    else
                        match->flip_by_x();
                } else
                    match->flip_90();
            }
            if (side_flipped)
                side = flip(side);
            previous -> add_neighbor(match, previous->get_side_index(side));
            match -> add_neighbor(previous, match->get_side_index(side));
            for (int i = 0; i < 4; i++) {
                std::string match_side = match->get_side(i);
                recursive_link(match, match_side, visited_cache);
                recursive_link(match, flip(match_side), visited_cache);
            }
        } catch (std::out_of_range &e) {
            return;
        }
    }

public:
    void add_tile(std::string &id, std::vector<std::string> &data, std::vector<std::string> sides) {
        Tile *current = new Tile(id, data);
        tiles.insert({id, current});
        for (auto &side: sides) {
            add_side(id, side);
        }
    }

    void match_sides() {
        for (auto it = side_matches.begin(); it != side_matches.end();) {
            if (it->second.size() != 2) {
                try {
                    side_matches.at(flip(it->first)).insert(*it->second.begin());
                    it = side_matches.erase(it);
                    continue;
                } catch (std::out_of_range &e) {

                }
            }
            it++;
        }
    };

    unsigned long long multiply_corner_ids() {
        unsigned long long val = 1;
        std::unordered_map<std::string, int> side_tiles;
        for (auto &side: side_matches) {
            if (side.second.size() != 2) {
                try {
                    side_tiles.at(*side.second.begin())++;
                } catch (std::out_of_range &e) {
                    side_tiles.insert({*side.second.begin(), 1});
                }
            }
        }

        for (auto &tile: side_tiles) {
            if (tile.second == 2) {
                val *= std::stoull(tile.first);
            }
        }
        return val;
    }

    void match_tiles() {
        Tile *tile = tiles.begin()->second;
        std::unordered_map<std::string, bool> visited_cache;
        for (int i = 0; i < 4; i++) {
            std::string tile_side = tile->get_side(i);
            recursive_link(tile, tile_side, visited_cache);
            recursive_link(tile, flip(tile_side), visited_cache);
        }
    }

    void print() {
        for (auto &tile: tiles) {
            std::cout << tile.first << std::endl;
            for (int i = 0; i < 4; i++) {
                Tile *curr = tile.second->get_neighbor(i);
                if (curr == nullptr)
                    std::cout << "\tNone" << std::endl;
                else
                    std::cout << "\t" << curr->get_id() << std::endl;
            }
            /*if (tile.second->get_neighbor(0) == nullptr && tile.second->get_neighbor(2) == nullptr) {
                Tile *row_tile = tile.second;
                while (row_tile != nullptr) {
                    Tile *column_tile = row_tile;
                    while (column_tile != nullptr) {
                        std::cout << column_tile ->get_id() << " ";
                        column_tile = column_tile->get_neighbor(3);
                    }
                    std::cout << std::endl;
                    row_tile = row_tile->get_neighbor(1);
                }
            }*/
        }
    }
};

std::string parse_current_id(std::string raw) {
    std::string id = split_by_string(raw, " ")[1];
    return id.substr(0, id.length() - 1);
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
    std::string current_id = parse_current_id(line);
    std::string upper, left, right, previous_line;
    std::vector<std::string> data;
    std::vector<std::vector<std::string>> datas;
    Image image;
    bool is_upper = true;
    while (getline(file, line)) {
        if (line.empty()) { // 2 empty lines at the end of input
            image.add_tile(current_id, data, {upper, previous_line, left, right});
            datas.push_back(data);
            data.clear();
            upper.clear();
            left.clear();
            right.clear();
            getline(file, line);
            if (line.empty())
                break;
            current_id = parse_current_id(line);
            is_upper = true;
        } else {
            if (is_upper) {
                upper = line;
                is_upper = false;
            }
            left += line[0];
            right += line[line.length() - 1];
            data.push_back(line);
            previous_line = line;
        }
    }
    file.close();
    image.match_sides();
    std::cout << image.multiply_corner_ids() << std::endl;
    //image.match_tiles();
    //image.print();
    //this is where i give up... day 20 part 2 failed
    return 0;
}
