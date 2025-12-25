#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// defining some random stuff
#define something "something"

void get_input() {
    while (true) {
        std::cout << "Welcome to the Code Editor!" << std::endl;
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit") {
            std::cout << "Exiting..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    }
}