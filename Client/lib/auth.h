#pragma once

namespace dragon_auth {
	inline const char* SIGN_OFF = "amongers/o";
	inline const char* KEY = "amongusballs";

	inline bool verbose = false;
	void disc();
	void init();
	bool authenticate();
}