#include "../../Logger/Logger.h"
#include "../Check.h"
#include "../../Utility/Utility.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include <vector>

DWORD get_service_pid(const std::string& service_name) { // For some reason it throws errors and sometimes it works??????
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

void checks::start_dns_check()
{
    logger::set_global(log_type::INFO);
    logger::log("Starting DNS Check...\n");

    bool pass = true;
	std::string websites[] = {"unknowncheats.me:::UnknownCheats", "dreamclient.xyz:::Dream Client", "neverlack.in:::Drip Client", "slapp.in:::Slappin Client", "reddit.com/r/ghostclient:::Reddit cheating page"}; // Add more sites (I fogor ze hak names :skull:)

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, get_service_pid("Dnscache"));

    if (!handle) {
        logger::set_global(log_type::ERR);
        logger::log("Couldn't establish handle for DNS!\n");
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

            if (buffer.size() > 1000000)
                continue;

            std::string string = std::string(buffer.data(), buffer.size());

            string.erase(
                remove_if(
                    string.begin(), string.end(), [](char c) {
                        return !(c >= 32 && c < 126) || c == 10 || c == 13 || c == 9;
                    }),
                string.end());

            for (std::string s : websites) {
                auto separated_s = util::separate_string(s);

                if (string.find(separated_s[0]) != std::string::npos) {
                    logger::set_global(log_type::WARNING);
                    logger::log("Found Blacklisted Website in DNSCache: ", separated_s[1], "\n");
                    pass = false;
                }
            }

            Sleep(10);
        }
    }

    if(pass)
        checks::passed++;
}