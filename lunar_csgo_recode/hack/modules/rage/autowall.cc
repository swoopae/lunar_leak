#include "autowall.hh"

#include "../../../sdk/math/mathutils.hh"

void hack::modules::rage::autowall::trace_line ( const c_vector3 & start , const c_vector3 & end , std::uint32_t mask , sdk::c_player * ignore , sdk::valve::c_trace * trace_pointer ) {
	sdk::valve::ray_t ray { start , end };
	sdk::valve::c_trace_filter filter { ignore };

	hack::game::interfaces::engine_trace->trace_ray ( ray , mask , &filter , trace_pointer );
}

bool is_breakable ( sdk::c_entity * entity ) { 
	if ( !entity )
		return false;

	// backup original takedamage value
	const int o_take_dmg = entity->take_damage ( );
	const auto client_class = entity->client_class ( );

	if ( !client_class || !client_class->m_class_id )
		return false;

	// force set DAMAGE_YES for certain breakable entities (as props, doors, etc)
	if ( client_class->m_class_id == sdk::valve::cbreakablesurface || client_class->m_class_id == sdk::valve::cbreakableprop || client_class->m_class_id == sdk::valve::cbasedoor
		 || client_class->m_class_id == sdk::valve::cfuncbrush )
		entity->take_damage ( ) = 2;

	using fn = bool ( __thiscall * )( void * );
	static fn func = nullptr;

	if ( !func )
		func = reinterpret_cast< fn >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 51 56 8B F1 85 F6 74 68" ) ) );

	// restore original take damage
	entity->take_damage ( ) = o_take_dmg;
	return func ( entity );
}

bool hack::modules::rage::autowall::trace_to_exit ( sdk::valve::c_trace & enter_trace , sdk::valve::c_trace & exit_trace , const c_vector3 & start , const c_vector3 & view_direction ) {
	constexpr float max_dist = 90.f , ray_extend = 4.f;

	c_vector3 cur_start {} , cur_end {};

	float cur_dist = 0.f;
	int first_contents = 0;

	while ( cur_dist <= max_dist ) {
		cur_dist += ray_extend;
		cur_start = start + ( view_direction * cur_dist );

		if ( !first_contents )
			first_contents = hack::game::interfaces::engine_trace->get_point_contents ( cur_start , sdk::valve::mask::mask_shot_player );

		const int point_contents = hack::game::interfaces::engine_trace->get_point_contents ( cur_start , sdk::valve::mask::mask_shot_player );

		if ( !( point_contents & sdk::valve::mask::mask_shot_hull ) || point_contents & sdk::valve::contents::contents_hitbox && point_contents != first_contents ) {
			cur_end = cur_start - ( view_direction * ray_extend );
			trace_line ( cur_start , cur_end , sdk::valve::mask::mask_shot_player , nullptr , &exit_trace );

			if ( exit_trace.startsolid && exit_trace.surface.flags & sdk::valve::surf::surf_hitbox ) {
				trace_line ( cur_start , start , sdk::valve::mask::mask_shot_hull , ( sdk::c_player * )exit_trace.m_pent , &exit_trace );

				if ( exit_trace.didhit ( ) && !exit_trace.startsolid ) {
					cur_start = exit_trace.endpos;
					return true;
				}
			} else if ( exit_trace.didhit ( ) && !exit_trace.startsolid ) {
				if ( ( exit_trace.surface.flags & sdk::valve::surf::surf_nodraw ) && is_breakable ( exit_trace.m_pent ) && is_breakable ( enter_trace.m_pent ) ) {
					cur_start = exit_trace.endpos;
					return true;
				} else if ( !( exit_trace.surface.flags & sdk::valve::surf::surf_nodraw ) || ( ( enter_trace.surface.flags & sdk::valve::surf::surf_nodraw ) && 
							( exit_trace.surface.flags & sdk::valve::surf::surf_nodraw ) ) ) {
					if ( view_direction.dot_product ( exit_trace.plane.normal ) <= 1.f ) {
						cur_start -= view_direction * ( exit_trace.flfraction * ray_extend );
						return true;
					}
				}
			} else if ( enter_trace.didhitnonworldentity ( ) && is_breakable ( enter_trace.m_pent ) ) {
				exit_trace = enter_trace;
				exit_trace.endpos = cur_start + view_direction;
				return true;
			}
		}
	}

	return false;
}

