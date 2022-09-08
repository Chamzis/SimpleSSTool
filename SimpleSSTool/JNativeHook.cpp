#include "Logger.h"
#include "Xray_check.h"
#include "Check.h"
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

auto start_jnative_check() -> void 
{
    logger("Starting JNativeHook Check", log_type::INFO);

    char* temp_path;
    size_t len1;
    errno_t qqq = _dupenv_s(&temp_path, &len1, "TMP");

    vector<string> listFiles;
    listFiles = XRAY_CHECK_H::get_all_files_names_within_folder(temp_path);

    if (listFiles.size() != 0) 
        for (string s : listFiles) 
            if (s.find("JNativeHook") != std::string::npos) 
                logger("Found JNativeHook in %temp% ", log_type::ERR);
            
                
        
    
    delete temp_path;
}