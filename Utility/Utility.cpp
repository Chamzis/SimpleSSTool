#include <vector>
#include <string>
#include <Windows.h>
#include "../Utility/Utility.h"
#include <TlHelp32.h>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>

using namespace std;

DWORD util::get_process_pid(const std::string& process_name) {
    PROCESSENTRY32 process_info;
    process_info.dwSize = sizeof(process_info);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(snapshot, &process_info);
    if (!process_name.compare((char*)process_info.szExeFile)) {
        CloseHandle(snapshot);
        return process_info.th32ProcessID;
    }

    while (Process32Next(snapshot, &process_info)) {
        if (!process_name.compare((char*)process_info.szExeFile)) {
            CloseHandle(snapshot);
            return process_info.th32ProcessID;
        }
    }

    CloseHandle(snapshot);

    return 0;
}

DWORD util::get_service_pid(const std::string& service_name) { // For some reason it throws errors and sometimes it works??????
    const auto hScm = OpenSCManagerA(nullptr, nullptr, NULL);
    const auto hSc = OpenServiceA(hScm, (LPCSTR)service_name.c_str(), SERVICE_QUERY_STATUS);

    SERVICE_STATUS_PROCESS ssp = {};
    DWORD bytesNeeded = 0;
    QueryServiceStatusEx(hSc, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(ssp), &bytesNeeded);

    CloseServiceHandle(hSc);
    CloseServiceHandle(hScm);

    return ssp.dwProcessId;
}