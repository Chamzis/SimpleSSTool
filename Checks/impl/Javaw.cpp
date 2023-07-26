#define _CRT_SECURE_NO_WARNINGS
#include "../../Logger/Logger.h"
#include "../Check.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <future>
#include <TlHelp32.h> 
#include <sddl.h>
#include <regex>
#include <iostream>

using namespace std;

void checks::start_javaw_scanner()
{
    logger("Starting Javaw Check...", log_type::INFO);

    bool pass = true;
    const char* strings[] = {
        "KillAura",
        "Destruct",
        "Reach",
        "AutoClicker",

    };

    DWORD pID = 0;
    HWND hwnd = FindWindowA("LWJGL", NULL);
    GetWindowThreadProcessId(hwnd, &pID);
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

    if (!handle) {
        logger("Couldn't establish handle for Minecraft! (Minecraft not launched?)", log_type::ERR);
        return;
    }

    unsigned char* p = NULL;
    MEMORY_BASIC_INFORMATION info;

    for (p = NULL; VirtualQueryEx(handle, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize) {
        if (info.State == MEM_COMMIT && (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE)) {
            SIZE_T bytes_read;
            std::vector<char> buffer;

            buffer.resize(info.RegionSize);
            ReadProcessMemory(handle, p, &buffer[0], info.RegionSize, &bytes_read); // Buffer overflow causing a 500mb uprise in memory usage (pls fix i have no idea)

            if (buffer.size() > 1000000)
                continue;
            

            std::string string = std::string(buffer.data(), buffer.size());

            string.erase(
                remove_if(
                    string.begin(), string.end(), [](char c) {
                        return !(c >= 32 && c < 126) || c == 10 || c == 13 || c == 9; // Checks if it's a valid char byte
                    }),
                string.end());

            for (std::string s : strings)
                if (string.find(s) != std::string::npos) {
                    logger("Found BlackListed String in Javaw!", log_type::ERR);
                    pass = false;
                }
                    

            Sleep(10);
        }
    }

    if (pass)
        checks::passed++;
}