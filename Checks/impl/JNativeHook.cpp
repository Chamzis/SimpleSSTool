#include "../../Logger/Logger.h"
#include "../../Utility/Utility.h"
#include "../Check.h"
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void checks::start_jnative_check() 
{
    logger("Starting JNativeHook Check", log_type::INFO);

    char* temp_path;
    size_t len1;
    _dupenv_s(&temp_path, &len1, "TMP");
    //_dupenv_s(&temp_path, nullptr, "TMP");

    vector<string> listFiles;
    listFiles = util::files_in_folder(temp_path);

        for (const auto& entry : fs::directory_iterator(temp_path)) {
            filesystem::path balls = entry.path().filename();
            if (balls.string().find("JNativeHook") != std::string::npos)
                logger("Found JNativeHook in %temp% ", log_type::ERR);
        }
           

    delete temp_path;
}