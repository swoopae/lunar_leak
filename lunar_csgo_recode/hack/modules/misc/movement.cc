#include "movement.hh"

#include "../../menu/framework.hh"

#include "../../../sdk/math/mathutils.hh"

#include <deque>

void strafe ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd )
		return;

	const auto move_type = hack::game::globals::local->movetype ( );
	const auto flags = hack::game::globals::local->flags ( );

	/* more sanity checks */
	if ( move_type == sdk::movetypes::movetype_ladder || move_type == sdk::movetypes::movetype_noclip )
		return;

	/* @TODO */
}

void hop ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd )
		return;

	const auto move_type = hack::game::globals::local->movetype ( );
	const auto flags = hack::game::globals::local->flags ( );

	if ( move_type != sdk::movetypes::movetype_ladder && move_type != sdk::movetypes::movetype_noclip )
		if ( !( flags & 1 ) && hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_jump )
			hack::game::globals::cmd->m_buttons &= ~sdk::valve::cmd_buttons::in_jump;
}

void selfpeek ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd )
		return;

	static std::deque<hack::modules::misc::movement::peekcmd_t> known_cmds;

	const auto rotate_movement = [ & ] ( float yaw ) {
		float rotation = deg2rad ( hack::game::globals::cmd->m_angs.y - yaw );

		float cos_rot = cos ( rotation );
		float sin_rot = sin ( rotation );

		float new_forwardmove = ( cos_rot * hack::game::globals::cmd->m_fmove ) - ( sin_rot * hack::game::globals::cmd->m_smove );
		float new_sidemove = ( sin_rot * hack::game::globals::cmd->m_fmove ) + ( cos_rot * hack::game::globals::cmd->m_smove );

		hack::game::globals::cmd->m_fmove = new_forwardmove;
		hack::game::globals::cmd->m_smove = new_sidemove;
	};

	/* reset */
	if ( !hack::modules::misc::movement::m_selfpeek_keybind_state ) {
		hack::modules::misc::movement::selfpeek_state = hack::modules::misc::movement::e_selfpeek_states::STATE_REST;
		known_cmds.clear ( );
		return;
	}

	/* if we were resting and we got here store origin and start storing cmds */
	if ( hack::modules::misc::movement::selfpeek_state == hack::modules::misc::movement::e_selfpeek_states::STATE_REST ) {
		hack::modules::misc::movement::last_selfpeek_origin = hack::game::globals::local->absolute_origin ( );
		hack::modules::misc::movement::selfpeek_state = hack::modules::misc::movement::e_selfpeek_states::STATE_PEEKING;

		/* reset just in case */
		known_cmds.clear ( );
	}

	/* store cmds - TODO: make max stored cmds something like 128 and after just go to origin to save some memory ~swoopae */
	if ( hack::modules::misc::movement::selfpeek_state == hack::modules::misc::movement::e_selfpeek_states::STATE_PEEKING ) {
		/* construct our cmd */
		hack::modules::misc::movement::peekcmd_t peekcmd {};
		peekcmd.m_yaw = hack::game::globals::cmd->m_angs.y;
		peekcmd.m_fmove = hack::game::globals::cmd->m_fmove;
		peekcmd.m_smove = hack::game::globals::cmd->m_smove;

		/* store it */
		known_cmds.push_front ( peekcmd );

		/* did we just shoot? stop storing and start going back to cover */
		if ( hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_attack )
			hack::modules::misc::movement::selfpeek_state = hack::modules::misc::movement::e_selfpeek_states::STATE_RETURNING;
	}

	/* go thru cmds and go back */
	if ( hack::modules::misc::movement::selfpeek_state == hack::modules::misc::movement::e_selfpeek_states::STATE_RETURNING ) {
		/* no more cmds to go through , keep drawing origin but stop doing anything */
		if ( known_cmds.empty ( ) && hack::game::globals::local->absolute_origin ( ).dist_to ( hack::modules::misc::movement::last_selfpeek_origin ) < 30.f ) {
			hack::modules::misc::movement::selfpeek_state = hack::modules::misc::movement::e_selfpeek_states::STATE_MAX;
		} else if ( known_cmds.empty ( ) ) {
			/* move to our origin manually */
			c_vector3 angle {} , final_vec = hack::modules::misc::movement::last_selfpeek_origin - hack::game::globals::local->absolute_origin ( );
			sdk::math::vector_angles ( final_vec , angle );

			hack::game::globals::cmd->m_fmove = 450.f;

			/* rotate move value */
			rotate_movement ( angle.y );

			return;
		} else {
			/* let's move */
			/* figure out the fastest possible values for fmove and smove without completely fucking direction up */
			/* this obviously needs some work such as removing a number of commands to prevent overshooting the target but it should work in most scenarios where you'll release the selfpeek key after you reach safety */
			auto factor = 450.f / ( std::max ( std::abs ( known_cmds.begin ( )->m_fmove ) , std::abs ( known_cmds.begin ( )->m_smove ) ) );

			hack::game::globals::cmd->m_fmove = -known_cmds.begin ( )->m_fmove * factor;
			hack::game::globals::cmd->m_smove = -known_cmds.begin ( )->m_smove * factor;

			/* rotate old move values */
			rotate_movement ( known_cmds.begin ( )->m_yaw );

			/* delete current cmd */
			known_cmds.erase ( known_cmds.begin ( ) );

			return;
		}
	}

	/* cuck our movement until the user lets go of the selfpeek key */
	if ( hack::modules::misc::movement::selfpeek_state == hack::modules::misc::movement::e_selfpeek_states::STATE_MAX ) {
		hack::game::globals::cmd->m_fmove = std::clamp ( hack::game::globals::cmd->m_fmove , -10.f , 10.f );
		hack::game::globals::cmd->m_smove = std::clamp ( hack::game::globals::cmd->m_smove , -10.f , 10.f );
	}
}

void hack::modules::misc::movement::in_createmove ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd )
		return;

	/* variables */
	const auto & bhop = CONFIG_GET ( bool , fnv1a ( "m_bhop" ) );

	/* movement modules */
	if ( bhop )
		hop ( );
}

void hack::modules::misc::movement::in_prediction ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd )
		return;

	/* variables */
	const auto & m_autostrafer = CONFIG_GET ( bool , fnv1a ( "m_autostrafer" ) );
	const auto & m_selfpeek = CONFIG_GET ( bool , fnv1a ( "m_selfpeek" ) );

	/* movement modules */
	if ( m_autostrafer )
		strafe ( );

	if ( m_selfpeek )
		selfpeek ( );
}

void hack::modules::misc::movement::in_endscene ( ) {
	const auto m_selfpeek_key = HANDLE_KEYBIND ( fnv1a ( "m_selfpeek_key" ) );

	m_selfpeek_keybind_state = m_selfpeek_key ( );
}
