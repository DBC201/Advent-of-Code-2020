#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <unordered_map>
#include "../commonly_used_functions/library.cpp"

class Eqparse {
private:
    const std::unordered_map<std::string, int> precedence_values = {
            {"+", 2},
            {"*", 1}
    };

    static unsigned long long operate(std::string op, unsigned long long a, unsigned long long b) {
        if (op == "+") {
            return a + b;
        } else if (op == "*") {
            return a * b;
        } else {
            return -1;
        }
    }

    unsigned long long evaluate_post_fix(std::queue<std::string> post_fix) {
        std::stack<unsigned long long> results;
        while (!post_fix.empty()) {
            std::string x = post_fix.front();
            post_fix.pop();
            try {
                precedence_values.at(x);
                unsigned long long a = results.top();
                results.pop();
                unsigned long long b = results.top();
                results.pop();
                results.push(operate(x, b, a));
            } catch (std::out_of_range &e) {
                results.push(std::stoull(x));
            }
        }
        if (results.size() > 1)
            return -1;
        else
            return results.top();
    }

    bool is_operator(std::string token) {
        try {
            precedence_values.at(token);
            return true;
        } catch (std::out_of_range &e) {
            return false;
        }
    }

public:
    unsigned long long parse(std::string input) {
        std::queue<std::string> postfix;
        std::stack<std::string> operators;
        for (int i=0; i<input.length(); i++) {
            std::string token;
            token = input[i];
            if (std::isdigit(input[i])) {
                std::string num_string;
                while (isdigit(input[i])) {
                    num_string += input[i];
                    i++;
                }
                i--;
                postfix.push(num_string);
                continue;
            } else if (token == "(") {
                operators.push(token);
            } else if (token == ")") {
                while (operators.top() != "(") {
                    postfix.push(operators.top());
                    operators.pop();
                }
                operators.pop();
            } else if (is_operator(token)) {
                try {
                    while (!operators.empty() && (operators.top() != "(") &&
                           (precedence_values.at(operators.top()) >= precedence_values.at(token))) {
                        postfix.push(operators.top());
                        operators.pop();
                    }
                } catch (std::out_of_range &e) {
                    return -1;
                }
                operators.push(token);
            }
        }

        while (!operators.empty()) {
            postfix.push(operators.top());
            operators.pop();
        }
        return evaluate_post_fix(postfix);
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
    Eqparse eqparse;
    unsigned long long sum = 0;
    while (getline(file, line)) {
        sum += eqparse.parse(line);
    }
    file.close();
    std::cout << sum << std::endl;
    return 0;
}
