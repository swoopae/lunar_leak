#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include <functional>

namespace hack::modules::visuals::esp {
	struct esp_info_t {
		c_vector2 m_pos , m_size;
		sdk::c_player * m_player;
		bool m_dormant;
	};

	void in_endscene ( );
}