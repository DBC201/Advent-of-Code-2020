#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"
#include <algorithm>

class AllergenSolver{
private:
    std::unordered_map<std::string, std::string> ingredient_matches;
    std::unordered_map<std::string, int> occurence_counts;
    std::unordered_map<std::string, std::unordered_map<std::string, bool>> possible_matches;

    static std::vector<std::string> split_allergens(std::string allergens) {
        return split_by_string(allergens, ", ");
    }

    void process_allergen_ingredients(std::string allergen, std::unordered_map<std::string, bool> possibilities) {
        try {
            for (auto it=possible_matches.at(allergen).begin(); it!=possible_matches.at(allergen).end();) {
                try {
                    possibilities.at(it->first);
                    it++;
                } catch (std::out_of_range &e) {
                    it = possible_matches.at(allergen).erase(it);
                }
            }
        } catch (std::out_of_range &e) {
            possible_matches.insert({allergen, possibilities});
        }
    }

    void reduce_possible_matches() {
        while (1) {
            bool no_change = true;
            for (auto it=possible_matches.begin(); it!=possible_matches.end(); it++) {
                if (it->second.size() == 1) {
                    std::string ingredient;
                    for (auto ing: it->second) {
                        ingredient = ing.first;
                    }
                    for (auto it2=possible_matches.begin(); it2!=possible_matches.end(); it2++) {
                        if (it != it2) {
                            try {
                                it2->second.at(ingredient);
                                it2->second.erase(ingredient);
                                no_change = false;
                            } catch (std::out_of_range &e) {

                            }
                        }
                    }
                    ingredient_matches.insert({ingredient, it->first});
                }
            }

            if (no_change)
                return;
        }

    }

public:
    AllergenSolver(std::vector<std::string> list) {
        for (auto line: list) {
            std::vector<std::string> ingredients = split_by_string(line.substr(0, line.find("(") - 1), " ");
            std::string allergen_string = line.substr(line.find("contains") + 8 + 1,
                                                      line.length() - line.find("contains") - 10);
            std::vector<std::string> allergens = split_allergens(allergen_string);

            for (auto ingredient: ingredients) { //count occurences seperately
                try {
                    occurence_counts.at(ingredient)++;
                } catch (std::out_of_range &e) {
                    occurence_counts.insert({ingredient, 1});
                }
            }

            for (auto allergen: allergens) {
                std::unordered_map<std::string, bool> allergen_possibilities;
                for (auto ingredient: ingredients) {
                    allergen_possibilities.insert({ingredient, true});
                }
                process_allergen_ingredients(allergen, allergen_possibilities);
            }

        }
        reduce_possible_matches();
    }

    std::unordered_map<std::string, std::string> get_matches() {
        return ingredient_matches;
    }

    int get_unmatching_count() {
        int count = 0;
        for (auto occurence: occurence_counts) {
            try {
                ingredient_matches.at(occurence.first);
            } catch (std::out_of_range &e) {
                count += occurence.second;
            }
        }
        return count;
    }
};

bool compare_ingredients(std::pair<std::string, std::string> a, std::pair<std::string, std::string> b) {
    return a.second < b.second;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    if (!file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    std::string line;
    std::vector<std::string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    AllergenSolver allergenSolver(lines);
    std::cout << allergenSolver.get_unmatching_count() << std::endl;
    std::vector<std::pair<std::string, std::string>> matches;
    for (auto match: allergenSolver.get_matches()) {
        matches.push_back(match);
    }
    std::sort(matches.begin(), matches.end(), compare_ingredients);
    for (int i=0; i<matches.size(); i++) {
        std::cout << matches[i].first;
        if (i + 1 != matches.size())
            std::cout << ",";
    }
    std::cout << std::endl;
}
