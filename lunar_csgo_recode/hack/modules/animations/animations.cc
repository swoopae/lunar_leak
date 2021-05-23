#include "animations.hh"

#include "../prediction/engine_prediction.hh"
#include "../rage/antiaim.hh"

#include "../../../sdk/weapon.hh"
#include "../../../sdk/math/mathutils.hh"

#include "../../menu/config.hh"

void build_matrix ( sdk::c_player * player , c_matrix3x4 * m , std::uint32_t max , std::uint32_t mask , float time ) {
	/* backup info */
	const auto backup_eflags = *( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xE8 );
	const auto backup_readable = *( std::uint32_t * ) ( ( std::uintptr_t ) player + 0x26AC );
	const auto backup_prev_bone_mask = *( std::uint32_t * ) ( ( std::uintptr_t ) player + 0x269C );
	const auto old_occlusion_frame = foffset ( int , player , 0xA30 ) , old_computed_lod_frame = foffset ( int , player , 0xA28 );

	/* remove interpolation of the matrix */
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xE8 ) |= 8; // AddFlag( EFL_SETTING_UP_BONES );

	/* make sure attachment helper gets called */
	/* previous bone mask */
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0x269C ) |= 512;
	/* readable bones */
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0x26AC ) &= ~512;
	/* EFL_NO_INTERP */
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xF0 ) |= 8;

	player->set_absolute_origin ( player->origin ( ) );

	/* invalidate bone cache */
	player->invalidate_bone_cache ( );

	if ( hack::game::globals::local != player ) {
		foffset ( int , player , 0xA30 ) = hack::game::interfaces::global_vars->m_framecount;
		foffset ( int , player , 0xA28 ) = 0;
	}

	/* allow setup */
	hack::modules::animations::allow_setup_bones = true;
	player->setup_bones ( m , max , mask , time );
	hack::modules::animations::allow_setup_bones = false;

	if ( hack::game::globals::local != player ) {
		foffset ( int , player , 0xA30 ) = old_occlusion_frame;
		foffset ( int , player , 0xA28 ) = old_computed_lod_frame;
	}

	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xF0 ) &= ~8;

	/* restore info */
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0x269C ) = backup_prev_bone_mask;
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0x26AC ) = backup_readable;
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xE8 ) = backup_eflags;
}

/* UNUSED LEGACY CODE! */
void build_local_matrix ( sdk::c_player * player , c_matrix3x4 * m , std::uint32_t max , std::uint32_t mask , float time ) {
	/* this may be incomplete and some of it is pasted from supremacy, i don't give a shit though it looks good enough ~swoopae */
	if ( !player || !player->valid ( ) )
		return;

	/* get model */
	auto studiohdr = player->studio_model ( );
	if ( !studiohdr )
		return;

	/* get bone accesor as pointer */
	auto accessor = &player->bone_accesor ( );
	if ( !accessor )
		return;

	/* cache old bone data */
	auto backup_matrix = accessor->m_bones;
	if ( !backup_matrix )
		return;

	/* lambda i just plop in here cuz i have nowhere else to put it cuz i cba to reverse it ~swoopae */
	const auto angle_matrix = [ ] ( const c_vector3& angle , const c_vector3& position , c_matrix3x4& out ) {
		const static auto fn = reinterpret_cast< void ( __fastcall * )( const c_vector3 & , c_matrix3x4 & ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 EC 24 F3 0F 10 41 ? 8D 45 EC F3 0F 10 0D ? ? ? ? F3 0F 59 C1 89 45 F4 8D 45 E8 56 8B F2 89 45 F0 F3 0F 11 45 ? D9 45 F8 D9 FB 8B 55 F4 8B 45 F0 D9 1A D9 18 F3 0F 10 01 8D 45 FC F3 0F 59" ) ) );
		fn ( angle , out );

		/* set matrix origin */
		out [ 0 ][ 3 ] = position.x;
		out [ 1 ][ 3 ] = position.y;
		out [ 2 ][ 3 ] = position.z;
	};

	auto abs_origin = player->absolute_origin ( ) , abs_angles = player->absolute_angles ( );

	c_matrix3x4 transf;

	angle_matrix ( abs_angles , abs_origin , transf );

	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xE8 ) |= 8; // AddFlag( EFL_SETTING_UP_BONES );

	player->set_absolute_origin ( abs_origin );
	player->set_absolute_angles ( abs_angles.y );

	const auto backup_occlusion_flags = *( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xA28 );

	/* occlusion flags - skips accumulatelayers call - failsafe in case our returnaddress ishltv meme doesn't work for some reason */
	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xA28 ) |= 10;

	/* i mean i guess we could just call the hook directly */
	player->standard_blending_rules ( studiohdr , hack::modules::animations::local_bones_pos , hack::modules::animations::local_bones_quat , time , mask );

	*( std::uint32_t * ) ( ( std::uintptr_t ) player + 0xA28 ) = backup_occlusion_flags;

	/* lol */
	accessor->m_bones = m;

	std::uint8_t computed [ 256 ];
	std::memset ( computed , 0 , 256 );

	player->build_transformations ( studiohdr , hack::modules::animations::local_bones_pos , hack::modules::animations::local_bones_quat , transf , mask , computed );

	/* reset our data */
	accessor->m_bones = backup_matrix;

	player->set_absolute_origin ( abs_origin );
	player->set_absolute_angles ( abs_angles.y );
}

