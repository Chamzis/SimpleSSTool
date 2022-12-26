#include "../Check.h"
#include "../../Logger/Logger.h"
#include "../../Utility/Utility.h"
#include <string>
#include <Windows.h>
#include <iostream>

void checks::start_pcasvc_check() { // Mainly for Koid
	logger("Starting PcaSvc Check...", log_type::INFO);

    std::string strings[] = {
        "0x1c3000:Koid (New)", // Koid
        "0x1d3000:Koid (Old)", // Koid
        "0xb1000:Itami"        // Itami
        "0xa1000:IceTea"       // IceTea
    }; 

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, util::get_service_pid("PcaSvc"));

    if (!handle) {
        logger("Couldn't establish handle for PcaSvc! (PcaSvc doesn't exist?)", log_type::ERR);
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
                        return !(c >= 32 && c < 126);
                    }),
                string.end());

            for (std::string s : strings)
                if (string.find(s.substr(0, s.find(":") - 1)) != std::string::npos) {
                    logger("Found Blacklisted String in PcaSvc!", log_type::ERR);
                    std::cout << "Client: " << s.substr(s.find(":") + 1, s.size()) << std::endl;
                }

            Sleep(10);
        }
    }

    CloseHandle(handle);
    p = nullptr;
}