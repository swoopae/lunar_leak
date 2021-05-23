#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include "../../../sdk/weapon.hh"

#include <functional>
#include <array>

namespace hack::modules::misc::others {
	inline bool thirdperson_key = false;
	inline std::array<std::pair<std::string , bool> , 65> weapon_name;

	void in_endscene ( );
	void in_createmove ( );
	void in_fsn ( int stage );
	void in_overrideview ( sdk::valve::c_viewsetup * setup );
}