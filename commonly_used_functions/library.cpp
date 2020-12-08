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
