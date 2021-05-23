#include "antiaim.hh"
#include "tickbase.hh"

#include "../prediction/engine_prediction.hh"

#include "../../menu/framework.hh"

#include "../../../sdk/math/mathutils.hh"

/* placeholder */
void do_fakelag ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	const auto & lag_enabled = CONFIG_GET ( bool , fnv1a ( "lag_enabled" ) );

	if ( !lag_enabled )
		return;

	const auto & lag_standing = CONFIG_GET ( int , fnv1a ( "lag_standing" ) );
	const auto & lag_moving = CONFIG_GET ( int , fnv1a ( "lag_moving" ) );
	const auto & lag_air = CONFIG_GET ( int , fnv1a ( "lag_air" ) );
	const auto & lag_modifier = CONFIG_GET ( int , fnv1a ( "lag_modifier" ) );

	auto modifier = lag_modifier;

	//fnv1a ( "lag_onpeek" ) , fnv1a ( "lag_onland" ) , fnv1a ( "lag_onreload" ) , fnv1a ( "lag_onkey" )
	const auto & lag_onpeek = CONFIG_GET ( bool , fnv1a ( "lag_onpeek" ) );
	const auto & lag_onland = CONFIG_GET ( bool , fnv1a ( "lag_onland" ) );
	const auto & lag_onreload = CONFIG_GET ( bool , fnv1a ( "lag_onreload" ) );
	const auto & lag_onkey = CONFIG_GET ( bool , fnv1a ( "lag_onkey" ) );

	int limit = 2;

	if ( !( hack::game::globals::local->flags ( ) & 1 ) ) // swoopae ur such a retard --- didnt ask + it works ~swoopae
		limit = lag_air;
	else if ( hack::game::globals::local->velocity ( ).length_2d ( ) > 5.f )
		limit = lag_moving;
	else
		limit = lag_standing;

	/* unchoke on land for funny moment */
	if ( lag_onland && hack::game::globals::local->flags ( ) & 1 && hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_jump ) {
		limit = 0;
		modifier = 0;
	} else if ( lag_onland && !( hack::game::globals::local->flags ( ) & 1 ) && hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_jump ) {
		limit = /* hack::game::interfaces::clientstate->valve_server ? 7 : */ 16;
		modifier = 0;
	}

	if ( lag_onreload && hack::game::globals::local->active_weapon ( ) && hack::game::globals::local->active_weapon ( )->in_reload ( ) ) {
		limit = /* hack::game::interfaces::clientstate->valve_server ? 7 : */ 16;
		modifier = 0;
	}


	const auto & r_tickbasemanip = CONFIG_GET ( bool , fnv1a ( "r_tickbasemanip" ) );

	const static auto get_optimal_fakelag_if_shifting = [ & ] ( ) {
		if ( !hack::game::globals::local->active_weapon ( ) )
			return 1;

		switch ( hack::game::globals::local->active_weapon ( )->item_definition_index ( ) ) {
			case sdk::e_item_definition_index::weapon_deagle:
				return 2; break;
			case sdk::e_item_definition_index::weapon_awp:
			case sdk::e_item_definition_index::weapon_ssg08:
				return 4; break;
			case sdk::e_item_definition_index::weapon_usp_silencer:
			case sdk::e_item_definition_index::weapon_hkp2000:
			case sdk::e_item_definition_index::weapon_fiveseven:
			case sdk::e_item_definition_index::weapon_p250:
				return 3; break;
			case sdk::e_item_definition_index::weapon_cz75a:
				return 5; break;
			case sdk::e_item_definition_index::weapon_tec9:
				return 6; break;
			default:
				return 1; break;
		}

		return 1;
	};

	if ( r_tickbasemanip && hack::modules::rage::tickbase::tickbase_key_state && limit )
		limit = get_optimal_fakelag_if_shifting ( );

	/* modify limit based on lag_spike multicombo */


	/* TODO: set send packet based on modifier */
	switch ( modifier ) {
		case 0: /* linear */
			hack::game::globals::send_packet = hack::game::interfaces::clientstate->choked_commands >= limit;
			break;
		case 1: /* adaptive */
			hack::game::globals::send_packet = hack::game::interfaces::global_vars->m_tickcount % 11 ? hack::game::interfaces::clientstate->choked_commands >= limit : hack::game::interfaces::clientstate->choked_commands >= limit / 3;
			break;
	}

	if ( hack::modules::rage::tickbase::force_sendpacket ) {
		hack::game::globals::send_packet = true;
		hack::modules::rage::tickbase::force_sendpacket = false;
	}
}

