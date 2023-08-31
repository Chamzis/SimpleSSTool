#include "Logger.h"
#include <iostream>
#include <windows.h>
#include <sstream>

const auto h_console = GetStdHandle(STD_OUTPUT_HANDLE);

// Colors
/*const std::string RESET_COLOR = "\033[0m";
const std::string RED_COLOR = "\033[31m";
const std::string GREEN_COLOR = "\033[32m";
const std::string YELLOW_COLOR = "\033[33m";
const std::string BLUE_COLOR = "\033[34m";*/

void logger::set_global(log_type type) {
    switch (type) {
    case log_type::ERR:
        std::cout << "[" << RED_COLOR << "X" << RESET_COLOR << "] >> ";
        break;

    case log_type::INFO:
        std::cout << "[" << GREEN_COLOR << "#" << RESET_COLOR << "] >> ";
        break;

    case log_type::WARNING:
        std::cout << "[" << YELLOW_COLOR << "!" << RESET_COLOR << "] >> ";
        break;
    }
}

/*template<typename... Args>
void logger::customPrint(const Args&...) {
    std::cout << RESET_COLOR;
}

template<typename T, typename... Args>
void logger::log(const T& arg, const Args&... args) {
    std::stringstream ss;
    ss << arg;

    if (std::is_same<T, int>::value || std::is_same<T, double>::value) {
        std::cout << GREEN_COLOR;
    }
    else if (std::is_same<T, std::string>::value) {
        std::cout << BLUE_COLOR;
    }
    else {
        std::cout << RESET_COLOR;
    }

    std::cout << ss.str();
    customPrint(args...);
}*/

/*auto logger(const char info[], const log_type lt) -> void
{
    switch (lt) {
    case log_type::INFO:
        SetConsoleTextAttribute(h_console, 8);
        std::cout << "[";
        SetConsoleTextAttribute(h_console, 2);
        std::cout << "#";
        SetConsoleTextAttribute(h_console, 8);
        std::cout << "] >> ";
        SetConsoleTextAttribute(h_console, 7);
        std::cout << info << std::endl;
        break;
    case log_type::WARNING:
        SetConsoleTextAttribute(h_console, 8);
        std::cout << "[";
        SetConsoleTextAttribute(h_console, 6);
        std::cout << "!";
        SetConsoleTextAttribute(h_console, 8);
        std::cout << "] >> ";
        SetConsoleTextAttribute(h_console, 7);
        std::cout << info << std::endl;
        break;
    case log_type::ERR:
        SetConsoleTextAttribute(h_console, 8);
        std::cout << "[";
        SetConsoleTextAttribute(h_console, 4);
        std::cout << "X";
        SetConsoleTextAttribute(h_console, 8);
        std::cout << "] >> ";
        SetConsoleTextAttribute(h_console, 7);
        std::cout << info << std::endl;
        break;
    }
}*/

auto reset_console_color(const HANDLE h_console) -> void
{
    SetConsoleTextAttribute(h_console, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}