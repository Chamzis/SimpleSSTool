#include "Xray_check.h"
#include "JNativeHook.h"
#include "Javaw.h"
#include "Version.h"
#include "Check.h"
#include "Logger.h"
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <chrono>

__int64 currentTimeMillis() {
	FILETIME f;
	GetSystemTimeAsFileTime(&f);
	(long long)f.dwHighDateTime;
	__int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
	return (nano - 116444736000000000LL) / 10000;
}

auto start_checks() -> void 
{
	auto timer = currentTimeMillis();

	system("CLS");

	start_xray_check();
	start_jnative_check();
	start_javaw_scanner();
	start_version_check();

	const auto h_console = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(h_console, 1);
	
	std::cout << "" << std::endl;
	std::cout << "[===!===] FINISHED [===!===]" << std::endl;

	SetConsoleTextAttribute(h_console, 7);

	std::cout << "Finished in ";

	SetConsoleTextAttribute(h_console, 2);

	auto time = currentTimeMillis() - timer;

	std::cout << time;

	SetConsoleTextAttribute(h_console, 7);

	std::cout << " ms ";
	SetConsoleTextAttribute(h_console, 8);
	std::cout << "(" << time / 1000 << " seconds)" << std::endl;
	SetConsoleTextAttribute(h_console, 7);

	system("pause"); //
}