void hack::modules::animations::build_custom_matrix ( sdk::c_player * player , c_matrix3x4 * m , std::uint32_t max , std::uint32_t mask , float time ) {
	build_matrix ( player , m , max , mask , time );
}

void hack::modules::animations::in_createmove ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) ) {
		hack::modules::animations::got_layers = false;
		return;
	}

	if ( !hack::game::globals::local || !hack::game::globals::local->valid ( ) || !hack::game::globals::cmd ) {
		hack::modules::animations::got_layers = false;
		return;
	}

	hack::modules::animations::last_origin = hack::game::globals::local->absolute_origin ( );

	/* backup globals */
	const auto old_frametime = hack::game::interfaces::global_vars->m_frametime;

	/* set globals correctly */
	hack::game::interfaces::global_vars->m_frametime = hack::game::interfaces::global_vars->m_ipt;

	constexpr auto pose_jump_fall = [ ] ( sdk::c_player * ent , float air_time ) {
		auto airtime = ( air_time - 0.72f ) * 1.25f;
		auto clamped = airtime >= 0.0f ? std::min< float > ( airtime , 1.0f ) : 0.0f;
		auto jump_fall = ( 3.0f - ( clamped + clamped ) ) * ( clamped * clamped );

		if ( jump_fall >= 0.0f )
			jump_fall = std::min< float > ( jump_fall , 1.0f );

		ent->pose_parameter ( ) [ 6 ] = jump_fall;
	};

	// if ( !hack::game::globals::local->animstate ( )->m_on_ground )
	pose_jump_fall ( hack::game::globals::local , hack::game::globals::local->anim_overlays ( ) [ 4 ].m_cycle );

	/* animate the last cmd in the batch (the current one) */
	std::memcpy ( old_layers , hack::game::globals::local->anim_overlays ( ) , ( sizeof ( sdk::animlayer_t ) * 13 ) );

	/* update player with current data */
	hack::game::globals::local->clientanims ( ) = hack::modules::animations::allow_clientside_anims = hack::modules::animations::in_local_animfix = true;

	/* absvelocity fix */
	foffset ( int , hack::game::globals::local , 0xE8 ) &= ~0x1000; /* DIRTY_ABSVEL */

	hack::game::globals::local->force_velocity ( hack::game::globals::local->velocity ( ) );

	/* funny leg fix */
	// hack::game::globals::local->animstate ( )->m_feet_yaw_rate = 0.f;

	/* allow reanimating same tick just in case */
	hack::game::globals::local->animstate ( )->m_last_client_side_update_frame_count -= 1;

	hack::game::globals::local->animstate ( )->update ( hack::game::globals::send_packet ? hack::modules::rage::antiaim::last_sent_angles : hack::modules::rage::antiaim::last_choked_angles );

	/* for taser animation / proper viewmodel */
	hack::game::globals::local->update_anims ( );

	hack::game::globals::local->clientanims ( ) = hack::modules::animations::allow_clientside_anims = hack::modules::animations::in_local_animfix = false;

	/* set gfy and pose parameters correctly */
	if ( hack::game::globals::send_packet ) {
		last_sent_absyaw = hack::game::globals::local->animstate ( )->m_goal_feet_yaw;
		hack::modules::animations::got_layers = true;
		std::memcpy ( &networked_poses , reinterpret_cast< void * >( &hack::game::globals::local->pose_parameter ( ) ) , sizeof ( float ) * 24 );
	}

	/* restore animlayers and pose parameters */
	std::memcpy ( hack::game::globals::local->anim_overlays ( ) , old_layers , ( sizeof ( sdk::animlayer_t ) * 13 ) );
	std::memcpy ( reinterpret_cast< void * >( &hack::game::globals::local->pose_parameter ( ) ) , &networked_poses , sizeof ( float ) * 24 );

	/* restore globals */
	hack::game::interfaces::global_vars->m_frametime = old_frametime;

	/* prepare for setting up bones */
	// if ( hack::game::globals::local->anim_overlays ( ) )
 	// 	hack::game::globals::local->anim_overlays ( ) [ 12 ].m_weight = 0.f;

	/* set proper info */
	hack::game::globals::local->set_absolute_angles ( hack::modules::animations::last_sent_absyaw );

	/* setup bones this tick */
	build_matrix ( hack::game::globals::local , hack::modules::animations::local_real_bones , 128 , 0x7ff00 , hack::game::interfaces::global_vars->m_curtime );

	// if ( hack::game::globals::local->bone_accesor ( ).m_bones )
	// 	std::memcpy ( hack::game::globals::local->bone_accesor ( ).m_bones , hack::game::globals::local->bone_cache ( ).base ( ) , sizeof ( c_matrix3x4 ) * hack::game::globals::local->bone_cache ( ).get_count ( ) );
}