float hack::modules::rage::autowall::scale_damage ( sdk::c_player * target , float damage , const float weapon_armor_ratio , int hitgroup , bool is_zeus ) {
	const auto is_armored = [ & ] ( ) -> bool {
		if ( target->armor ( ) > 0.f ) {
			switch ( hitgroup ) {
				case hitgroup_generic:
				case hitgroup_chest:
				case hitgroup_stomach:
				case hitgroup_leftarm:
				case hitgroup_rightarm:
					return true;
				case hitgroup_head:
					return target->helmet ( );
				default:
					break;
			}
		}

		return false;
	};

	if ( !is_zeus )
		switch ( hitgroup ) {
			case hitgroup_head:
				if ( target->heavy_armor ( ) )
					damage = ( damage * 4.f ) * .5f;
				else
					damage *= 4.f;
				break;
			case hitgroup_stomach:
				damage *= 1.25f;
				break;
			case hitgroup_leftleg:
			case hitgroup_rightleg:
				damage *= .75f;
				break;
			default:
				break;
		}

	if ( is_armored ( ) ) {
		auto modifier = 1.f , armor_bonus_ratio = .5f , armor_ratio = weapon_armor_ratio * .5f;

		if ( target->heavy_armor ( ) ) {
			armor_bonus_ratio = 0.33f;
			armor_ratio = ( weapon_armor_ratio * 0.5f ) * 0.5f;
			modifier = 0.33f;
		}

		auto new_damage = damage * armor_ratio;

		if ( target->heavy_armor ( ) )
			new_damage *= 0.85f;

		if ( ( damage - damage * armor_ratio ) * ( modifier * armor_bonus_ratio ) > target->armor ( ) )
			new_damage = damage - target->armor ( ) / armor_bonus_ratio;

		damage = new_damage;
	}

	return damage;
}

bool hack::modules::rage::autowall::handle_bullet_penetration ( sdk::c_weaponinfo * data , sdk::valve::c_trace & enter_trace , c_vector3 & eye_position , const c_vector3 & direction , int & penetrate_count , float & current_damage , float penetration_power ) {
	static const auto ff_damage_reduction_bullets = ( hack::game::interfaces::convar_system->get_convar ( STR ( "ff_damage_reduction_bullets" ) ) );
	static const auto ff_damage_bullet_penetration = ( hack::game::interfaces::convar_system->get_convar ( STR ( "ff_damage_bullet_penetration" ) ) );

	const auto damage_reduction_bullets = ff_damage_reduction_bullets->get_float ( );
	const auto damage_bullet_penetration = ff_damage_bullet_penetration->get_float ( );

	sdk::valve::c_trace exit_trace {};
	auto enemy = reinterpret_cast< sdk::c_player * >( enter_trace.m_pent );
	const auto enter_surface_data = hack::game::interfaces::surface_props->get_surface_data ( enter_trace.surface.surface_props );
	const int enter_material = enter_surface_data->game.material;

	const auto enter_surf_penetration_modifier = enter_surface_data->game.penetration_mod;
	float final_damage_modifier , combined_penetration_modifier;
	const bool is_solid_surf = enter_trace.contents >> 3 & sdk::valve::contents_solid;
	const bool is_light_surf = enter_trace.surface.flags >> 7 & sdk::valve::surf_light;

	if ( ( !penetrate_count && !is_light_surf && !is_solid_surf && enter_material != char_tex_grate && enter_material != char_tex_glass )
		 || data->flpenetration <= 0.f
		 || ( !trace_to_exit ( enter_trace , exit_trace , enter_trace.endpos , direction )
		 && !( hack::game::interfaces::engine_trace->get_point_contents ( enter_trace.endpos , sdk::valve::mask_shot_hull ) & sdk::valve::mask_shot_hull ) ) )
		return false;

	const auto exit_surface_data = hack::game::interfaces::surface_props->get_surface_data ( exit_trace.surface.surface_props );
	const auto exit_material = exit_surface_data->game.material;
	const auto exit_surf_penetration_modifier = exit_surface_data->game.penetration_mod;

	if ( enter_material == char_tex_grate || enter_material == char_tex_glass ) {
		combined_penetration_modifier = 3.f;
		final_damage_modifier = 0.05f;
	} else if ( is_solid_surf || is_light_surf ) {
		combined_penetration_modifier = 1.f;
		final_damage_modifier = 0.16f;
	} else if ( enter_material == char_tex_flesh && enemy->teammate() && damage_reduction_bullets == 0.f ) {
		if ( damage_bullet_penetration == 0.f )
			return false;

		combined_penetration_modifier = damage_bullet_penetration;
		final_damage_modifier = 0.16f;
	} else {
		combined_penetration_modifier = ( enter_surf_penetration_modifier + exit_surf_penetration_modifier ) / 2.f;
		final_damage_modifier = 0.16f;
	}

	if ( enter_material == exit_material ) {
		if ( exit_material == char_tex_cardboard || exit_material == char_tex_wood )
			combined_penetration_modifier = 3.f;
		else if ( exit_material == char_tex_plastic )
			combined_penetration_modifier = 2.f;
	}

	auto thickness = ( exit_trace.endpos - enter_trace.endpos ).length ( );
	thickness *= thickness;
	thickness *= fmaxf ( 0.f , 1.0f / combined_penetration_modifier );
	thickness /= 24.0f;

	const auto lost_damage = fmaxf ( 0.0f , current_damage * final_damage_modifier + fmaxf ( 0.f , 1.0f / combined_penetration_modifier )
									 * 3.0f * fmaxf ( 0.0f , 3.0f / penetration_power ) * 1.25f + thickness );

	if ( lost_damage > current_damage )
		return false;

	if ( lost_damage > 0.f )
		current_damage -= lost_damage;

	if ( current_damage < 1.f )
		return false;

	eye_position = exit_trace.endpos;
	--penetrate_count;
	return true;
}

