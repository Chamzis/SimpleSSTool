#ifndef CHECK_H
#define CHECK_H

void start_checks();

#endif 

namespace checks {
	void start_dns_check();
	void start_javaw_scanner();
	void start_jnative_check();
	void start_version_check();
	void start_xray_check();
	void start_pcasvc_check();
}
