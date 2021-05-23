#include "others.hh"

#include "../rage/antiaim.hh"

#include "../../menu/framework.hh"
#include "../../../sdk/math/mathutils.hh"

#define	MAX_EDICTS (1<<11)

void hack::modules::misc::others::in_endscene ( ) {
	const auto handle_key = HANDLE_KEYBIND ( fnv1a ( "fx_thirdperson_key" ) );

	thirdperson_key = handle_key ( );
}

void do_clantag ( ) { /* lol no */ }

void do_precipitation ( int stage ) {
	static bool created_rain = false;
	static void * rain_networkable = nullptr;
	static sdk::c_precipitation_entity * rain_ent = nullptr;
	static sdk::valve::client_class_t * precipitation_client_class = nullptr;

	const auto & fx_precipitation = CONFIG_GET ( bool , fnv1a ( "fx_precipitation" ) );

	if ( !fx_precipitation ) {
		if ( created_rain && rain_ent ) rain_ent->precipitation_type ( ) = sdk::e_precipitation_types::precipitation_type_particlesnow;
		return;
	}

	if ( created_rain && rain_ent ) rain_ent->precipitation_type ( ) = sdk::e_precipitation_types::precipitation_type_snow;

	if ( !precipitation_client_class )
		for ( auto pclass = hack::game::interfaces::client->get_all_classes ( ); pclass && !precipitation_client_class; pclass = pclass->m_next )
			if ( pclass->m_class_id == sdk::valve::class_id::cprecipitation )
				precipitation_client_class = pclass;

	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) || !hack::game::globals::local ) {
		rain_networkable = rain_ent = nullptr;
		created_rain = false;
	} else {
		if ( !created_rain && precipitation_client_class && precipitation_client_class->m_create_func && hack::game::globals::local && stage == 5 ) {
			rain_networkable = ( ( void * ( * )( int , int ) )precipitation_client_class->m_create_func )( MAX_EDICTS - 1 , 0 );

			if ( rain_networkable ) {
				rain_ent = hack::game::interfaces::entity_list->get< sdk::c_precipitation_entity * > ( MAX_EDICTS - 1 );

				rain_ent->precipitation_type ( ) = sdk::e_precipitation_types::precipitation_type_snow;

				rain_ent->pre_data_update ( 0 );
				rain_ent->on_pre_data_change ( 0 );

				rain_ent->collision ( ).mins ( ) = c_vector3 ( -16384.0f , -16384.0f , -16384.0f );
				rain_ent->collision ( ).maxs ( ) = c_vector3 ( 16384.0f , 16384.0f , 16384.0f );

				rain_ent->on_post_data_change ( 0 );
				rain_ent->post_data_update ( 0 );

				created_rain = true;
			}
		}
	}

	// if ( created_rain && rain_networkable && rain_ent && stage == 1 ) {
	// 	/* A1 ? ? ? ? 83 C1 F4 */
	// 	/* xref: 'Rain simulation: %du (%d tracers)' xref the callee of the func you get */
	// 	static auto fn = reinterpret_cast< void ( __thiscall * )( void * ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "A1 ? ? ? ? 83 C1 F4" ) ) );
	// 	fn ( rain_ent + 12 );
	// }
}

void hack::modules::misc::others::in_createmove ( ) {
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) || !hack::game::globals::local )
		return;

	do_clantag ( );

	/* gather info for drawing use */
	sdk::foreach_player ( [ & ] ( sdk::c_player * player ) {
		if ( !player->active_weapon ( ) )
			return;

		weapon_name.at ( player->index ( ) ) = player->active_weapon ( )->get_weapon_as_string ( );
	} );
}

void hack::modules::misc::others::in_fsn ( int stage ) {
	do_precipitation ( stage );
}

