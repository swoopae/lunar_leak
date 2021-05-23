#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include <functional>

namespace hack::modules::rage::antiaim {
	inline bool aa_desync_invert_keystate = false , rage_fakeduck_keystate = false;
	inline c_vector3 last_sent_angles , last_choked_angles , last_shot_angles;

	void in_movement_fix ( std::function<void ( )> fn );
	void in_prediction ( );
	void in_endscene ( );
}