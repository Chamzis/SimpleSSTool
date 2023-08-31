#ifndef LOGGER_H
#define LOGGER_H

/*extern enum log_type {
    INFO,
    WARNING,
    ERR
};
namespace logger {
    template<typename T, typename... Args>
    extern void log(const T& arg, const Args&... args);

    extern void customPrint();
    extern void set_global(log_type type);
}*/

#include <iostream>
#include <sstream>
#include <string>

// Colors
const std::string RESET_COLOR = "\033[0m";
const std::string RED_COLOR = "\033[31m";
const std::string GREEN_COLOR = "\033[32m";
const std::string YELLOW_COLOR = "\033[33m";
const std::string BLUE_COLOR = "\033[34m";

enum class log_type { ERR, INFO, WARNING };

class logger {

public:
    template<typename... Args>
    static void log(const Args&...) {
        std::cout << RESET_COLOR;
    }
    static void set_global(log_type type);

    template<typename T, typename... Args>
    static void log(const T& arg, const Args&... args) {
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
        log(args...);
        
    }
};


#endif 

