#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "Logger.h"
#include "Check.h"


int main() {
    /*
    Made by dabrovic
    Have fun using it (it's most likley going to be shit)
    */

    using namespace std::this_thread;    
    using namespace std::chrono_literals; 
    
    const auto h_console = GetStdHandle(STD_OUTPUT_HANDLE); // Setting the variables up

    SetConsoleTextAttribute(h_console, 2);
    printf("Welcome to SimpleSSTool!\nThis isn't the best SS tool and is made for fun.\nMost of the checks are trash and the whole program is just a base for an another project.\nThe program is going to start in a few seconds...\n \n-dabrovic");

    sleep_for(6s);

    start_checks(); // GOTO CheckThread filter to edit the order of the operations
    return 0;
}