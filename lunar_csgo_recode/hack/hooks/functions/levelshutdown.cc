#include "../handler.hh"

#include "../../game/globals.hh"

#include "../../modules/misc/others.hh"
#include "../../modules/animations/animations.hh"

void __fastcall hack::hooks::levelshutdown_hk ( x86reg ) {
	/* prevents some weird crashes */
	hack::game::globals::cmd = nullptr;
	hack::game::globals::local = nullptr;
	hack::game::globals::send_packet = true;

	for ( auto & iter : hack::modules::misc::others::weapon_name )
		iter.first.empty ( );

	/* reset animfix records */
	/* flush cached data */
	for ( auto & iter : hack::modules::animations::animation_system->m_animation_info ) {
		/* reset important data */
		iter.m_player = nullptr;
		iter.m_handle = 0;
		iter.m_missed_shots = 0;
		iter.m_spawntime = 0.f;
		iter.m_should_resolve = false;

		/* reset resolver data */
		iter.m_last_resolver_state = hack::modules::animations::e_resolver_state::resolver_auto;
		iter.m_resolved_side = hack::modules::animations::e_resolver_side::side_auto;

		/* erase all records if we have any */
		if ( !iter.m_recieved_frames.empty ( ) )
			iter.m_recieved_frames.clear ( );
	}

	originals::o_levelshutdown ( x86regout );
}