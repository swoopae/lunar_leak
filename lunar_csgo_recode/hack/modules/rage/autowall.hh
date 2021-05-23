#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include "../../../sdk/weapon.hh"

#include <functional>

namespace hack::modules::rage::autowall {
	struct wall_pen_t {
		float m_damage;
		sdk::hitboxes m_hitbox;
		int32_t m_hitgroup;
	};

	void trace_line ( const c_vector3 & start , const c_vector3 & end , std::uint32_t mask , sdk::c_player * ignore , sdk::valve::c_trace * trace_pointer );
	bool trace_to_exit ( sdk::valve::c_trace & enter_trace , sdk::valve::c_trace & exit_trace , const c_vector3 & start , const c_vector3 & view_direction );
	float scale_damage ( sdk::c_player * target , float damage , float weapon_armor_ratio , int hitgroup , bool is_zeus );
	bool handle_bullet_penetration ( sdk::c_weaponinfo * data , sdk::valve::c_trace & enter_trace ,
									 c_vector3 & eye_position , const c_vector3 & direction , int & penetrate_count ,
									 float & current_damage , float penetration_power );
	wall_pen_t fire_bullet ( sdk::c_weaponinfo * data , c_vector3 & src ,
							 const c_vector3 & pos , sdk::valve::c_trace_filter * filter ,
							 sdk::c_player * target = nullptr , bool point = false , bool is_zeus = false );

	wall_pen_t run_autowall ( c_vector3 shoot_position , const c_vector3 & aimpos , sdk::c_player * target , sdk::c_player * override_player = nullptr );
}