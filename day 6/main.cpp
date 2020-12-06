#include <iostream>
#include <fstream>
#include <vector>
#include <map>

typedef struct {
    int amount_of_people = 0;
    std::map<char, int> answer_occurences; 
	// could have been a pair vector since no random individual needs to be accessed?
	// nvm easier to check for duplicates this way
} group;

int sum_group_questions(std::vector<group> groups) {
    int sum = 0;
    for (auto group: groups) {
        sum += group.answer_occurences.size();
    }
    return sum;
}

int sum_yes_questions(std::vector<group> groups) {
    int sum = 0;
    for (auto group: groups) {
        for (auto answer: group.answer_occurences) {
            if (answer.second == group.amount_of_people)
                sum++;
        }
    }
    return sum;
}

int main() {
    std::fstream file;
    file.open("../input.txt", std::fstream::in);
    std::string line;
    std::vector<std::string> group_lines;
    std::vector<group> groups;
    while (getline (file, line)) {
        if (line.empty()) { // put 2 extra blank lines at the end of input
            group current_group;
            current_group.amount_of_people = group_lines.size();
            for (auto person_line: group_lines) {
                for (char question: person_line) {
                    try {
                        current_group.answer_occurences.at(question)++;
                    } catch (std::out_of_range &e) {
                        current_group.answer_occurences.insert({question, 1});
                    }
                }
            }
            groups.push_back(current_group);
            group_lines.clear();
        } else {
            group_lines.push_back(line);
        }
    }
    file.close();
    std::cout << sum_group_questions(groups) << std::endl;
    std::cout << sum_yes_questions(groups) << std::endl;
    return 0;
}
