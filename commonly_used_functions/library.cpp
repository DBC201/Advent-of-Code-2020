#include <iostream>
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

std::string remove_spaces(std::string str) {
    for (int i=0; i<str.length(); i++) {
        if (str[i] == ' ') {
            str = str.substr(0,i) + str.substr(i+1);
        }
    }
    return str;
}
