#include <iostream>
#include <windows.h>

const auto h_console = GetStdHandle(STD_OUTPUT_HANDLE);

enum log_type {
    INFO,
    WARNING,
    ERR
};

auto logger(const char info[], const log_type lt) -> void
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
}

auto reset_console_color(const HANDLE h_console) -> void
{
    SetConsoleTextAttribute(h_console, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}