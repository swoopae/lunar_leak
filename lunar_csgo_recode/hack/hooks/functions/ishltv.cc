#include "../handler.hh"

#include "../../game/globals.hh"

#include "../../modules/animations/animations.hh"

#include <intrin.h>

bool __fastcall hack::hooks::ishltv_hk ( x86reg ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return originals::o_ishltv ( x86regout );

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return originals::o_ishltv ( x86regout );

	const auto current_address = std::uintptr_t ( _ReturnAddress ( ) );

	const static auto anim_lod_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "84 C0 0F 85 ? ? ? ? A1 ? ? ? ? 8B B7" ) );
	const static auto setup_velocity_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80" ) );
	const static auto accumulate_layers_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "84 C0 75 0D F6 87" ) );

	if ( hack::modules::animations::in_local_animfix && current_address != accumulate_layers_address )
		return originals::o_ishltv ( x86regout );

	if ( current_address == anim_lod_address ||
		 current_address == setup_velocity_address ||
		 current_address == accumulate_layers_address )
		return true;

	return originals::o_ishltv ( x86regout );
}