bool do_fakeduck ( ) {
	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return false;

	/* don't fakeduck in air lol... */
	if ( !( hack::game::globals::local->flags ( ) & 1 ) )
		return false;

	const auto & rage_fakeduck = CONFIG_GET ( bool , fnv1a ( "rage_fakeduck" ) );

	if ( !rage_fakeduck || !hack::modules::rage::antiaim::rage_fakeduck_keystate )
		return false;

	hack::game::globals::cmd->m_buttons |= sdk::valve::cmd_buttons::in_bullrush;

	hack::game::globals::send_packet = hack::game::interfaces::clientstate->choked_commands > 13;

	if ( hack::game::interfaces::clientstate->choked_commands <= 7 )
		hack::game::globals::cmd->m_buttons &= ~0x4;
	else
		hack::game::globals::cmd->m_buttons |= 0x4;

	return true;
}

float grab_preferred_yaw ( ) {
	float best_yaw = 180.f; /* default */

	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return best_yaw;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return best_yaw;

	const auto & aa_enabled = CONFIG_GET ( bool , fnv1a ( "aa_enabled" ) );

	if ( !aa_enabled )
		return best_yaw;

	const auto & aa_at_target = CONFIG_GET ( bool , fnv1a ( "aa_at_target" ) );
	const auto & aa_yaw = CONFIG_GET ( int , fnv1a ( "aa_yaw" ) );
	// std::vector<std::string> modifiers = { STR ( "none" ) , STR ( "180z" ) , STR ( "jitter" ) , STR ( "spin" ) };
	const auto & aa_modifier = CONFIG_GET ( int , fnv1a ( "aa_modifier" ) );
	const auto & aa_modifier_range = CONFIG_GET ( int , fnv1a ( "aa_modifier_range" ) );

	best_yaw = aa_yaw + ( aa_at_target ? 0.f : hack::game::globals::cmd->m_angs.y );

	/* TODO: do manual aa here */

	/* basic at target based on fov */
	if ( aa_at_target ) {
		c_vector3 current_angles;
		hack::game::interfaces::engine->get_viewangles ( current_angles );

		auto best_fov = 9999.f;
		sdk::c_player * best_player = nullptr;

		sdk::foreach_player ( [ & ] ( sdk::c_player * player ) {
			if ( hack::game::globals::local == player || !player->enemy ( ) )
				return;

			const auto current_fov =
				sdk::math::get_fov ( hack::game::globals::local->eye_offset ( ) , player->absolute_origin ( ) , current_angles );

			if ( current_fov < best_fov ) {
				best_player = player;
				best_fov = current_fov;
			}
		} );

		if ( best_player ) {
			const auto best_at_target_angles =
				sdk::math::calc_angle ( hack::game::globals::local->eye_offset ( ) , best_player->absolute_origin ( ) );
			best_yaw += best_at_target_angles.y;
		} else {
			best_yaw += hack::game::globals::cmd->m_angs.y;
		}
	}

	switch ( aa_modifier ) {
		case 0:
			return best_yaw;
			break;
		case 1: /* 180z */
			return best_yaw - 90 / 2.f + std::fmodf ( hack::game::interfaces::global_vars->m_curtime * 60 , 90 );
			break;
		case 2: { /* jitter */
			static auto jitter = false;

			if ( !hack::game::interfaces::clientstate->choked_commands )
				jitter = !jitter;

			return jitter ? best_yaw - ( aa_modifier_range / 2 ) : best_yaw + ( aa_modifier_range / 2 );
		} break;
		case 3: /* ayyware slowspin */
			if ( aa_modifier_range != 0.f )
				return best_yaw + ( std::fmodf ( hack::game::interfaces::global_vars->m_curtime * 80 , aa_modifier_range ) -
									( aa_modifier_range / 2 ) );
			else
				return best_yaw + std::fmodf ( hack::game::interfaces::global_vars->m_curtime * 80 , 360.f );
			break;
	}

	return best_yaw;
}