void force_required_cvars ( ) {
	/* grab needed cvars */
	static auto r_jiggle_bones = hack::game::interfaces::convar_system->get_convar ( STR ( "r_jiggle_bones" ) ); /* makes attachments look really wacky */
	static auto cl_foot_contact_shadows = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_foot_contact_shadows" ) ); /* ugly foot shadows that sometimes display inaccurate data */
	static auto cl_interp = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_interp" ) ); /* self-explanatory TODO: move this in level init */
	static auto cl_interp_ratio = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_interp_ratio" ) ); /* self-explanatory */
	static auto cl_extrapolate = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_extrapolate" ) ); /* does pretty much nothing as far as i know */
	static auto cl_extrapolate_amount = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_extrapolate_amount" ) ); /* actually disables extrapolation */
	static auto cl_pred_optimize = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_pred_optimize" ) ); /* no prediction optimization */

#ifdef _DEBUG
	static auto cl_camera_height_restriction_debug = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_camera_height_restriction_debug" ) ); /* b r u h */
	cl_camera_height_restriction_debug->set_value ( true );
#endif

	/* set needed values */
	cl_pred_optimize->set_value ( 0 );
	// r_jiggle_bones->set_value ( 0 );
	cl_foot_contact_shadows->set_value ( 0 );
	// cl_extrapolate->set_value ( 0 );
	// cl_extrapolate_amount->set_value ( 0.f );
	cl_interp->set_value ( hack::game::interfaces::global_vars->m_ipt );
	cl_interp_ratio->set_value ( 2.0f );
}

/* meme :^) */
void force_disable_interpolation ( ) {
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::local->valid ( ) || !hack::game::globals::cmd )
		return;

	struct var_map_entry_t {
		unsigned short m_type;
		bool m_interpolate;
		pad ( 8 );
	};

	struct var_mapping_t {
		var_map_entry_t * m_entries;
		int m_interpolated_entries;
		float m_last_interp_time;
	};

	sdk::foreach_player ( [ & ] ( sdk::c_player * player ) {
		const auto map = reinterpret_cast< var_mapping_t * >( player + 36 );

		for ( auto i = 0; i < map->m_interpolated_entries; i++ ) {
			var_map_entry_t * e = &map->m_entries [ i ];

			if ( !e )
				continue;

			e->m_interpolate = false;
		}
	} );
}

