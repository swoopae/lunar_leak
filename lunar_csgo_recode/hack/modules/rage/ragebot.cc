#include "ragebot.hh"

#include "../animations/animations.hh"

#include "../../menu/config.hh"

#include "../../../sdk/weapon.hh"
#include "../../../sdk/math/mathutils.hh"

#include <deque>

std::vector<sdk::hitboxes> get_hitboxes ( int shots_missed = 0 ) {
	std::vector<sdk::hitboxes> ret;

	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return ret;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return ret;

	const auto weapon = hack::game::globals::local->active_weapon ( );

	if ( !weapon )
		return ret;

	const auto & rc_hitbox_hands = CONFIG_GET ( bool , fnv1a ( "rc_hitbox_hands" ) );
	const auto & rc_hitbox_legs = CONFIG_GET ( bool , fnv1a ( "rc_hitbox_legs" ) );
	const auto & rc_hitbox_feet = CONFIG_GET ( bool , fnv1a ( "rc_hitbox_feet" ) );

	/* we manipulate what hitboxes we use based on the weapon and how many shots we missed */
	auto weapon_index = 0;

	switch ( weapon->item_definition_index ( ) ) {
		case sdk::e_item_definition_index::weapon_scar20:
		case sdk::e_item_definition_index::weapon_g3sg1:
			weapon_index = 1;
			break;
		case sdk::e_item_definition_index::weapon_ssg08:
			weapon_index = 2;
			break;
		case sdk::e_item_definition_index::weapon_awp:
			weapon_index = 3;
			break;
		case sdk::e_item_definition_index::weapon_revolver:
		case sdk::e_item_definition_index::weapon_deagle:
			weapon_index = 4;
			break;
	}

	/* hitboxes are in order of safety */

	/* always hitscan this */
	ret.push_back ( sdk::hitboxes::pelvis );
	ret.push_back ( sdk::hitboxes::body );
	ret.push_back ( sdk::hitboxes::chest );
	ret.push_back ( sdk::hitboxes::upper_chest );

	/* gonna let the compiler optimize this shit */
	if ( ( weapon_index == 0 || weapon_index == 4 ) && shots_missed < 4 )
		ret.push_back ( sdk::hitboxes::head );
	else if ( weapon_index == 1 && shots_missed < 3 )
		ret.push_back ( sdk::hitboxes::head );
	else if ( weapon_index == 2 && shots_missed < 2 )
		ret.push_back ( sdk::hitboxes::head );
	else if ( weapon_index == 3 && !shots_missed )
		ret.push_back ( sdk::hitboxes::head );

	/* TODO: refactor this section */
	if ( rc_hitbox_legs && shots_missed < 3 ) {
		ret.push_back ( sdk::hitboxes::left_thigh );
		ret.push_back ( sdk::hitboxes::right_thigh );
	}

	if ( rc_hitbox_feet && shots_missed < 3 ) {
		ret.push_back ( sdk::hitboxes::left_foot );
		ret.push_back ( sdk::hitboxes::right_foot );
	}

	if ( rc_hitbox_hands && shots_missed < 3 ) {
		ret.push_back ( sdk::hitboxes::left_forearm );
		ret.push_back ( sdk::hitboxes::right_forearm );
		ret.push_back ( sdk::hitboxes::left_upper_arm );
		ret.push_back ( sdk::hitboxes::right_upper_arm );
	}

	return ret;
}

bool get_hitbox_points ( sdk::c_player * player , sdk::hitboxes hitbox , c_matrix3x4 * resolved_matrix , std::vector<c_vector3> & out ) {
	const sdk::valve::model_t * model = player->anim_model ( );
	if ( !model )
		return false;

	sdk::valve::c_studio_hdr * hdr = hack::game::interfaces::model_info->get_studio_model ( model );
	if ( !hdr )
		return false;

	sdk::valve::studio_hitbox_set_t * set = hdr->hitbox_set ( player->hitbox_set ( ) );
	if ( !set )
		return false;

	sdk::valve::studio_box_t * bbox = set->hitbox ( static_cast< int >( hitbox ) );
	if ( !bbox )
		return false;


}

/* entity list allocated here because stack overflowed :sunglasses: */
std::deque<sdk::c_player *> possible_targets;

