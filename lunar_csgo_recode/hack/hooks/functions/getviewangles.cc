#include "../handler.hh"

#include "../../game/globals.hh"
#include "../../modules/rage/antiaim.hh"

#include <intrin.h>

c_vector3 * __fastcall hack::hooks::getviewangles_hk ( x86reg ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return originals::o_getviewangles ( x86regout );

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return originals::o_getviewangles ( x86regout );

	if ( !ecx )
		return originals::o_getviewangles ( x86regout );

	/* oops forgot about this */
	if ( ecx != hack::game::globals::local )
		return originals::o_getviewangles ( x86regout );

	const static auto ret_to_tp_pitch = sig ( MODULE_CLIENT_PANORAMA , STR ( "8B CE F3 0F 10 00 8B 06 F3 0F 11 45 ? FF 90 ? ? ? ? F3 0F 10 55" ) );
	const static auto ret_to_tp_yaw = sig ( MODULE_CLIENT_PANORAMA , STR ( "F3 0F 10 55 ? 51 8B 8E" ) );

	if ( ( uintptr_t ) _ReturnAddress ( ) == ret_to_tp_pitch
		 || ( uintptr_t ) _ReturnAddress ( ) == ret_to_tp_yaw )
		return &hack::modules::rage::antiaim::last_sent_angles;

	return originals::o_getviewangles ( x86regout );
}