#ifndef LOGGER_H
#define LOGGER_H

enum log_type {
    INFO,
    WARNING,
    ERR
};

void logger(const char info[], const log_type lt);
#endif 

