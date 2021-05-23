#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include <functional>

namespace hack::modules::misc::movement {
	enum class e_selfpeek_states {
		STATE_REST = 0,
		STATE_PEEKING,
		STATE_RETURNING,
		STATE_MAX
	};

	struct peekcmd_t {
		float m_yaw , m_fmove , m_smove;
	};

	inline c_vector3 last_selfpeek_origin;
	inline e_selfpeek_states selfpeek_state;
	inline bool m_selfpeek_keybind_state;

	void in_createmove ( );
	void in_prediction ( );
	void in_endscene ( );
}