void on_debug ( ) {
	auto util_playerbyindex = [ ] ( int idx ) {
		using player_by_index_fn = sdk::c_player * ( __fastcall * )( int );
		static auto fn = player_by_index_fn ( sig ( MODULE_SERVER , STR ( "85 C9 7E 2A A1" ) ) );
		return fn ( idx );
	};

	return;

	static auto draw_hitboxes = std::uintptr_t ( sig ( MODULE_SERVER , STR ( "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" ) ) );

	const auto dur = hack::game::interfaces::global_vars->m_abs_frametime / 4;

	for ( auto i = 1; i <= hack::game::interfaces::global_vars->m_max_clients; i++ ) {
		auto e = hack::game::interfaces::entity_list->get< sdk::c_player * > ( i );

		if ( !e )
			continue;

		const auto server_ent = util_playerbyindex ( e->index ( ) );

		if ( !server_ent )
			continue;

		__asm {
			pushad
			movss xmm1 , dur
			push 0
			mov ecx , e
			call draw_hitboxes
			popad
		}
	}
}

void handle_local_animations ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::local->valid ( ) || !hack::game::globals::cmd )
		return;

	/* disable clientside anims */
	// hack::game::globals::local->clientanims ( ) = hack::modules::animations::allow_clientside_anims = hack::modules::animations::in_local_animfix = false;
	
	/* set proper info */
	hack::game::globals::local->set_absolute_angles ( hack::modules::animations::last_sent_absyaw );

	/* new tick, build bones */
	// if ( hack::modules::animations::new_tick ) {
	// 	/* restore pose parameters */
	// 	pose_params backup_pose_params;
	// 
	// 	std::memcpy ( &backup_pose_params , reinterpret_cast< void * >( &hack::game::globals::local->pose_parameter ( ) ) , sizeof ( float ) * 24 );
	// 	std::memcpy ( reinterpret_cast< void * >( &hack::game::globals::local->pose_parameter ( ) ) , &hack::modules::animations::networked_poses , sizeof ( float ) * 24 );
	// 
	// 	/* remove the ugly ass lean */
	// 	if ( hack::game::globals::local->anim_overlays ( ) )
	// 		hack::game::globals::local->anim_overlays ( ) [ 12 ].m_weight = 0.f;
	// 
	// 	/* finally build our bones */
	// 	build_matrix ( hack::game::globals::local , hack::modules::animations::local_real_bones , 128 , 0x7ff00 , hack::game::interfaces::global_vars->m_curtime );
	// 
	// 	/* restore to original state */
	// 	std::memcpy ( reinterpret_cast< void * >( &hack::game::globals::local->pose_parameter ( ) ) , &backup_pose_params , sizeof ( float ) * 24 );
	// 
	// 	/* set origin */
	// 	hack::modules::animations::last_origin = c_vector3 ( hack::modules::animations::local_real_bones [ 0 ][ 0 ][ 3 ] , hack::modules::animations::local_real_bones [ 0 ][ 1 ][ 3 ] , hack::modules::animations::local_real_bones [ 0 ][ 2 ][ 3 ] );
	// 
	// 	/* no longer a new tick */
	// 	hack::modules::animations::new_tick = false;
	// }
	// else {
	// 	/* force bone origin to what we want it to be */
	// 	for ( auto & i : hack::modules::animations::local_real_bones ) {
	//		i [ 0 ][ 3 ] -= hack::modules::animations::last_origin.x;
	//		i [ 1 ][ 3 ] -= hack::modules::animations::last_origin.y;
	//		i [ 2 ][ 3 ] -= hack::modules::animations::last_origin.z;
	//
	//		i [ 0 ][ 3 ] += hack::game::globals::local->render_origin ( ).x;
	//		i [ 1 ][ 3 ] += hack::game::globals::local->render_origin ( ).y;
	//		i [ 2 ][ 3 ] += hack::game::globals::local->render_origin ( ).z;
	//	}
	// 
	// 	/* make sure our attachements are okay */
	// 	build_matrix ( hack::game::globals::local , hack::modules::animations::local_real_bones , 128 , 0x200 , hack::game::globals::local->simulation_time ( ) );
	// 
	// 	/* set our new origin */
	// 	hack::modules::animations::last_origin = c_vector3 ( hack::modules::animations::local_real_bones [ 0 ][ 0 ][ 3 ] , hack::modules::animations::local_real_bones [ 0 ][ 1 ][ 3 ] , hack::modules::animations::local_real_bones [ 0 ][ 2 ][ 3 ] );
	// }
}

