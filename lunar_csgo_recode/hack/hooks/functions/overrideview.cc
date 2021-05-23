#include "../handler.hh"

#include "../../game/globals.hh"
#include "../../menu/config.hh"

#include "../../modules/misc/others.hh"

void __fastcall hack::hooks::overrideview_hk ( x86reg , sdk::valve::c_viewsetup * setup ) {
	if ( !setup )
		return originals::o_overrideview ( x86regout , setup );

	if ( hack::game::interfaces::engine->is_connected ( ) && hack::game::interfaces::engine->is_in_game ( ) && hack::game::globals::local ) {
		const auto & fx_fov = CONFIG_GET ( int , fnv1a ( "fx_fov" ) );
		const auto & fx_fov_override = CONFIG_GET ( bool , fnv1a ( "fx_fov_override" ) );

		if ( !hack::game::globals::local->scoped ( ) )
			setup->m_fov = fx_fov;

		if ( fx_fov_override )
			setup->m_fov = fx_fov;

		hack::modules::misc::others::in_overrideview ( setup );
	}

	originals::o_overrideview ( x86regout , setup );
}