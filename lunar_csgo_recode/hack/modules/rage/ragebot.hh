#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include "../../../sdk/weapon.hh"

#include <array>
#include <functional>

namespace hack::modules::rage::ragebot {
	void in_prediction ( );
	void in_endscene ( );

	/* 0 - autosniper , 1 - scout , 2 - awp , 3 - deag/r8 , 4 - pistols , 5 - others */
	inline std::array<int , 6> missed_shots_by_weapon;
}