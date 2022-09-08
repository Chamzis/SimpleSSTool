#ifndef XRAY_CHECK_H
#define XRAY_CHECK_H
#include <vector>
#include <string>

using namespace std;

string wchar_t2string(const wchar_t* wchar);
wchar_t* string2wchar_t(const string& str);
vector<string> get_all_files_names_within_folder(string folder);
void start_xray_check();


#endif 
