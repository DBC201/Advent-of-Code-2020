#include <iostream>
#include <fstream>
#include <vector>
#include "../commonly_used_functions/library.cpp"

class Instruction {
private:
    std::string raw;
    std::string command;
    int num_val;
    int execution_count;
public:
    Instruction(std::string raw){
        this -> raw = raw;
        std::vector<std::string> tokens(split(raw, ' '));
        command = tokens[0];
        num_val = std::stoi(tokens[1]);
        execution_count = 0;
    }

    int get_num_val() {
        return num_val;
    }

    int get_execution_count() {
        return execution_count;
    }

    std::string get_command() {
        return command;
    }

    void executed() {
        execution_count++;
    }
};

class Code{
private:
    int accumulator;
    std::vector<Instruction> instructions;
    int index;
    void execute(Instruction &instruction) {
        std::string command = instruction.get_command();
        if (command == "nop") {
            index++;
            instruction.executed();
            return;
        } else if (command == "acc") {
            accumulator += instruction.get_num_val();
            index++;
            instruction.executed();
            return;
        } else if (command == "jmp") {
            index += instruction.get_num_val();
            instruction.executed();
            return;
        }
    }
public:
    Code(std::vector<std::string> instructions) {
        accumulator = 0;
        index = 0;
        for (auto instruction: instructions) {
            Instruction current(instruction);
            this -> instructions.push_back(current);
        }
    }

    int terminate_value() {
        while (index < instructions.size()) {
            if (instructions[index].get_execution_count() > 0)
                return -1;
            execute(instructions[index]);
        }
        return accumulator;
    }

    int get_accumulator_until_second_execution() {
        while (index < instructions.size()) {
            if (instructions[index].get_execution_count() > 0)
                return accumulator;
            execute(instructions[index]);
        }
    }
};

int test_instructions(std::vector<std::string> instructions) {
    Code code(instructions);
    return code.terminate_value();
}

int part2(std::vector<std::string> instructions) {
    for (int i=0; i<instructions.size(); i++) {
        Instruction instruction(instructions[i]);
        std::vector<std::string> test_set(instructions);
        int test_val = -1;
        if (instruction.get_command() == "jmp") {
            test_set[i] = "nop " + std::to_string(instruction.get_num_val());
            test_val = test_instructions(test_set);
        } else if (instruction.get_command() == "nop") {
            test_set[i] = "jmp " + std::to_string(instruction.get_num_val());
            test_val = test_instructions(test_set);
        }
        if (test_val != -1) {
            return test_val;
        }
    }
    return -1;
}

int main() {
    std::fstream file;
    std::vector<std::string> lines;
    std::string line;
    file.open("../input.txt", std::fstream::in);
    while(getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    Code code(lines);
    std::cout << code.get_accumulator_until_second_execution() << std::endl;
    std::cout << part2(lines) << std::endl;
    return 0;
}
