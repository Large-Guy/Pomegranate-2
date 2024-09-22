#include "debug.h"
#include <iostream>

void Debug::Log::info(std::string message) {
    std::cout << "\033[1;44m[INFO]\033[0m  " << message << std::endl;
}

void Debug::Log::warn(std::string message) {
    std::cout << "\033[1;43m[WARN]\033[0m  " << message << std::endl;
}

void Debug::Log::error(std::string message) {
    std::cout << "\033[1;41m[ERROR]\033[0m " << message << std::endl;
}

void Debug::Log::pass(std::string message) {
    std::cout << "\033[1;42m[PASS]\033[0m  " << message << std::endl;
}

void Debug::Log::fail(std::string message) {
    std::cout << "\033[1;41m[FAIL]\033[0m  " << message << std::endl;
}