void update_player_animations ( sdk::c_player * player ) {
	/* backup old globals */
	const auto old_frametime = hack::game::interfaces::global_vars->m_frametime , old_curtime = hack::game::interfaces::global_vars->m_curtime;

	/* use proper values */
	hack::game::interfaces::global_vars->m_frametime = hack::game::interfaces::global_vars->m_ipt;
	hack::game::interfaces::global_vars->m_curtime = player->simulation_time ( );

	/* absvelocity fix */
	foffset ( int , player , 0xE8 ) &= ~0x1000; /* DIRTY_ABSVEL */

	player->force_velocity ( player->velocity ( ) );

	/* allow reanimating */
	player->animstate ( )->m_last_client_side_update_frame_count -= 1;

	/* animate */
	player->clientanims ( ) = hack::modules::animations::allow_clientside_anims = true;
	player->update_anims ( );
	player->clientanims ( ) = hack::modules::animations::allow_clientside_anims = false;

	/* invalidate physics */
	/* NOTE: ANGLES_CHANGED being present among the other flags will result in the function invalidating bones which prevents us from just using one call. pretty stupid fix but should not mess anything important up... please check baseentity_shared.cpp line 1670 @ source leak ~swoopae */
	/* ANIMATION_CHANGED without ANGLES_CHANGED or BOUNDS_CHANGED does nothing... why the fuck was nitro using it without BOUNDS_CHANGED in a call in supremacy src? am i missing something? */
	player->invalidate_physics ( 2 ); /* ANGLES_CHANGED */
	player->invalidate_physics ( 56 ); /* ANIMATION_CHANGED | BOUNDS_CHANGED | SEQUENCE_CHANGED */

	/* restore globals */
	hack::game::interfaces::global_vars->m_frametime = old_frametime;
	hack::game::interfaces::global_vars->m_curtime = old_curtime;
}

bool is_tick_valid ( int simtime ) {
	const auto lerp = [ ] ( ) {
		static auto interp_cvar = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_interp" ) );
		static auto updaterate_cvar = hack::game::interfaces::convar_system->get_convar ( STR ( "cl_updaterate" ) );

		const auto updaterate_value = updaterate_cvar->get_int ( );
		const auto interp_value = interp_cvar->get_float ( );

		const auto lerp = interp_value / updaterate_value;

		return ( lerp <= interp_value ) ? interp_value : lerp;
	};

	const auto channel_info = hack::game::interfaces::engine->get_net_channel_info ( );

	if ( !channel_info )
		return false;

	float max_unlag = 0.2f;
	static auto sv_maxunlag = hack::game::interfaces::convar_system->get_convar ( STR ( "sv_maxunlag" ) );

	if ( sv_maxunlag )
		max_unlag = sv_maxunlag->get_float ( );

	const float curtime = hack::game::interfaces::global_vars->m_curtime;

	const float correct = channel_info->get_avg_latency ( 1 ) + channel_info->get_avg_latency ( 0 ) + lerp ( );
	std::clamp ( correct , 0.f , max_unlag );

	return std::abs ( correct - ( curtime - simtime ) ) <= 0.2f;
}