/* pasted but i don't give a shit */
void hack::modules::rage::antiaim::in_movement_fix ( std::function<void ( )> fn ) {
	const auto o_angs = hack::game::globals::cmd->m_angs;

	fn ( );

	c_vector3 view_fwd , view_right , view_up , cmd_fwd , cmd_right , cmd_up;
	c_vector3 viewangles = hack::game::globals::cmd->m_angs;

	sdk::math::angle_vectors ( o_angs , view_fwd , &view_right , &view_up );
	sdk::math::angle_vectors ( viewangles , cmd_fwd , &cmd_right , &cmd_up );

	const float v8 = std::sqrtf ( view_fwd.x * view_fwd.x + view_fwd.y * view_fwd.y );
	const float v10 = std::sqrtf ( view_right.x * view_right.x + view_right.y * view_right.y );
	const float v12 = std::sqrtf ( view_up.z * view_up.z );

	const c_vector3 norm_view_fwd ( 1.f / v8 * view_fwd.x , 1.f / v8 * view_fwd.y , 0.f );
	const c_vector3 norm_view_right ( 1.f / v10 * view_right.x , 1.f / v10 * view_right.y , 0.f );
	const c_vector3 norm_view_up ( 0.f , 0.f , 1.f / v12 * view_up.z );

	const float v14 = std::sqrtf ( cmd_fwd.x * cmd_fwd.x + cmd_fwd.y * cmd_fwd.y );
	const float v16 = std::sqrtf ( cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y );
	const float v18 = std::sqrtf ( cmd_up.z * cmd_up.z );

	const c_vector3 norm_cmd_fwd ( 1.f / v14 * cmd_fwd.x , 1.f / v14 * cmd_fwd.y , 0.f );
	const c_vector3 norm_cmd_right ( 1.f / v16 * cmd_right.x , 1.f / v16 * cmd_right.y , 0.f );
	const c_vector3 norm_cmd_up ( 0.f , 0.f , 1.f / v18 * cmd_up.z );

	const float v22 = norm_view_fwd.x * hack::game::globals::cmd->m_fmove;
	const float v26 = norm_view_fwd.y * hack::game::globals::cmd->m_fmove;
	const float v28 = norm_view_fwd.z * hack::game::globals::cmd->m_fmove;
	const float v24 = norm_view_right.x * hack::game::globals::cmd->m_smove;
	const float v23 = norm_view_right.y * hack::game::globals::cmd->m_smove;
	const float v25 = norm_view_right.z * hack::game::globals::cmd->m_smove;
	const float v30 = norm_view_up.x * hack::game::globals::cmd->m_umove;
	const float v27 = norm_view_up.z * hack::game::globals::cmd->m_umove;
	const float v29 = norm_view_up.y * hack::game::globals::cmd->m_umove;

	hack::game::globals::cmd->m_fmove = norm_cmd_fwd.x * v24 + norm_cmd_fwd.y * v23 + norm_cmd_fwd.z * v25 +
		( norm_cmd_fwd.x * v22 + norm_cmd_fwd.y * v26 + norm_cmd_fwd.z * v28 ) +
		( norm_cmd_fwd.y * v30 + norm_cmd_fwd.x * v29 + norm_cmd_fwd.z * v27 );

	hack::game::globals::cmd->m_smove = norm_cmd_right.x * v24 + norm_cmd_right.y * v23 + norm_cmd_right.z * v25 +
		( norm_cmd_right.x * v22 + norm_cmd_right.y * v26 + norm_cmd_right.z * v28 ) +
		( norm_cmd_right.x * v29 + norm_cmd_right.y * v30 + norm_cmd_right.z * v27 );

	hack::game::globals::cmd->m_umove = norm_cmd_up.x * v23 + norm_cmd_up.y * v24 + norm_cmd_up.z * v25 +
		( norm_cmd_up.x * v26 + norm_cmd_up.y * v22 + norm_cmd_up.z * v28 ) +
		( norm_cmd_up.x * v30 + norm_cmd_up.y * v29 + norm_cmd_up.z * v27 );

	hack::game::globals::cmd->m_fmove = std::clamp ( hack::game::globals::cmd->m_fmove , -450.f , 450.f );
	hack::game::globals::cmd->m_smove = std::clamp ( hack::game::globals::cmd->m_smove , -450.f , 450.f );
	hack::game::globals::cmd->m_umove = std::clamp ( hack::game::globals::cmd->m_umove , -320.f , 320.f );

	if ( hack::game::globals::local->movetype ( ) != sdk::movetypes::movetype_ladder )
		hack::game::globals::cmd->m_buttons &= ~( sdk::valve::cmd_buttons::in_forward | sdk::valve::cmd_buttons::in_back | sdk::valve::cmd_buttons::in_moveright | sdk::valve::cmd_buttons::in_moveleft );
}

