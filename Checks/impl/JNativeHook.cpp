#include "../../Logger/Logger.h"
#include "../../Utility/Utility.h"
#include "../Check.h"
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

void checks::start_jnative_check() 
{
    logger::set_global(log_type::INFO);
    logger::log("Starting JNativeHook Check...\n");

    bool p = true;
    char* temp_path;
    size_t len1;
    errno_t qqq = _dupenv_s(&temp_path, &len1, "TMP");

    vector<string> listFiles;
    listFiles = util::files_in_folder(temp_path);

    if (listFiles.size() != 0) 
        for (string s : listFiles) 
            if (s.find("JNativeHook") != std::string::npos) {
                logger::set_global(log_type::ERR);
                logger::log("Found JNativeHook in %temp%\n");
                p = false;
            }
                
    
    delete temp_path;

    if (p)
        checks::passed++;
}