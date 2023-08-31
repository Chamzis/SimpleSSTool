#include <vector>
#include <string>
#include <Windows.h>
#include "../Utility/Utility.h"

using namespace std;

std::string wchar_t2string(const wchar_t* wchar)
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
    size_t index = 0;
    while (index < str.size())
    {
        wchar[index] = (wchar_t)str[index];
        ++index;
    }
    wchar[index] = 0;
    return wchar;
}

std::vector<std::string> util::files_in_folder(std::string folder)
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

std::vector<std::string> util::separate_string(std::string string) {
    std::vector<std::string> separation;
    size_t separatorPos = string.find(":::"); // This will be constant in every string

    separation.push_back(string.substr(0, separatorPos)); // The actual string that needs to be searched ( separate_string("unknowncheats.me")[0] )
    separation.push_back(string.substr(separatorPos + 3)); // The masked string that is displayed ( separate_string("unknowncheats.me")[1] )

    return separation;
}