/* this also handles lag compensation records */
void handle_enemy_animations ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) || !hack::game::globals::local ) {
		/* flush cached data */
		for ( auto& iter : hack::modules::animations::animation_system->m_animation_info ) {
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

		return;
	}

	/* we're dead or something else went wrong so flush cached animation records */
	if ( !hack::game::globals::local->valid ( ) ) {
		for ( auto & iter : hack::modules::animations::animation_system->m_animation_info ) {
			/* erase all records if we have any */
			if ( !iter.m_recieved_frames.empty ( ) )
				iter.m_recieved_frames.clear ( );
		}

		return;
	}

	/* go thru all players to see if anything important changed (new players, changed handles etc) */
	for ( auto i = 1; i <= hack::game::interfaces::global_vars->m_max_clients; i++ ) {
		/* grab player */
		const auto player = hack::game::interfaces::entity_list->get<sdk::c_player *> ( i );

		/* we don't care about teammates (or about ourselves, we handle our animations separately) */
		if ( player && !player->enemy ( ) ) {
			/* in case admins decide to change mp_teammates_are_enemies mid-game and we decide not to run animfix on this entity just let the game handle it for us */
			/* TODO: check in hooks for teammates (call orig if entity isn't handled by our anim system) ~swoopae */
			if ( player != hack::game::globals::local )
				player->clientanims ( ) = true;

			continue;
		}

		/* check if handle is invalid */
		if ( !player || player->handle ( ) != hack::modules::animations::animation_system->m_animation_info.at ( i ).m_handle ) {
			/* set handle and update player pointer if player exists */
			if ( player ) {
				hack::modules::animations::animation_system->m_animation_info.at ( i ).m_handle = player->handle ( );
				hack::modules::animations::animation_system->m_animation_info.at ( i ).m_player = player;
			}

			/* reset all records */
			if ( !hack::modules::animations::animation_system->m_animation_info.at ( i ).m_recieved_frames.empty ( ) )
				hack::modules::animations::animation_system->m_animation_info.at ( i ).m_recieved_frames.clear ( );

			continue;
		}

		/* check if spawntime changed */
		if ( player && player->spawntime ( ) != hack::modules::animations::animation_system->m_animation_info.at ( i ).m_spawntime ) {
			hack::modules::animations::animation_system->m_animation_info.at ( i ).m_spawntime = player->spawntime ( );

			/* reset player animstate */
			if ( player->animstate ( ) )
				player->animstate ( )->reset ( );

			/* reset resolver data */
			/* TODO: maybe this isn't such a good idea! but considering resolving desync is a fucking meme anyway i should be able to get away with this if we just safepoint his ass we should hit him in 2 shots ~swoopae */
			// hack::modules::animations::animation_system->m_animation_info.at ( i ).m_last_resolver_state = hack::modules::animations::e_resolver_state::resolver_auto;
			// hack::modules::animations::animation_system->m_animation_info.at ( i ).m_resolved_side = hack::modules::animations::e_resolver_side::side_auto;

			/* reset all records - this should be useless but who knows */
			if ( !hack::modules::animations::animation_system->m_animation_info.at ( i ).m_recieved_frames.empty ( ) )
				hack::modules::animations::animation_system->m_animation_info.at ( i ).m_recieved_frames.clear ( );

			continue;
		}

		/* i feel like i'm missing something... */
	}

	/* animate enemies properly */
	sdk::foreach_player ( [ & ] ( sdk::c_player * player ) {
		/* sanity checks */
		if ( hack::game::globals::local == player || !player->enemy ( ) )
			return;

		/* if we didn't get an update , don't do anything */
		if ( player->simulation_time ( ) == player->old_simulation_time ( ) )
			return;

		/* how many ticks did they choke? */
		/* this variable name is inaccurate, if they're not fakelagging they will still technically "choke" one tick so keep this in mind */
		auto ticks_choked = 1;

		auto diff = player->simulation_time ( ) - player->old_simulation_time ( );

		if ( !hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames.empty ( ) )
			ticks_choked = std::clamp ( time_to_ticks ( diff ) , 0 , 62 );

		/* store a backup frame */
		hack::modules::animations::anim_frame_t backup_frame;
		backup_frame.store ( player );

		/* create our new animation frame */
		hack::modules::animations::anim_frame_t current_animframe , last_animframe;
		current_animframe.store ( player );

		/* grab last animation frame if possible */
		if ( !hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames.empty ( ) )
			last_animframe = hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames.back ( );

		/* update animations and get bones */
		current_animframe.update ( player , &last_animframe , ticks_choked );

		/* genius hvh */
		hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames.push_back ( current_animframe );

		/* apply our original animation frame */
		backup_frame.apply ( player );

		/* check validity of records and erase them if invalid */
		auto & recieved_frames = hack::modules::animations::animation_system->m_animation_info.at ( player->index ( ) ).m_recieved_frames;

		for ( auto frame = recieved_frames.begin ( ); frame != recieved_frames.end ( ); ) {
			( !is_tick_valid ( frame->m_simulation_time ) ) ? frame = recieved_frames.erase ( frame ) : ++frame;
		}
	} );
}

