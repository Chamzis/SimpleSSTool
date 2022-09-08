#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include "Logger.h"
#include "Xray_check.h"

using namespace std;

auto start_version_check() -> void 
{
	logger("Starting Version check", log_type::INFO);

    vector<std::string> hack_names = {
        "Rise",
        "Wurst",
        "Flux",
    };

    vector<string> listFiles;
    char* appdata_path;
    size_t len;
    errno_t err = _dupenv_s(&appdata_path, &len, "APPDATA");
    strcat(appdata_path, "\\versions");
    listFiles = XRAY_CHECK_H::get_all_files_names_within_folder(appdata_path);

    if (listFiles.size() != 0)
        for (string s : listFiles)
            for (string ss : hack_names)                         // Most cancerous code I have ever written
                if (s.find(ss) != std::string::npos)
                    logger("Found Hack in Versions Folder!", log_type::WARNING);
}