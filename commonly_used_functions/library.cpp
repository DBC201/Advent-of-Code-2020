#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> split_by_string(std::string str, std::string delimeter) {
    std::vector<std::string> tokens;
    int start_index = str.find(delimeter);
    if (start_index == std::string::npos) {
        tokens.push_back(str);
    } else {
        while (start_index != -1) {
            tokens.push_back(str.substr(0, start_index));
            str = str.substr(start_index+delimeter.length());
            start_index = str.find(delimeter);
        }
        tokens.push_back(str);
    }
    return tokens;
}

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
            i--;
        }
    }
    return str;
}

std::vector<int> string_to_int_vector(std::vector<std::string> v) {
    std::vector<int> to_convert;
    for (auto i: v)
        to_convert.push_back(std::stoi(i));
    return to_convert;
}