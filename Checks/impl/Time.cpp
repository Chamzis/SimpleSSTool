#include "../Check.h"
#include "../../Logger/Logger.h"
#include "../../Utility/Utility.h"
#include <iostream>

using namespace std;

void checks::start_time_check() {
	logger("Starting Time Check...", log_type::INFO);

	if (util::get_start_time(util::get_process_pid("explorer.exe"))[1] < 30 && util::get_start_time(util::get_process_pid("explorer.exe"))[2] == 0) { // 30 Minutes
		logger("Explorer started under 30 minutes!", log_type::WARNING);
	}

	if (util::get_start_time(util::get_process_pid("javaw.exe"))[1] < 30 && util::get_start_time(util::get_process_pid("javaw.exe"))[1] == 0) { // 30 Minutes
		logger("Minecraft started under 30 minutes!", log_type::WARNING);
	}


}