void find_target ( ) {
	/* used for scaling ragebot features */
	const auto framerate_below_tickrate = hack::game::interfaces::global_vars->m_ipt > hack::game::interfaces::global_vars->m_abs_frametime;
	const auto framerate_below_expected = hack::game::interfaces::global_vars->m_ipt * 2 > hack::game::interfaces::global_vars->m_abs_frametime;

	auto target_update_tick = true;
	
	/* only update constantly if our fps is good */
	if ( framerate_below_tickrate ) {
		target_update_tick = !( hack::game::interfaces::global_vars->m_tickcount % 3 );
	} else if ( framerate_below_expected ) {
		target_update_tick = !( hack::game::interfaces::global_vars->m_tickcount % 2 );
	}

	if ( target_update_tick ) {
		/* clear our last list */
		possible_targets.clear ( );

		/* scroll thru entities */
		sdk::foreach_player ( [ & ] ( sdk::c_player * player ) {
			if ( player->immune ( ) || !player->enemy ( ) )
				return;

			/* TODO: optimize this further to get just the max possible on valve maps */
			if ( player->absolute_origin ( ).dist_to ( hack::game::globals::local->absolute_origin ( ) ) > 4096.f )
				return;

			possible_targets.push_back ( player );
		} );

		auto eyeangs = c_vector3 {};
		hack::game::interfaces::engine->get_viewangles ( eyeangs );

		/* target selection */
		const auto & r_targetselection = CONFIG_GET ( int , fnv1a ( "r_targetselection" ) );

		switch ( r_targetselection ) {
			case 0: /* by distance to crosshair */
				std::sort ( possible_targets.begin ( ) , possible_targets.end ( ) , [ & ] ( sdk::c_player * a , sdk::c_player * b ) {
					/* calculate fov */
					const auto fov1 = sdk::math::get_fov ( hack::game::globals::local->eye_offset ( ) , a->absolute_origin ( ) , eyeangs );
					const auto fov2 = sdk::math::get_fov ( hack::game::globals::local->eye_offset ( ) , b->absolute_origin ( ) , eyeangs );

					return fov1 < fov2;
				} );
				break;
			case 1: /* by distance to origin */
				std::sort ( possible_targets.begin ( ) , possible_targets.end ( ) , [ & ] ( sdk::c_player * a , sdk::c_player * b ) {
					/* calculate fov */
					const auto dist1 = hack::game::globals::local->absolute_origin ( ).dist_to ( a->absolute_origin ( ) );
					const auto dist2 = hack::game::globals::local->absolute_origin ( ).dist_to ( b->absolute_origin ( ) );

					return dist1 < dist2;
				} );
				break;
			case 2: /* by lowest hp */
				std::sort ( possible_targets.begin ( ) , possible_targets.end ( ) , [ & ] ( sdk::c_player * a , sdk::c_player * b ) {
					/* calculate fov */
					const auto hp1 = a->health ( );
					const auto hp2 = b->health ( );

					return hp1 < hp2;
				} );
				break;
			case 3: /* by height */
				std::sort ( possible_targets.begin ( ) , possible_targets.end ( ) , [ & ] ( sdk::c_player * a , sdk::c_player * b ) {
					/* calculate fov */
					const auto height1 = a->absolute_origin ( ).z;
					const auto height2 = b->absolute_origin ( ).z;

					return height1 < height2;
				} );
				break;
		}
	}

	/* we sorted our targets... find viable ones to shoot at */
	/* do any of our targets have a shot record? */
	sdk::foreach_player ( [ & ] ( sdk::c_player * player ) {
		if ( player->immune ( ) || !player->enemy ( ) )
			return;

		/* TODO: optimize this further to get just the max possible on valve maps eg. bottom of pit to a site or something like that */
		if ( player->absolute_origin ( ).dist_to ( hack::game::globals::local->absolute_origin ( ) ) > 4096.f )
			return;

		/* search thru all records */
		for ( const auto & iter : hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames ) {
			if ( iter.m_shot_record ) {
				/* push it at the front so we scan it first */
				possible_targets.push_front ( player );
				break;
			}
		}
    } );

	/* are any players visible? if yes, prioritize them */

	/* we have a maximum amount of targets to hitscan so our fps doesn't go to shit... */
	auto max_hitscan_targets = 5;

	/* framerate based scaling for our hitscan so we know how many targets we should try to target */
	if ( framerate_below_tickrate ) {
		max_hitscan_targets = 2;
	} else if ( framerate_below_expected ) {
		target_update_tick = 3;
	}

	auto scanned_targets = 0;

	const auto & rc_mindmg = CONFIG_GET ( int , fnv1a ( "rc_mindmg" ) );
	const auto required_minimum_damage = static_cast< float >( rc_mindmg );

	/* we have our targets sorted... let's start hitscan */
	for ( const auto & player : possible_targets ) {
		/* sanity checks */
		if ( !player->valid ( ) || !player->immune ( ) )
			continue;

		/* store current enemy record */
		auto current_record = hack::modules::animations::anim_frame_t {};
		current_record.store ( player );

		/* grab first and last record for current target */
		auto & first_record = hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames.front ( ) ,
			& last_record = hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames.back ( );

		/* get hitboxes */
		auto hitboxes_to_scan = get_hitboxes ( hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_missed_shots );

		const auto scan_current_record = [ & ] ( ) {
			auto best_dmg = required_minimum_damage;

			/* look thru hitboxes to find our best */
			for ( const auto & hitbox : hitboxes_to_scan ) {
				/* run multipoint */

				/* run autowall for each hitbox */

				/* TODO: safepoint! - check hit hitgroup and scale damage... */

				/* is our damage above mindmg? return */
			} 
		};

		auto damage = required_minimum_damage;

		/* TODO: scan shot record */

		/* scan first record */
		first_record.apply ( player );


		/* scan last record */
		last_record.apply ( player );

		/* restore enemy record */
		current_record.apply ( player );

		scanned_targets++;

		/* stop */
		if ( scanned_targets >= max_hitscan_targets )
			break;
	}
}

void hack::modules::rage::ragebot::in_prediction ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	if ( hack::game::globals::local->movetype ( ) == sdk::movetypes::movetype_observer ||
		 hack::game::globals::local->movetype ( ) == sdk::movetypes::movetype_noclip ||
		 hack::game::globals::local->movetype ( ) == sdk::movetypes::movetype_ladder )
		return;

	const auto & r_enabled = CONFIG_GET ( bool , fnv1a ( "r_enabled" ) );

	if ( !r_enabled )
		return;


}

void hack::modules::rage::ragebot::in_endscene ( ) { 

}
