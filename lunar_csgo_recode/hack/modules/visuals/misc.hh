#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include <vector>
#include <functional>

namespace hack::modules::visuals::misc {
	struct rain_particle_t {
		c_vector3 m_origin , m_velocity;
	};

	inline std::vector<rain_particle_t> rain_particles;

	void in_endscene ( );
}