void hack::modules::rage::antiaim::in_prediction ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	const auto shooting = hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_attack && hack::game::globals::local->can_shoot ( );

	if ( !do_fakeduck ( ) && !shooting ) {
		do_fakelag ( );
	}

	if ( shooting )
		return;

	const auto & aa_enabled = CONFIG_GET ( bool , fnv1a ( "aa_enabled" ) );

	if ( !aa_enabled )
		return;

	if ( hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_use )
		return;

	const auto movetypes = hack::game::globals::local->movetype ( );

	if ( movetypes == sdk::movetypes::movetype_observer || movetypes == sdk::movetypes::movetype_noclip ||
		 movetypes == sdk::movetypes::movetype_ladder )
		return;

	const auto & aa_desync = CONFIG_GET ( bool , fnv1a ( "aa_desync" ) );
	const auto & aa_desync_strength = CONFIG_GET ( int , fnv1a ( "aa_desync_strength" ) );
	// std::vector<std::string> desync_modifiers = { STR ( "static" ) , STR ( "sway" ) , STR ( "sidemove" ) };
	const auto & aa_desync_modifier = CONFIG_GET ( int , fnv1a ( "aa_desync_modifier" ) );

	/* TODO: hardcoding pitch is cringe, once i'll implement scrolling i'll do some cool shit here */
	hack::game::globals::cmd->m_angs.x = 89.f;
	hack::game::globals::cmd->m_angs.z = 0.f;

	/* may seem stupid at first but makes sense considering how our at target currently works */
	static float last_real_yaw = 180.f;
	if ( hack::game::globals::send_packet )
		last_real_yaw = grab_preferred_yaw ( );

	hack::game::globals::cmd->m_angs.y = last_real_yaw;

	if ( !aa_desync )
		return;

	switch ( aa_desync_modifier ) {
		case 0: { /* straight up lby breaking memes */
			static auto sidemove_direction = 0.f , sidemove_speed = 0.f;

			sidemove_speed = hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_duck ? -3.3f : -1.2f;

			/* force lby update */
			if ( hack::game::globals::send_packet ) {
				sidemove_direction *= -1.f;

				/* ghetto shit instead of checking for buttons to not fuck up autopeek -swoopae */
				if ( std::fabsf ( hack::game::globals::cmd->m_smove ) < 4.0f && std::fabsf ( hack::game::globals::cmd->m_fmove ) < 4.0f )
					hack::game::globals::cmd->m_smove = sidemove_direction * sidemove_speed;
			}

			if ( !hack::game::globals::send_packet )
				hack::game::globals::cmd->m_angs.y += 120.f * ( hack::modules::rage::antiaim::aa_desync_invert_keystate ? -1.f : 1.f );
		} break;
		case 1: { /* pseudo-sway */
			/* later */
		} break;
		case 2: { /* otc moment */
			static auto sidemove_direction = -3.3f;

			/* only do this shit on choke so they don't realise we're actually doing the otc meme 
			*  outsmarting the competition :sunglasses: */
			if ( !hack::game::globals::send_packet ) {
				sidemove_direction *= -1.f;

				if ( std::fabsf ( hack::game::globals::cmd->m_smove ) < 4.0f && std::fabsf ( hack::game::globals::cmd->m_fmove ) < 4.0f )
					hack::game::globals::cmd->m_smove = sidemove_direction;

				hack::game::globals::cmd->m_angs.y += 75.f * aa_desync_strength * ( hack::modules::rage::antiaim::aa_desync_invert_keystate ? -1.f : 1.f );
			}
		} break;
	}
}

void hack::modules::rage::antiaim::in_endscene ( ) {
	const auto aa_desync_invert_key = HANDLE_KEYBIND ( fnv1a ( "aa_desync_invert_key" ) );
	const auto rage_fakeduck_key = HANDLE_KEYBIND ( fnv1a ( "rage_fakeduck_key" ) );

	aa_desync_invert_keystate = aa_desync_invert_key (  );
	rage_fakeduck_keystate = rage_fakeduck_key (  );
}