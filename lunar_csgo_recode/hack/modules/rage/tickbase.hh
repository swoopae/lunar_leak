#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include "../../../sdk/weapon.hh"

#include <array>
#include <functional>

namespace hack::modules::rage::tickbase {
	bool in_clmove ( );
	void in_createmove ( );
	void in_endscene ( );
	bool in_writeucmddelta ( void * ecx , void * edx , int slot , void * buf , int from , int to , bool newcmd );

	inline sdk::valve::ucmd_t shifted_cmd;
	inline int tickbase_shift = 0 , shift_amount = 0 , original_tickbase = 0;
	inline bool tickbase_key_state = false , can_we_dt_again = true , did_just_dt = false , force_sendpacket = false;
}