hack::modules::rage::autowall::wall_pen_t hack::modules::rage::autowall::fire_bullet ( sdk::c_weaponinfo * data , c_vector3 & src ,
																					   const c_vector3 & pos , sdk::valve::c_trace_filter * filter ,
																					   sdk::c_player * target , bool point , bool is_zeus ) {
	if ( !data )
		return wall_pen_t { 0.f, sdk::hitboxes::max, 0 };

	if ( !hack::game::globals::local || !hack::game::globals::local->alive ( ) )
		return wall_pen_t { 0.f, sdk::hitboxes::max, 0 };

	c_vector3 angles;
	sdk::math::vector_angles ( pos - src , angles );

	if ( !angles.is_valid ( ) )
		return wall_pen_t { 0.f, sdk::hitboxes::max, 0 };

	c_vector3 direction;
	sdk::math::angle_vectors ( angles , direction );

	if ( !direction.is_valid ( ) )
		return wall_pen_t { 0.f, sdk::hitboxes::max, 0 };

	direction.a_normalize ( );

	auto penetrate_count = 5;
	auto length = 0.f , damage = static_cast< float >( data->idamage );
	sdk::valve::c_trace enter_trace {};

	const auto start = src;

	while ( penetrate_count > 0 && damage >= 1.0f ) {
		const auto length_remaining = data->flrange - length;
		auto end = src + direction * length_remaining;

		sdk::valve::ray_t r { src , end };

		r.init ( src , end );
		filter->pskip1 = hack::game::globals::local;

		hack::game::interfaces::engine_trace->trace_ray ( r , sdk::valve::mask_shot_player , filter , &enter_trace );

		if ( enter_trace.flfraction == 1.f && !point )
			break;

		if ( point && ( enter_trace.flfraction == 1.f ||
			 ( start - enter_trace.endpos ).length ( ) > ( start - pos ).length ( ) ) ) {
			return wall_pen_t {
				scale_damage ( target, damage, data->flarmorratio, hitgroup_head, is_zeus ),
				sdk::hitboxes::head, hitgroup_head };
		}

		auto end_extended = end + direction * 40.f;

		/* TODO: rebuild for fast or just multithread autowall all together lol */
		hack::game::interfaces::engine_trace->clip_ray_to_player ( src , end_extended , sdk::valve::mask_shot_player , target , filter , &enter_trace );

		length += enter_trace.flfraction * length_remaining;
		damage *= std::powf ( data->flrangemodifier , length * .002f );

		if ( enter_trace.hitgroup <= 7 && enter_trace.hitgroup > 0 ) {
			if ( !enter_trace.m_pent || enter_trace.m_pent != target )
				break;

			return wall_pen_t {
				scale_damage ( target, damage, data->flarmorratio, enter_trace.hitgroup, is_zeus ),
				static_cast< sdk::hitboxes >( enter_trace.hitbox ), enter_trace.hitgroup };
		}

		const auto enter_surface = hack::game::interfaces::surface_props->get_surface_data ( enter_trace.surface.surface_props );

		if ( !enter_surface || enter_surface->game.penetration_mod < .1f )
			break;

		if ( !handle_bullet_penetration ( data , enter_trace , src , direction , penetrate_count , damage , data->flpenetration ) )
			break;
	}

	return wall_pen_t { 0.f, sdk::hitboxes::max, 0 };
}

hack::modules::rage::autowall::wall_pen_t hack::modules::rage::autowall::run_autowall ( c_vector3 shoot_position , const c_vector3 & aimpos , 
																						sdk::c_player * target , sdk::c_player * override_player ) {
	static sdk::c_weaponinfo override_gun {};
	override_gun.idamage = 15000;
	override_gun.flrangemodifier = 1.0f;
	override_gun.flpenetration = 10.0f;
	override_gun.flarmorratio = 2.0f;
	override_gun.flrange = 8192.f;
	override_gun.m_pweapondef = reinterpret_cast< sdk::strike_weapon_definition_t * >( 1 );

	wall_pen_t result = wall_pen_t { 0.f, sdk::hitboxes::max, 0 };

	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return result;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return result;

	const auto weapon = hack::game::globals::local->active_weapon ( );

	if ( !weapon && !override_player )
		return result;

	const auto data = override_player ? &override_gun : weapon->data ( );

	if ( !data )
		return result;

	if ( !override_player ) {
		sdk::valve::c_trace_filter tr_filter ( hack::game::globals::local );
		result = fire_bullet ( data , shoot_position , aimpos ,
							   &tr_filter , target );
	} else {
		sdk::valve::c_trace_filter filter ( override_player );
		result = fire_bullet ( data , shoot_position , aimpos , &filter , override_player , true );
	}

	if ( result.m_damage < 1.f )
		return wall_pen_t { 0.f, sdk::hitboxes::max, 0 };

	return result;
}
