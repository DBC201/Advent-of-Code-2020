#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

std::vector<std::string> split(std::string str, char delimeter){
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, delimeter)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

class Regulations{
private:
    std::map<std::string, std::map<std::string, int>> contain_rules;
    std::string clean(std::string str) {
        if (str[0] == ' '){
            str.erase(str.begin());
        }
        if (str[str.length()-1] == '.') {
            str.erase(str.begin() + str.length()-1);
        }
        return str;
    }
    std::map<std::string, int> parse_holdable_bags(std::string holdable_string) {
        std::vector<std::string> raw_bags(split(holdable_string, ','));
        std::map<std::string, int> bags;
        for (auto bag_string: raw_bags) {
            bag_string = clean(bag_string);
            if (bag_string == "no other bags") {
                bags.insert({bag_string, 0});
                return bags;
            }
            std::string bag_name = bag_string.substr(2);
            try {
                int bag_count = std::stoi(bag_string.substr(0, 1));
                if (bag_count > 1) {
                    bag_name = bag_name.substr(0, bag_name.length()-1);
                }
                bags.insert({bag_name, bag_count});
            } catch (std::invalid_argument &e) {
                std::cout << "Invalid bag count at first character: " << bag_string << std::endl;
            }
        }
        return bags;
    }

    bool can_hold(std::string bag, std::string desired_bag) {
        if (bag == "no other bags")
            return false;

        try {
            for (auto holdable: contain_rules.at(bag)) {
                if (holdable.first == desired_bag || can_hold(holdable.first, desired_bag))
                    return true;
            }
        } catch (std::out_of_range &e) {
            return false;
        }
        return false;
    }
public:
    void add_regulation(std::string regulation) {
        int contain_index = regulation.find("contain");
        std::string color = regulation.substr(0, contain_index-2);
        std::string holdable_string = regulation.substr(contain_index+8);
        std::map<std::string, int> holdable_bags = parse_holdable_bags(holdable_string);
        contain_rules.insert({color, holdable_bags});
    }

    int total_valid_bags(std::string color) {
        int total = 0;
        for (auto bag: contain_rules){
            if (can_hold(bag.first, color)) {
                total++;
            }
        }
        return total;
    }

    int bag_capacity(std::string color) {
        if (color == "no other bags") {
            return 0;
        }

        try {
            contain_rules.at(color).at("no other bags");
            return 1;
        } catch (std::out_of_range &e) {
            ;
        }

        int sum = 0;
        try {
            for (auto bag: contain_rules.at(color)) {
                sum += bag.second * bag_capacity(bag.first);
            }
        } catch (std::out_of_range &e) {
            return 0;
        }
        return sum+1; //this will also count the initial bag
    }
};

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    std::string line;
    Regulations regulations;
    while (getline(file, line)) {
        regulations.add_regulation(line);
    }
    file.close();
    std::cout << regulations.total_valid_bags("shiny gold bag") << std::endl;
    std::cout << regulations.bag_capacity("shiny gold bag") - 1 << std::endl; // the bag itself doesn't count
    return 0;
}
