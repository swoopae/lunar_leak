#include "../handler.hh"

#include "../../game/globals.hh"
#include "../../menu/config.hh"

#include "../../modules/prediction/engine_prediction.hh"
#include "../../modules/rage/tickbase.hh"
#include "../../modules/rage/antiaim.hh"
#include "../../modules/animations/animations.hh"
#include "../../modules/misc/movement.hh"
#include "../../modules/misc/others.hh"

#include <intrin.h>

bool __fastcall hack::hooks::createmove_hk ( x86reg , float sampletime , sdk::valve::ucmd_t * cmd ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) ) {
		/* panic and clean up */
		hack::game::globals::local = nullptr;
		hack::game::globals::cmd = nullptr;

		return originals::o_createmove ( x86regout , sampletime , cmd );
	}

	if ( !cmd || !cmd->m_cmdnum )
		return originals::o_createmove ( x86regout , sampletime , cmd );

	/* grab local */
	hack::game::globals::local = hack::game::interfaces::entity_list->get<sdk::c_player *> ( hack::game::interfaces::engine->get_local_player ( ) );

	if ( !hack::game::globals::local )
		return originals::o_createmove ( x86regout , sampletime , cmd );

	/* grab sendpacket off the previous stackframe and set our other globals properly */
	const auto ebp = *reinterpret_cast< std::uintptr_t * >( reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress ( ) ) - sizeof ( std::uintptr_t * ) );

	hack::game::globals::send_packet = reinterpret_cast< bool * >( ebp - 28 );
	hack::game::globals::cmd = cmd;

	/* engine prediction "fix" for low performance users - prevents desync of important values if fps dips below tickrate which might cause commands to not be predicted and ep to use wrong values */
	if ( hack::game::globals::cmd->m_tickcount != hack::modules::rage::tickbase::shifted_cmd.m_tickcount && hack::game::interfaces::global_vars->m_ipt <= hack::game::interfaces::global_vars->m_frametime )
		hack::game::interfaces::prediction->update ( hack::game::interfaces::clientstate->delta_tick , hack::game::interfaces::clientstate->delta_tick > 0 ,
													 hack::game::interfaces::clientstate->last_command_ack ,
													 hack::game::interfaces::clientstate->last_outgoing_command + hack::game::interfaces::clientstate->choked_commands );
	/* fastduck */
	const auto & fastduck = CONFIG_GET ( bool , fnv1a ( "m_noduckdelay" ) );

	if ( fastduck )
		hack::game::globals::cmd->m_buttons |= sdk::valve::cmd_buttons::in_bullrush;

	hack::modules::misc::movement::in_createmove ( );
	hack::modules::misc::others::in_createmove ( );

	/* engine prediction */
	hack::modules::prediction::engine_prediction::run_in_prediction ( [ & ] {
		hack::modules::misc::movement::in_prediction ( );

		hack::modules::rage::antiaim::in_movement_fix ( [ & ] {
			hack::modules::rage::tickbase::in_createmove ( );
			hack::modules::rage::antiaim::in_prediction ( );
		} );
	} );

	const auto should_auto_attack = [ & ] ( ) {
		auto weapon = hack::game::globals::local->active_weapon ( );

		if ( !weapon )
			return false;

		return weapon->item_definition_index ( ) == sdk::e_item_definition_index::weapon_c4 || weapon->item_definition_index ( ) == sdk::e_item_definition_index::weapon_revolver || weapon->weapon_kind ( ) == sdk::e_cs_weapon_type::weapontype_grenade;
	};

	if ( !hack::game::globals::local->can_shoot ( ) && hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_attack && !should_auto_attack ( ) )
		hack::game::globals::cmd->m_buttons &= ~sdk::valve::cmd_buttons::in_attack;

	hack::game::globals::cmd->m_angs.clamp ( );

	/* storing shit */
	/* this if statement prevents fakeducking from showing an inaccurate angle on local in thirdperson - i know this is ghetto but i don't give a shit */
	static bool still_choking = false , sent_a_tick = false;
	if ( ( still_choking || sent_a_tick ) || ( hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_attack && !hack::game::globals::send_packet ) && !should_auto_attack ( ) ) {
		if ( ( hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_attack && !hack::game::globals::send_packet ) )
			hack::modules::rage::antiaim::last_shot_angles = hack::game::globals::cmd->m_angs;

		if ( hack::game::globals::send_packet ) sent_a_tick = true;

		if ( !sent_a_tick ) still_choking = true;
		else if ( !hack::game::globals::send_packet ) still_choking = sent_a_tick = false;

		if ( still_choking || sent_a_tick ) {
			hack::modules::rage::antiaim::last_sent_angles = hack::modules::rage::antiaim::last_shot_angles;
			hack::modules::rage::antiaim::last_choked_angles = hack::modules::rage::antiaim::last_shot_angles;
		}
	} else {
		if ( hack::game::globals::send_packet ) hack::modules::rage::antiaim::last_sent_angles = hack::game::globals::cmd->m_angs;
		else hack::modules::rage::antiaim::last_choked_angles = hack::game::globals::cmd->m_angs;
	}

	hack::modules::animations::in_createmove ( );

	*reinterpret_cast< bool * >( ebp - 28 ) = hack::game::globals::send_packet;

	return false;
}