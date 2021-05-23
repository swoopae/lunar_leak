#include "../handler.hh"

#include "../../game/globals.hh"
#include "../../modules/rage/tickbase.hh"
#include "../../menu/config.hh"

void __cdecl hack::hooks::clmove_hk ( float samples , bool final_tick ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_connected ( ) || !hack::game::interfaces::engine->is_in_game ( ) )
		return originals::o_clmove ( samples , final_tick );

	if ( !hack::game::globals::local || !hack::game::globals::cmd )
		return originals::o_clmove ( samples , final_tick );

	const auto & r_tickbasemanip = CONFIG_GET ( bool , fnv1a ( "r_tickbasemanip" ) );

	/* if shifting do shit */
	if ( hack::modules::rage::tickbase::in_clmove ( ) )
		return;

	originals::o_clmove ( samples , final_tick );
}