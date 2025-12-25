#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

std::string input;

void get_input2() {
    std::cout << "Enter your code (type 'EXIT' to quit):" << std::endl;
    while (true) {
        std::getline(std::cin, input);
        if (input == "EXIT") {
            break;
        }
        std::cout << "You entered: " << input << std::endl;
    }
}

void launch_vim() {
    std::system("vim");
}