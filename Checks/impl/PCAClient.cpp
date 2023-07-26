#include "../../Logger/Logger.h"
#include "../../SimpleSSTool.h"
#include "../Check.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <regex>
#include <iostream>

bool is_char(byte n) {
	return (n >= 32 && n <= 126) || n == 10 || n == 13 || n == 9;
}

void checks::start_pcaclient_check() {
	logger("Starting PCAClient Check...", log_type::INFO);

	bool pass = true;

	HWND explorerWindow = FindWindow(L"Shell_TrayWnd", NULL);
	if (explorerWindow == NULL) {
		logger("Windows Explorer wasn't found!", log_type::ERR);
		return;
	}

	DWORD id = 0;
	GetWindowThreadProcessId(explorerWindow, &id);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);

	if (!handle) {
		logger("Couldn't establish handle for explorer.exe!", log_type::ERR);
		system("PAUSE");
		exit(1);
	}

	unsigned char* p = NULL;
	MEMORY_BASIC_INFORMATION info;
	std::vector<std::string> all_strings;

	byte first = 0, second = 0;
	bool isUnicode = false;

	// Memory dump of explorer then regex search lol
	for (p = NULL; VirtualQueryEx(handle, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize) {
		if (info.Protect == PAGE_NOACCESS) continue;

		if (info.State == MEM_COMMIT && info.Type == MEM_PRIVATE) {
			SIZE_T bytes_read;
			std::vector<char> buffer;

			buffer.resize(info.RegionSize);
			ReadProcessMemory(handle, p, &buffer[0], info.RegionSize, &bytes_read);

			//if (buffer.size() > 1000000) Why do you cause pain
			//	continue;

			std::string string = std::string(buffer.data(), buffer.size());

			string.erase(
				remove_if(
					string.begin(), string.end(), [](char c) {
						return !((c >= 32 && c < 126) || c == 10 || c == 13 || c == 9);
					}),
				string.end());
			
			if (string.find("TRACE") != std::string::npos) 
				all_strings.push_back(string);
			
		}
	}

	std::regex regex("PcaClient,.+,(\\w:\\\\.+.exe)");
	std::smatch matches;
	for (std::string string : all_strings) {
		std::string::const_iterator searchStart(string.cbegin());
		while (std::regex_search(searchStart, string.cend(), matches, regex)) {
			std::cout << matches[0].str().substr(10, matches[0].str().length()) << std::endl;
			searchStart = matches[0].second;
		}
	}
}