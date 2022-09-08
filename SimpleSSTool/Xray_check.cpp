#include "Logger.h"
#include "SimpleSSTool.h"
#include <Windows.h>
#include <vector>
#include <string>

#include <future>
#include <TlHelp32.h>
#include <sddl.h>

using namespace std;

string wchar_t2string(const wchar_t* wchar)
{
    string str = "";
    int index = 0;
    while (wchar[index] != 0)
    {
        str += (char)wchar[index];
        ++index;
    }
    return str;
}

wchar_t* string2wchar_t(const string& str)
{
    wchar_t wchar[260];
    int index = 0;
    while (index < str.size())
    {
        wchar[index] = (wchar_t)str[index];
        ++index;
    }
    wchar[index] = 0;
    return wchar;
}

vector<std::string> get_all_files_names_within_folder(string folder)
{
    vector<string> names;
    string search_path = folder + "/*.*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(string2wchar_t(search_path), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
                names.push_back(wchar_t2string(fd.cFileName));
            
        } while (FindNextFile(hFind, &fd));
        FindClose(hFind);
    }
    return names;
}



auto start_xray_check() -> void 
{
	logger("Starting XRay Check", log_type::INFO);
    
    vector<string> listFiles; 
    char* appdata_path;
    size_t len;
    errno_t err = _dupenv_s(&appdata_path, &len, "APPDATA");
    std::string s = appdata_path;
    std::string res_path = s + "\\.minecraft\\resourcepacks";
    listFiles = get_all_files_names_within_folder(res_path.c_str()); // Too lazy to check for folders

    if (listFiles.size() != 0) 
        for (string s : listFiles) 
            if (s.find("xray") != std::string::npos || s.find("Xray") != std::string::npos || s.find("XRay") != std::string::npos) 
                logger("Found XRay in %appdata% ", log_type::WARNING);

    /*
    This is the most basic detection for xray.
    You can 
    */

    
    
}