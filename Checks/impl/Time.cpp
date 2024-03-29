#include "../Check.h"
#include "../../Logger/Logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <sddl.h>

#pragma comment(lib, "Advapi32.lib")

using namespace std;

DWORD GetServiceProcessId(const std::wstring& serviceName) { // Generated by ChatGPT
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) {
        std::cerr << "Failed to open SC Manager." << std::endl;
        return 0;
    }

    DWORD bufferSize = 0;
    DWORD serviceCount = 0;
    DWORD resumeHandle = 0;

    EnumServicesStatusEx(hSCManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_ACTIVE,
        NULL, 0, &bufferSize, &serviceCount, &resumeHandle, NULL);

    if (GetLastError() != ERROR_MORE_DATA) {
        CloseServiceHandle(hSCManager);
        logger::set_global(log_type::ERR);
        logger::log("Failed to enumerate services for retreiving process ID!\n");
        return 0;
    }

    BYTE* buffer = new BYTE[bufferSize];
    LPENUM_SERVICE_STATUS_PROCESS services = reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESS>(buffer);

    if (!EnumServicesStatusEx(hSCManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_ACTIVE,
        buffer, bufferSize, &bufferSize, &serviceCount, &resumeHandle, NULL)) {
        delete[] buffer;
        CloseServiceHandle(hSCManager);
        logger::set_global(log_type::ERR);
        logger::log("Failed to enumerate services for retreiving process ID!\n");
        return 0;
    }

    DWORD processId = 0;
    for (DWORD i = 0; i < serviceCount; i++) {
        if (serviceName == services[i].lpServiceName) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, services[i].ServiceStatusProcess.dwProcessId);
            if (hProcess) {
                CloseHandle(hProcess);
                processId = services[i].ServiceStatusProcess.dwProcessId;
                break;
            }
        }
    }

    delete[] buffer;
    CloseServiceHandle(hSCManager);

    return processId;
}

LPWSTR getSID() {
    // Get the access token for the current process
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        std::cerr << "[!] >> Failed to open process token. Error code: " << GetLastError() << std::endl;
        return 0;
    }

    // Get the size of the SID
    DWORD sidSize = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &sidSize);
    if (sidSize == 0) {
        std::cerr << "[!] >> Failed to get SID size. Error code: " << GetLastError() << std::endl;
        CloseHandle(hToken);
        return 0;
    }

    // Allocate memory for the SID
    PTOKEN_USER pTokenUser = static_cast<PTOKEN_USER>(malloc(sidSize));
    if (!pTokenUser) {
        logger::set_global(log_type::ERR);
        logger::log("Memory allocation failed for SID!\n");
        CloseHandle(hToken);
        return 0;
    }

    // Get the user SID
    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, sidSize, &sidSize)) {
        std::cerr << "[!] >> Failed to get user SID. Error code: " << GetLastError() << std::endl;
        free(pTokenUser);
        CloseHandle(hToken);
        return 0;
    }

    // Convert the binary SID to a string representation
    LPWSTR sidString;
    if (ConvertSidToStringSid(pTokenUser->User.Sid, &sidString)) {
        return sidString;
        LocalFree(sidString);
    }
    else {
        logger::set_global(log_type::ERR);
        logger::log("Failed to convert SID to string!\n");
        return 0;
    }

    // Clean up
    free(pTokenUser);
    CloseHandle(hToken);

}

ULONGLONG GetTimeDifferenceInSeconds(const FILETIME& fileTime) {
    SYSTEMTIME stNow;
    GetSystemTime(&stNow);

    FILETIME ftNow;
    SystemTimeToFileTime(&stNow, &ftNow);

    ULONGLONG timeDifference = (static_cast<ULONGLONG>(ftNow.dwHighDateTime) << 32 | ftNow.dwLowDateTime) -
        (static_cast<ULONGLONG>(fileTime.dwHighDateTime) << 32 | fileTime.dwLowDateTime);

    const ULONGLONG ticksPerSecond = 10000000ULL; // 1 second = 10,000,000 ticks

    ULONGLONG secondsDifference = timeDifference / ticksPerSecond;

    return secondsDifference;
}


ULONGLONG GetProcessStartTime(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (hProcess == NULL) {
        return 0;
    }

    FILETIME createTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime) == FALSE) {
        CloseHandle(hProcess);
        return 0;
    }

    CloseHandle(hProcess);

    ULONGLONG createTime64 = ((ULONGLONG)createTime.dwHighDateTime << 32) | createTime.dwLowDateTime;
    return createTime64;
}

std::string ConvertLPWSTRToString(LPWSTR lpwstr) {
    int size = WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, NULL, 0, NULL, NULL);
    if (size == 0) {
        // Conversion failed
        return "";
    }

    std::string result(size - 1, 0); // Exclude the null-terminator
    WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, &result[0], size, NULL, NULL);

    return result;
}

