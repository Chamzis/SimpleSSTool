#include "../../Logger/Logger.h"
#include "../../SimpleSSTool.h"
#include "../../Utility/Utility.h"
#include "../Check.h"
#include <Windows.h>
#include <vector>
#include <string>

#include <future>
#include <TlHelp32.h>
#include <sddl.h>

using namespace std;

/*
This is the most basic detection for xray.
You can check for .json size in bytes
*/

void checks::start_xray_check() 
{
	logger("Starting XRay Check", log_type::INFO);
    
    vector<string> listFiles; 
    char* appdata_path;
    size_t len;
    errno_t err = _dupenv_s(&appdata_path, &len, "APPDATA");
    std::string s = appdata_path;
    std::string res_path = s + "\\.minecraft\\resourcepacks";
    listFiles = util::files_in_folder(res_path.c_str()); // Too lazy to check for folders

    if (listFiles.size() != 0) 
        for (string s : listFiles) 
            if (s.find("xray") != std::string::npos || s.find("Xray") != std::string::npos || s.find("XRay") != std::string::npos) 
                logger("Found XRay in %appdata% ", log_type::WARNING);
}