#include "../handler.hh"

#include "../../modules/prediction/engine_prediction.hh"
#include "../../modules/rage/tickbase.hh"
#include "../../modules/animations/animations.hh"

#include "../../../sdk/math/mathutils.hh"

#include "../../menu/config.hh"

/* hahahahahahahhahahahahahahahhahahahahahahah -swoopae from future */
void __fastcall hack::hooks::runcommand_hk ( x86reg , sdk::c_player * player , sdk::valve::ucmd_t * cmd , sdk::valve::c_move_helper * helper ) {
	if ( player == hack::game::globals::local ) {
		static auto sv_max_usercmd_future_ticks = hack::game::interfaces::convar_system->get_convar ( STR ( "sv_max_usercmd_future_ticks" ) ); /* used to detect prediction issues */

		/* TODO: recharging fix & account for ping - this only acts perfectly in low ping situations and on local servers. with high ping i'm sure it'll fuck up */
		static int tickbase_difference = 2;
		static int old_tickbase = 0;
		
		if ( hack::modules::rage::tickbase::shift_amount == 15 ) {
			if ( cmd->m_cmdnum == hack::modules::rage::tickbase::shifted_cmd.m_cmdnum ) {
				tickbase_difference = hack::game::globals::local->tickbase ( ) - cmd->m_tickcount;
				player->tickbase ( ) = cmd->m_tickcount + ( hack::modules::rage::tickbase::shift_amount - 12 );
			}
		} else {
			if ( cmd->m_cmdnum == hack::modules::rage::tickbase::shifted_cmd.m_cmdnum && hack::modules::rage::tickbase::shift_amount - 11 > 0 ) {
				player->tickbase ( ) = old_tickbase - ( hack::modules::rage::tickbase::shift_amount - 12 );
			}
		}

		old_tickbase = player->tickbase ( );

		/* TODO: unfuck revolver also other things ~swoopae */

		/* fix for velocity modifier */
		const auto velmod = player->velocity_modifier ( );

		/* call orig */
		originals::o_runcommand ( x86regout , player , cmd , helper );

		/* fix for 14 tick ONLY */
		if ( hack::modules::rage::tickbase::shift_amount == 14 ) {
			if ( cmd->m_cmdnum == hack::modules::rage::tickbase::shifted_cmd.m_cmdnum + 1 ) {
				player->tickbase ( ) += tickbase_difference + 1;
			}
		}

		/* fix for some prediction errors */
		player->collision_state ( ) = 0;

		if ( velmod != -FLT_MAX && velmod != FLT_MAX )
			player->velocity_modifier ( ) = velmod;

		/* get uncompressed netvar data */
		// hack::modules::prediction::engine_prediction::store_netvars ( );

		return;
	}

	originals::o_runcommand ( x86regout , player , cmd , helper );
}