#include "../../Logger/Logger.h"
#include "../../SimpleSSTool.h"
#include "../Check.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>

DWORD get_service_pid_(const std::string& service_name) { // For some reason it throws errors and sometimes it works??????
    SC_HANDLE scm = OpenSCManagerA(nullptr, nullptr, NULL);
    SC_HANDLE sc = OpenServiceA(scm, (LPCSTR)service_name.c_str(), SERVICE_QUERY_STATUS);

    SERVICE_STATUS_PROCESS ssp = { 0 };
    DWORD bytes_needed = 0;
    QueryServiceStatusEx(sc, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(ssp),
        &bytes_needed);

    CloseServiceHandle(sc);
    CloseServiceHandle(scm);

    return ssp.dwProcessId;
}

void checks::start_dps_check() {
    logger("Starting DPS Check...", INFO);

    bool pass = true; // I will add the detected reason later when i upgrade the logger to include the variables
    std::string strings[] = { // All of these strings can be found in the echo.ac discord server <3
    "2021/03/03:14:04:39", // Generic AutoClicker
    "2020/09/28:01:13:10", // Itami 2.0
    "2021/01/31:06:05:27", // Wannacry
    "2021/07/16:13:55:15", // CC Cleaner has been run
    "2021/07/16:13:53:13", // CC Cleaner has been run
    "2019/11/19:16:48:04", // Ettelen
    "2083/04/22:21:51:14", // Encephalon
    "2020/06/23:09:45:05", // IceTea
    "2016/01/20:08:28:11", // Itami 1.4.1
    "2020/08/24:15:03:39", // Itami 1.4
    "2020/07/21:00:58:02", // Itami 1.3
    "2020/07/22:23:28:56", // Koid New (i think Koid kills DPS idk, need PcaSvc to check which is 0x1d3000 & 0x1c3000)
    "2020/05/29:06:27:30", // Koid Old
    "2019/05/19:20:48:16", // Cracked Vape Lite
    };

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, get_service_pid_("dps"));

    if (!handle) {
        logger("Couldn't establish handle for DPS!", log_type::ERR);
        system("PAUSE");
        exit(1);
    }

    unsigned char* p = NULL;
    MEMORY_BASIC_INFORMATION info;

    for (p = NULL; VirtualQueryEx(handle, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize) {
        if (info.State == MEM_COMMIT && (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE)) {
            SIZE_T bytes_read;
            std::vector<char> buffer;

            buffer.resize(info.RegionSize);
            ReadProcessMemory(handle, p, &buffer[0], info.RegionSize, &bytes_read);

            std::string string = std::string(buffer.data(), buffer.size());

            string.erase(
                remove_if(
                    string.begin(), string.end(), [](char c) {
                        return !(c >= 32 && c < 126) || c == 10 || c == 13 || c == 9;
                    }),
                string.end());

            for (std::string s : strings)
                if (string.find(s) != std::string::npos) {
                    logger("Found Blacklisted String in DPS!", log_type::WARNING);
                    pass = false;
                }


            Sleep(10);
        }
    }

    if (pass)
        checks::passed++;
}