void do_thirdperson ( ) {
	static float local_interp_timer = hack::menu::globals::frametime;

	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) || !hack::game::globals::local ) {
		hack::game::interfaces::input->m_camera_in_thirdperson = false;
		local_interp_timer = 0.f;
		return;
	}

	const auto & fx_thirdperson = CONFIG_GET ( bool , fnv1a ( "fx_thirdperson" ) );
	const auto & fx_thirdperson_on_dead = CONFIG_GET ( bool , fnv1a ( "fx_thirdperson_on_dead" ) );
	const auto & fx_thirdperson_value = CONFIG_GET ( int , fnv1a ( "fx_thirdperson_value" ) );

	if ( !fx_thirdperson ) {
		hack::game::interfaces::input->m_camera_in_thirdperson = false;
		local_interp_timer = 0.f;
		return;
	}

	if ( !hack::game::globals::local->alive ( ) && fx_thirdperson_on_dead && hack::game::globals::local->observer_mode ( ) == 4 ) {
		/* force thirdperson on dead */
		hack::game::interfaces::input->m_camera_in_thirdperson = false;
		hack::game::globals::local->observer_mode ( ) = 5;
		local_interp_timer = 0.f;
		return;
	} else if ( !hack::game::globals::local->alive ( ) && !fx_thirdperson_on_dead && hack::game::globals::local->observer_mode ( ) == 5 ) {
		hack::game::interfaces::input->m_camera_in_thirdperson = false;
		hack::game::globals::local->observer_mode ( ) = 4;
		local_interp_timer = 0.f;
		return;
	}

	if ( !hack::game::globals::local->alive ( ) ) {
		hack::game::interfaces::input->m_camera_in_thirdperson = false;
		local_interp_timer = 0.f;
		return;
	}

	if ( !hack::modules::misc::others::thirdperson_key ) {
		hack::game::interfaces::input->m_camera_in_thirdperson = false;
		local_interp_timer = 0.f;
		return;
	}

	c_vector3 angles;
	hack::game::interfaces::engine->get_viewangles ( angles );

	auto get_correct_distance = [ & ] ( float ideal_distance ) -> float {
		if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) || !hack::game::globals::local || !hack::game::globals::local->valid ( ) )
			return 0.f;

		c_vector3 angle;
		hack::game::interfaces::engine->get_viewangles ( angle );

		angle.x *= -1.f , angle.y += 180.f;

		c_vector3 direction;
		sdk::math::angle_vectors ( angle , direction );

		sdk::valve::ray_t ray ( hack::game::globals::local->eye_offset ( ) , hack::game::globals::local->eye_offset ( ) + ( direction * ideal_distance ) );
		sdk::valve::c_trace trace;
		sdk::valve::c_trace_world_only filter ( hack::game::globals::local );

		filter.pskip1 = hack::game::globals::local;

		hack::game::interfaces::engine_trace->trace_ray ( ray , sdk::valve::mask_shot , &filter , &trace );

		return ( ideal_distance * trace.flfraction ) - 10.f;
	};

	angles.z = sdk::math::interpolate ( angles.z , get_correct_distance ( fx_thirdperson_value ) , hack::menu::utils::ease_out_quart ( local_interp_timer * 2.f ) );

	local_interp_timer += hack::menu::globals::frametime;

	local_interp_timer = std::clamp ( local_interp_timer , 0.f , 0.5f );

	hack::game::interfaces::input->m_camera_in_thirdperson = true;
	hack::game::interfaces::input->m_camera_offset = angles;
}

void hack::modules::misc::others::in_overrideview ( sdk::valve::c_viewsetup * setup ) {
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::globals::local )
		return;

	do_thirdperson ( );

	const auto & rage_fakeduck = CONFIG_GET ( bool , fnv1a ( "rage_fakeduck" ) );
	
	/* fix fakeduck view being fucky wucky uwu */	
	if ( rage_fakeduck && hack::modules::rage::antiaim::rage_fakeduck_keystate ) {
		/* todo: do it properly lol... ~swoopae */
		foffset ( float , setup , 192 ) = hack::game::globals::local->origin ( ).z + 64.f;
	}
}