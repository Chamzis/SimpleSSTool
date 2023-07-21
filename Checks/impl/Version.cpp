#define _CRT_SECURE_NO_WARNINGS
#include "../Check.h"
#include "../../Utility/Utility.h"
#include "../../Logger/Logger.h"
#include <iostream>
#include <vector>

using namespace std;

void checks::start_version_check() 
{
	logger("Starting Version Check...", log_type::INFO);

    bool p = true;
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
    listFiles = util::files_in_folder(appdata_path);

    if (listFiles.size() != 0)
        for (string s : listFiles)
            for (string ss : hack_names)                         // Most cancerous code I have ever written
                if (s.find(ss) != std::string::npos) {
                    logger("Found Hack in Versions Folder!", log_type::WARNING);
                    p = false;
                }

    if (p)
        checks::passed;
                    
}