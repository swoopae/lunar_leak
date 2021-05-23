#pragma once

#include <windows.h>

#include "interfaces.hh"

namespace hack::game::globals {
	inline HMODULE this_instance;
	inline bool should_uninject = false , send_packet = true;
	inline sdk::valve::ucmd_t * cmd = nullptr;
	inline sdk::c_player * local = nullptr;
}