void hack::modules::animations::in_framestagenotify ( int stage ) {
	if ( stage == 4 /* FRAME_NET_UPDATE_END */ ) {
		/* why are we doing this before sanity checks? because we need to clear out data on our own if we fail sanity checks */
		handle_enemy_animations ( );
	}

	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local )
		return;

	switch ( stage ) {
		case 2: /* FRAME_NET_UPDATE_POSTDATAUPDATE_START */
			force_disable_interpolation ( );
			force_required_cvars ( );
			break;
		case 4: /* FRAME_NET_UPDATE_END */
			hack::modules::prediction::engine_prediction::apply_netvars ( );
			break;
		case 5: /* FRAME_RENDER_START */
			handle_local_animations ( );
			on_debug ( );
			break;
	}
}

void hack::modules::animations::anim_frame_t::store ( sdk::c_player * player ) {
	this->m_absangs = player->absolute_angles ( );
	this->m_angs = player->eyeangles ( );
	this->m_duck = player->duck ( );
	this->m_flags = player->flags ( );
	this->m_maxs = player->collision ( ).maxs ( );
	this->m_mins = player->collision ( ).mins ( );
	this->m_origin = player->absolute_origin ( );
	this->m_poses = player->pose_parameter ( );
	this->m_simulation_time = player->simulation_time ( );
	this->m_velocity = player->velocity ( ); /* this isn't local only but the game seems to think so... "DT_LocalPlayerExclusive->m_vecVelocity[0]=276 - hash: 856357998" */

	if ( player->active_weapon ( ) )
		this->m_shot = player->active_weapon ( )->last_shot_time ( );

	if ( player->anim_overlays ( ) )
		std::memcpy ( &this->m_overlays , player->anim_overlays ( ) , sizeof ( sdk::animlayer_t ) * player->num_anim_overlays ( ) );

	/* sometimes animstate is nullptr wtf */
	if ( player->animstate ( ) )
		std::memcpy ( &this->m_animstate , player->animstate ( ) , sizeof ( sdk::animstate_t ) );
}

void hack::modules::animations::anim_frame_t::apply ( sdk::c_player * player ) {
	player->set_absolute_angles ( this->m_absangs.y );
	player->set_absolute_origin ( this->m_origin );
	player->eyeangles ( ) = this->m_angs;
	player->duck ( ) = this->m_duck;
	player->flags ( ) = this->m_flags;
	player->collision ( ).maxs ( ) = this->m_maxs;
	player->collision ( ).mins ( ) = this->m_mins;
	player->pose_parameter ( ) = this->m_poses;
	player->simulation_time ( ) = this->m_simulation_time;
	player->velocity ( ) = player->absolute_velocity ( ) = this->m_velocity;

	if ( player->active_weapon ( ) )
		player->active_weapon ( )->last_shot_time ( ) = this->m_shot;

	if ( player->anim_overlays ( ) )
		std::memcpy ( player->anim_overlays ( ) , &this->m_overlays , sizeof ( sdk::animlayer_t ) * player->num_anim_overlays ( ) );

	/* sometimes animstate is nullptr wtf */
	if ( player->animstate ( ) )
		std::memcpy ( player->animstate ( ) , &this->m_animstate , sizeof ( sdk::animstate_t ) );
}