LPCWSTR ConvertStringToLPCWSTR(string narrowStr) {

    // Get the size of the wide string buffer needed
    int wideStrSize = MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), -1, NULL, 0);
    if (wideStrSize == 0) {
        std::cerr << "[!] >> Conversion failed. Error code: " << GetLastError() << std::endl;
        return 0;
    }

    // Allocate memory for the wide string buffer
    WCHAR* wideStrBuffer = new WCHAR[wideStrSize];

    // Convert the narrow string to wide string
    if (MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), -1, wideStrBuffer, wideStrSize) == 0) {
        std::cerr << "[!] >> Conversion failed. Error code: " << GetLastError() << std::endl;
        delete[] wideStrBuffer;
        return 0;
    }
    else {
        return wideStrBuffer;
    }
}

void checks::start_time_check() {
    logger::set_global(log_type::INFO);
    logger::log("Starting Time Check...\n");

	// Recycle bin
    string recyclePath = "C:\\$Recycle.Bin\\" + ConvertLPWSTRToString(getSID());
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (GetFileAttributesEx(ConvertStringToLPCWSTR(recyclePath), GetFileExInfoStandard, &fileInfo)) {
        FILETIME lastWriteTime = fileInfo.ftLastWriteTime;

        // Convert FILETIME to SYSTEMTIME for easier reading
        SYSTEMTIME st;
        if (FileTimeToSystemTime(&lastWriteTime, &st)) {
            ULONGLONG secondsDifference = GetTimeDifferenceInSeconds(lastWriteTime);
            if (secondsDifference < 3600) {
                logger::set_global(log_type::WARNING);
                logger::log("Recycle Bin was cleared less than an hour ago!\n");
            }
            else {
                logger::set_global(log_type::INFO);
                logger::log("Recycle Bin was checked\n");
                checks::passed++;
            }
        }
        else {
            logger::set_global(log_type::ERR);
            logger::log("Failed to convert FILETIME to SYSTEMTIME for Recycle Bin!\n");
        }
    }
    else {
        std::cerr << "[!] >> Failed to get file attributes for RecycleBin. Error code: " << GetLastError() << std::endl;
    }





	// Explorer
    HWND explorerWindow = FindWindow(L"Shell_TrayWnd", NULL);
    if (explorerWindow == NULL) {
        logger::set_global(log_type::ERR);
        logger::log("Windows Explorer wasn't found!\n");
        return;
    }

    DWORD explorerProcessId = 0;
    GetWindowThreadProcessId(explorerWindow, &explorerProcessId);

    ULONGLONG startTime = GetProcessStartTime(explorerProcessId);
    if (startTime == 0) {
        logger::set_global(log_type::ERR);
        logger::log("Failed to get the start time of explorer.exe!\n");
        return;
    }

    ULONGLONG currentTime__ = GetTickCount64() * 10000ULL;
    ULONGLONG timeDifference = currentTime__ - startTime;
    double secondsSinceStart = static_cast<double>(timeDifference) / 10000000.0;

    if (secondsSinceStart < 3600) { // If it was restarted under 1 hour
        logger::set_global(log_type::WARNING);
        logger::log("Windows Explorer was restarted less than an hour ago!\n");
    }
    else {
        logger::set_global(log_type::INFO);
        logger::log("Windows Explorer was checked\n");
        checks::passed++;
    }





	// DNS
    DWORD processID = GetServiceProcessId(L"Dnscache");

    ULONGLONG startTime_ = GetProcessStartTime(processID);
    if (startTime_ == 0) {
        logger::set_global(log_type::ERR);
        logger::log("Failed to get the start time of DNSCache!\n");
        return;
    }

    ULONGLONG currentTime_ = GetTickCount64() * 10000ULL;
    ULONGLONG timeDifference_ = currentTime__ - startTime_;
    double secondsSinceStart_ = static_cast<double>(timeDifference_) / 10000000.0;

    if (secondsSinceStart_ < 3600) { // If it was restarted under 1 hour
        logger::set_global(log_type::WARNING);
        logger::log("DNSCache was restarted less than an hour ago!\n");
    }
    else {
        logger::set_global(log_type::INFO);
        logger::log("DNSCache runtime was checked\n");
        checks::passed++;
    }





    // DPS
    DWORD processID_ = GetServiceProcessId(L"dps");

    ULONGLONG startTime__ = GetProcessStartTime(processID);
    if (startTime__ == 0) {
        logger::set_global(log_type::ERR);
        logger::log("Failed to get the start time of DPS!\n");
        return;
    }

    ULONGLONG currentTime___ = GetTickCount64() * 10000ULL;
    ULONGLONG timeDifference__ = currentTime___ - startTime__;
    double secondsSinceStart__ = static_cast<double>(timeDifference__) / 10000000.0;

    if (secondsSinceStart__ < 3600) { // If it was restarted under 1 hour
        logger::set_global(log_type::WARNING);
        logger::log("DPS was restarted less than an hour ago!\n");
    }
    else {
        logger::set_global(log_type::INFO);
        logger::log("DPS runtime was checked\n");
        checks::passed++;
    }
}