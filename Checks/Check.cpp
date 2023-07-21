#include "Check.h"
#include "../Logger/Logger.h"
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <chrono>

inline int passed = 0;

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

	checks::start_time_check();
	checks::start_xray_check();
	checks::start_jnative_check();
	checks::start_version_check();
	checks::start_dns_check();
	checks::start_javaw_scanner();

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

	std::cout << "User has passed ";
	SetConsoleTextAttribute(h_console, 2);
	std::cout << checks::passed << " ";
	SetConsoleTextAttribute(h_console, 7);
	std::cout << "checks out of ";
	SetConsoleTextAttribute(h_console, 2);
	std::cout << "8";
	SetConsoleTextAttribute(h_console, 7);
	std::cout << "!" << std::endl;

	system("pause"); 
}