void hack::modules::animations::anim_frame_t::update ( sdk::c_player * player , anim_frame_t * previous , int choked ) {
	/* pretty sure this function is stolen from https://github.com/EternityX/DEADCELL-CSGO lol... */
	const auto extrapolate_movement = ( [ ] ( sdk::c_player * p , c_vector3 & origin , c_vector3 & velocity , std::uint32_t & flags , bool on_ground ) {
		c_vector3 start , end , normal;
		sdk::valve::c_trace trace;
		sdk::valve::c_trace_world_only filter ( nullptr );

		start = origin;

		end = start + ( velocity * hack::game::interfaces::global_vars->m_ipt );

		hack::game::interfaces::engine_trace->trace_ray ( sdk::valve::ray_t ( start , end , p->collision( ).mins() , p->collision ( ).maxs ( ) ) , sdk::valve::contents::contents_solid , &filter , &trace );

		if ( trace.flfraction != 1.f ) {
			for ( int i { }; i < 2; ++i ) {
				velocity -= trace.plane.normal * velocity.dot_product ( trace.plane.normal );

				float adjust = velocity.dot_product ( trace.plane.normal );
				if ( adjust < 0.f )
					velocity -= ( trace.plane.normal * adjust );

				start = trace.endpos;
				end = start + ( velocity * ( hack::game::interfaces::global_vars->m_ipt * ( 1.f - trace.flfraction ) ) );

				hack::game::interfaces::engine_trace->trace_ray ( sdk::valve::ray_t ( start , end , p->collision ( ).mins ( ) , p->collision ( ).maxs ( ) ) , sdk::valve::contents::contents_solid , &filter , &trace );
				if ( trace.flfraction == 1.f )
					break;
			}
		}

		start = end = origin = trace.endpos;
		end.z -= 2.f;

		hack::game::interfaces::engine_trace->trace_ray ( sdk::valve::ray_t ( start , end , p->collision ( ).mins ( ) , p->collision ( ).maxs ( ) ) , sdk::valve::contents::contents_solid , &filter , &trace );

		flags &= ~1;

		if ( trace.flfraction != 1.f && trace.plane.normal.z > 0.7f )
			flags |= 1;
	} );

	/* if we didn't cache enough records for this entity */
	if ( !previous ) {
		/* TODO: */
		bool should_we_resolve_player = true;

		/* build our untouched set of bones */
		build_matrix ( player , this->m_bones , 128 , 0x7ff00 , this->m_simulation_time );

		/* do resolver here */

		this->apply ( player );
		
		/* update anims */
		update_player_animations ( player );

		/* build our resolved set of bones */
		build_matrix ( player , this->m_resolved_bones , 128 , 0x7ff00 , this->m_simulation_time );

		return;
	}

	/* TODO: */
	bool should_we_resolve_player = true;

	/* build our untouched set of bones */
	build_matrix ( player , this->m_bones , 128 , 0x7ff00 , this->m_simulation_time );

	/* set anim overlays */
	std::memcpy ( player->anim_overlays ( ) , &this->m_overlays , sizeof ( sdk::animlayer_t ) * player->num_anim_overlays ( ) );

	const auto old_velocity = player->velocity ( );

	/* set other important values */
	player->set_absolute_origin ( this->m_origin );
	player->set_absolute_angles ( this->m_absangs.y );
	player->velocity ( ) = this->m_velocity;

	/* did we shoot? can we on shot backtrack? */
	const auto did_we_shoot = this->m_shot > previous->m_simulation_time && this->m_shot <= this->m_simulation_time;

	auto old_origin = previous->m_origin;
	auto old_flags = previous->m_flags;

	/* animate all missing cmds */
	for ( auto i = 0; i < choked; i++ ) {
		const auto simtime = previous->m_simulation_time + ticks_to_time ( i + 1 );
		const auto lerp = 1.f - ( this->m_simulation_time - simtime ) / ( this->m_simulation_time - simtime );

		/* TODO: lerping eyeangles is kind of a bad idea but w/e... */
		if ( choked - 1 != i ) {
			if ( !did_we_shoot )
				player->eyeangles ( ).y = sdk::math::interpolate ( previous->m_angs.y , this->m_angs.y , lerp );

			/* again, maybe not a great idea... */
			player->duck ( ) = sdk::math::interpolate ( previous->m_duck , this->m_duck , lerp );
		}

		/* current tick */
		if ( choked - 1 == i ) {
			player->velocity ( ) = old_velocity;
			player->flags ( ) = this->m_flags;

			/* resolve here */
			if ( did_we_shoot ) {
				/* pitch checks */


				/* resolve onshot if past pitch checks */
				{

					/* this is a shot record and should be prioritized */
					this->m_shot_record = true;
				}
			} else {
				/* resolve standing / moving w/e */
			}
		} else {
			/* TODO: replace this garbage */
			extrapolate_movement ( player , old_origin , player->velocity ( ) , player->flags ( ) , old_flags & 1 );
			old_flags = player->flags ( );
		}

		/* set the eyeangles accordingly */
		if ( did_we_shoot && this->m_shot <= simtime ) {
			player->eyeangles ( ).y = this->m_angs.y;
		}

		/* animate our player */
		const auto old_simtime = player->simulation_time ( );
		player->simulation_time ( ) = simtime;
		update_player_animations ( player );
		player->simulation_time ( ) = old_simtime;
	}

	/* build our resolved set of bones */
	build_matrix ( player , this->m_resolved_bones , 128 , 0x7ff00 , this->m_simulation_time );
}
