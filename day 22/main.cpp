#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <unordered_map>

typedef bool side;

int part1(std::vector<std::queue<int>> decks) {
    while (!decks[0].empty() && !decks[1].empty()) {
        int top1 = decks[0].front(), top2 = decks[1].front();
        decks[0].pop();
        decks[1].pop();
        if (top1 > top2) {
            decks[0].push(top1);
            decks[0].push(top2);
        } else {
            decks[1].push(top2);
            decks[1].push(top1);
        }
    }
    for (auto deck: decks) {
        if (!deck.empty()) {
            int sum = 0;
            int coefficient = deck.size();
            while (!deck.empty()) {
                sum += deck.front()*coefficient;
                deck.pop();
                coefficient--;
            }
            return sum;
        }
    }
    return -1;
}

std::vector<std::queue<int>> get_new_decks(std::vector<std::queue<int>> old_decks, std::vector<int> cards) {
    std::vector<std::queue<int>> new_decks(2);
    for (int i=0; i<cards.size(); i++) {
        for (int j=0; j<cards[i]; j++) {
            new_decks[i].push(old_decks[i].front());
            old_decks[i].pop();
        }
    }
    return new_decks;
}

std::string decks_to_string(std::vector<std::queue<int>> decks) {
    std::string decks_str;
    for (auto deck: decks) {
        while (!deck.empty()) {
            decks_str += std::to_string(deck.front());
            deck.pop();
        }
        decks_str += '|';
    }
    return decks_str;
}

std::unordered_map<std::string, side> deck_cache;
side recursive_combat(std::vector<std::queue<int>> decks) {
    std::unordered_map<std::string, bool> deck_history;
    while (!decks[0].empty() && !decks[1].empty()) {
        std::string decks_string = decks_to_string(decks);
        try {
            if(deck_history.at(decks_string))
                return 0;
        } catch (std::out_of_range &e) {
            deck_history.insert({decks_string, true});
        }
        std::vector<int> cards(2);
        cards[0] = decks[0].front();
        decks[0].pop();
        cards[1] = decks[1].front();
        decks[1].pop();
        side winner;
        if (decks[0].size() >= cards[0] && decks[1].size() >= cards[1]) {
            std::vector<std::queue<int>> new_decks = get_new_decks(decks, cards);
            std::string deck_str = decks_to_string(new_decks);
            try {
                winner = deck_cache.at(deck_str);
            } catch (std::out_of_range &e) {
                winner = recursive_combat(new_decks);
                deck_cache.insert({deck_str, winner});
            }
        } else {
            winner = cards[0] < cards[1];
        }

        if (winner == 0) {
            decks[0].push(cards[0]);
            decks[0].push(cards[1]);
        } else {
            decks[1].push(cards[1]);
            decks[1].push(cards[0]);
        }
    }
    if (decks[0].empty()) {
        return 1;
    } else {
        return 0;
    }
}

int part2(std::vector<std::queue<int>> decks) {
    std::unordered_map<std::string, bool> deck_history;
    while (!decks[0].empty() && !decks[1].empty()) {
        std::string decks_string = decks_to_string(decks);
        try {
            if(deck_history.at(decks_string)) {
                decks[1] = std::queue<int>();
                break;
            }
        } catch (std::out_of_range &e) {
            deck_history.insert({decks_string, true});
        }
        std::vector<int> cards(2);
        cards[0] = decks[0].front();
        decks[0].pop();
        cards[1] = decks[1].front();
        decks[1].pop();
        side winner;
        if (decks[0].size() >= cards[0] && decks[1].size() >= cards[1]) {
            winner = recursive_combat(get_new_decks(decks, cards));
        } else {
            winner = cards[0] < cards[1];
        }

        if (winner == 0) {
            decks[0].push(cards[0]);
            decks[0].push(cards[1]);
        } else {
            decks[1].push(cards[1]);
            decks[1].push(cards[0]);
        }
    }
    for (auto deck: decks) {
        if (!deck.empty()) {
            int sum = 0;
            int coefficient = deck.size();
            while (!deck.empty()) {
                sum += deck.front()*coefficient;
                deck.pop();
                coefficient--;
            }
            return sum;
        }
    }
    return -1;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::vector<std::queue<int>> decks(2);
    std::string line;
    getline(file, line); // Player 1
    int deck_index = 0;
    while (getline(file, line)) {
        if (line.empty()) {
            getline(file, line);
            deck_index++;
        } else {
            decks[deck_index].push(std::stoi(line));
        }
    }
    file.close();
    std::cout << part1(decks) << std::endl;
    std::cout << part2(decks) << std::endl;
    return 0;
}
