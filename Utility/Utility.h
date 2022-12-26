#include <string>
#include <vector>
#include <tchar.h>
#include <Windows.h>

namespace util {
	DWORD get_service_pid(const std::string& service_name);
	DWORD get_process_pid(const std::string& process_name);
}