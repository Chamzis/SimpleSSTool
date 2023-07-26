#ifndef CHECK_H
#define CHECK_H

void start_checks();

#endif 

namespace checks {
	inline int passed;
	void start_dns_check();
	void start_javaw_scanner();
	void start_jnative_check();
	void start_version_check();
	void start_xray_check();
	void start_time_check();
	void start_pcaclient_check();
	void start_dps_check();
}
