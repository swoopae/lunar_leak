#include "player.hh"
#include "weapon.hh"

#include "math/mathutils.hh"

#include "../hack/game/interfaces.hh"
#include "../hack/game/globals.hh"

sdk::c_bone_accessor & sdk::c_player::bone_accesor ( ) {
	const static auto offset = sig ( MODULE_CLIENT_PANORAMA , STR ( "03 86 ? ? ? ? F3 0F 10 40" ) ) * 4; /* 0x26A8 */
	return *reinterpret_cast< c_bone_accessor * >( std::uintptr_t ( this ) + offset );
}

void sdk::c_player::invalidate_bone_cache ( ) {
	const static auto modelbonecounter = sig ( MODULE_CLIENT_PANORAMA , STR ( "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) ) + 10;

	*( std::uint32_t * ) ( ( std::uintptr_t ) this + 0x2924 ) = 0xFF7FFFFF;
	*( std::uint32_t * ) ( ( std::uintptr_t ) this + 0x2690 ) = **( std::uintptr_t ** ) modelbonecounter - 1;
}

std::int32_t sdk::c_player::post_think_rebuilt ( ) {
	/* MDLCACHE_CRITICAL_SECTION(); */
	// shared::memory::v_func< fn > ( hack::game::interfaces::mdl_cache , 33 )( hack::game::interfaces::mdl_cache );

	if ( this->alive ( ) && *reinterpret_cast< std::uintptr_t * >( reinterpret_cast< std::uintptr_t >( this ) + 0x3A81 ) ) {
		shared::memory::v_func< void * ( __thiscall * )( void * ) > ( this , 339 )( this );

		const auto unknown = *reinterpret_cast< std::uintptr_t * >( reinterpret_cast< std::uintptr_t >( this ) + 0x32AC );

		//if ( biggerThanZeroIfCommentaryMode <= 0 || !(unknown & 0x20) || !(unknown & 0x800) && !sub_10220FC0(v3) )
		// (*(void (__thiscall **)(unsigned long *))(*this + 1280))(this);
		reinterpret_cast< void ( __thiscall * )( void * ) >( this + 1280 )( this ); /* ItemPostFrame ( ); */

		if ( this->flags ( ) & 1 )
			this->local_exclusive ( ).fall_velocity ( ) = 0.f;

		/* if GetSequence() == -1 SetSequence(0)*/
		if ( *reinterpret_cast< std::uintptr_t * >( reinterpret_cast< std::uintptr_t >( this ) + 0x28BC ) == -1 )
			shared::memory::v_func< void * ( __thiscall * )( void * , int ) > ( this , 218 )( this , 0 );

		/* StudioFrameAdvance() */
		shared::memory::v_func< void * ( __thiscall * )( void * ) > ( this , 219 )( this );

		static auto post_think_v_physics_fn = reinterpret_cast< bool ( __thiscall * )( void * ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) ) );
		post_think_v_physics_fn ( this );
	}

	static auto simulate_player_simulated_entities_fn = reinterpret_cast< bool ( __thiscall * )( void * ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72" ) ) );
	simulate_player_simulated_entities_fn ( this );

	return 1;
	// return shared::memory::v_func< int ( __thiscall* )( void* ) > ( hack::game::interfaces::mdl_cache , 34 )( hack::game::interfaces::mdl_cache );
}

bool sdk::c_player::physics_run_think ( std::int32_t a1 ) {
	/* xref: "PhysicsSimulate: %s bad movetype %d"
	 * sig last called function (appears in case 0 and case 3 also) */
	static auto physics_run_think_fn = reinterpret_cast< bool ( __thiscall * )( void * , int ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) ) );
	return physics_run_think_fn ( this , a1 );
}

void sdk::c_player::invalidate_physics ( std::int32_t flags ) {
	/* xref: "move_x", "move_y", "move_yaw"
	 * sig function called just above movex movey */
	static const auto invalidate_physics_recursive = reinterpret_cast< void ( __thiscall * )( void * , int32_t ) >(
		sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3 56" ) ) );
	invalidate_physics_recursive ( this , flags );
}

sdk::valve::c_studio_hdr * sdk::c_player::studio_model ( ) {
	if ( !this || !this->anim_model ( ) )
		return nullptr;

	return hack::game::interfaces::model_info->get_studio_model ( this->anim_model ( ) );
}

sdk::valve::c_studio_hdr * sdk::c_player::studio_hdr ( ) {
	if ( !this )
		return nullptr;

	const static auto offset = 0x294C;//sig ( MODULE_CLIENT_PANORAMA , STR ( "8B 8F ? ? ? ? 85 C9 74 26" ) ); /* 0x294C */
	return *reinterpret_cast< sdk::valve::c_studio_hdr ** >( std::uintptr_t ( this ) + offset );
}

void sdk::c_player::lock_studio_hdr ( ) {
	/* xref: "main: %s(%d), cycle: %.2f cyclerate: %.2f playbackrate: %.2f\n"
	 * sig first called function - xref supposedly leads to CBasePlayerAnimState::DebugShowAnimState */
	static const auto lockstudiohdr_fn = reinterpret_cast< void ( __thiscall * )( void * ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 51 53 8B D9 56 57 8D B3" ) ) );
	lockstudiohdr_fn ( this );
}

c_vector3 sdk::c_player::eye_offset ( ) {
	if ( !this ) return {};
	static auto fn = reinterpret_cast< float * ( __thiscall * )( void * , c_vector3 * ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90" ) ) );

	c_vector3 ret;
	fn ( this , &ret );

	return ret;
}

bool sdk::c_player::enemy ( ) {
	if ( !this || !hack::game::globals::local )
		return false;

	const static auto mp_teammates_are_enemies = hack::game::interfaces::convar_system->get_convar ( STR ( "mp_teammates_are_enemies" ) );

	if ( !mp_teammates_are_enemies )
		return this->team ( ) != hack::game::globals::local->team ( );

	if ( mp_teammates_are_enemies->get_int ( ) )
		return this != hack::game::globals::local;

	return this->team ( ) != hack::game::globals::local->team ( );
}

bool sdk::c_player::teammate ( ) {
	return !this->enemy ( );
}

bool sdk::c_player::can_shoot ( ) {
	if ( !this || !this->valid ( ) )
		return false;

	const auto servertime = ticks_to_time ( this->tickbase ( ) );

	sdk::c_weapon * weapon = this->active_weapon ( );

	if ( !weapon )
		return false;

	if ( !weapon->clip ( ) )
		return false;

	if ( this->next_attack ( ) > servertime )
		return false;

	if ( weapon->next_primary_attack ( ) > servertime )
		return false;

	if ( weapon->weapon_kind ( ) == sdk::e_cs_weapon_type::weapontype_grenade ) {
		if ( !weapon->pin_pulled ( ) )
			return false;

		if ( weapon->throw_time ( ) < 0.f || weapon->throw_time ( ) > servertime )
			return false;
	}

	if ( weapon->item_definition_index ( ) == sdk::e_item_definition_index::weapon_revolver )
		if ( weapon->postpone_fire_ready_time ( ) > servertime )
			return false;

	return true;
}

void sdk::c_player::set_absolute_angles ( const float & yaw ) {
	static auto set_abs_angles = reinterpret_cast< void ( __thiscall * )( void * , const c_vector3 & ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) ) );
	auto tmp = c_vector3 ( 0.0f , yaw , 0.0f );
	set_abs_angles ( this , tmp );
}

void sdk::c_player::set_absolute_origin ( const c_vector3 & new_origin ) {
	static auto set_abs_origin = reinterpret_cast< void ( __thiscall * )( void * , const c_vector3 & ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) ) );
	set_abs_origin ( this , new_origin );
}

void sdk::c_player::attachment_helper ( ) {
	if ( !this || !this->studio_hdr ( ) )
		return;

	static auto attachment_helper = reinterpret_cast< void ( __thiscall * )( void * , void * ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 EC 48 53 8B 5D 08 89 4D F4 56 57 85 DB 0F 84 ? ? ? ? 8B 43 04 85 C0 75 0A 8B 03 8B 80 ? ? ? ? EB 03 8B 40 3C 85 C0 0F 84 ? ? ? ? 33 F6 8B CB 89 75 08 E8 ? ? ? ? 85 C0 0F 8E ? ? ? ? 33 C9 89 4D F8 0F 1F 40 00 8B 53 04 85 D2 75 0C 8B 03 8B B8 ? ? ? ? 03 F9 EB 21" ) ) );
	attachment_helper ( this , this->studio_hdr ( ) );
}

/* alpha alpha alpha alpha alpha alpha alpha */
void sdk::c_player::studio_build_matrices ( sdk::valve::c_studio_hdr * hdr , const c_matrix3x4 & rotationmatrix , c_vector3 * pos , c_quaternion * q , int bone_mask , c_matrix3x4 * bone_to_world , std::uint32_t * bone_computed ) {
	if ( !hdr )
		return;

	int i , j;
	int chain [ 256 ] = { };
	int chainlength = 0;

	// just stub
	int iBone = -1;

	sdk::valve::c_studio_hdr * pStudioHdr = *( sdk::valve::c_studio_hdr ** ) hdr;
	if ( !pStudioHdr )
		return;

	if ( iBone < -1 || iBone >= pStudioHdr->bones_count )
		iBone = 0;

	auto v10 = ( uintptr_t ) hdr;

	auto boneFlags = ( std::vector<int> * ) ( v10 + 0x30 );
	auto boneParent = ( std::vector<int> * ) ( v10 + 0x44 );

	// build list of what bones to use
	if ( iBone <= -1 ) {
		// all bones
		chainlength = pStudioHdr->bones_count;
		for ( i = 0; i < pStudioHdr->bones_count; i++ ) {
			chain [ chainlength - i - 1 ] = i;
		}

	} else {
		// only the parent bones
		i = iBone;

		do {
			chain [ chainlength++ ] = i;
			i = boneParent->at ( i );
		} while ( i != -1 );
	}

	const auto concat_transforms = [ ] ( c_matrix3x4 & out , const c_matrix3x4 & in1 , const c_matrix3x4 & in2 ) {
		out [ 0 ][ 0 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 0 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 0 ] +
			in1 [ 0 ][ 2 ] * in2 [ 2 ][ 0 ];
		out [ 0 ][ 1 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 1 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 1 ] +
			in1 [ 0 ][ 2 ] * in2 [ 2 ][ 1 ];
		out [ 0 ][ 2 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 2 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 2 ] +
			in1 [ 0 ][ 2 ] * in2 [ 2 ][ 2 ];
		out [ 0 ][ 3 ] = in1 [ 0 ][ 0 ] * in2 [ 0 ][ 3 ] + in1 [ 0 ][ 1 ] * in2 [ 1 ][ 3 ] +
			in1 [ 0 ][ 2 ] * in2 [ 2 ][ 3 ] + in1 [ 0 ][ 3 ];
		out [ 1 ][ 0 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 0 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 0 ] +
			in1 [ 1 ][ 2 ] * in2 [ 2 ][ 0 ];
		out [ 1 ][ 1 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 1 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 1 ] +
			in1 [ 1 ][ 2 ] * in2 [ 2 ][ 1 ];
		out [ 1 ][ 2 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 2 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 2 ] +
			in1 [ 1 ][ 2 ] * in2 [ 2 ][ 2 ];
		out [ 1 ][ 3 ] = in1 [ 1 ][ 0 ] * in2 [ 0 ][ 3 ] + in1 [ 1 ][ 1 ] * in2 [ 1 ][ 3 ] +
			in1 [ 1 ][ 2 ] * in2 [ 2 ][ 3 ] + in1 [ 1 ][ 3 ];
		out [ 2 ][ 0 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 0 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 0 ] +
			in1 [ 2 ][ 2 ] * in2 [ 2 ][ 0 ];
		out [ 2 ][ 1 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 1 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 1 ] +
			in1 [ 2 ][ 2 ] * in2 [ 2 ][ 1 ];
		out [ 2 ][ 2 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 2 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 2 ] +
			in1 [ 2 ][ 2 ] * in2 [ 2 ][ 2 ];
		out [ 2 ][ 3 ] = in1 [ 2 ][ 0 ] * in2 [ 0 ][ 3 ] + in1 [ 2 ][ 1 ] * in2 [ 1 ][ 3 ] +
			in1 [ 2 ][ 2 ] * in2 [ 2 ][ 3 ] + in1 [ 2 ][ 3 ];
	};

	const auto quaternion_matrix = [ ] ( c_matrix3x4 & matrix , c_quaternion & q , c_vector3 pos ) {
		matrix [ 0 ][ 0 ] = 1.0 - 2.0 * q.y * q.y - 2.0 * q.z * q.z;
		matrix [ 1 ][ 0 ] = 2.0 * q.x * q.y + 2.0 * q.w * q.z;
		matrix [ 2 ][ 0 ] = 2.0 * q.x * q.z - 2.0 * q.w * q.y;

		matrix [ 0 ][ 1 ] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
		matrix [ 1 ][ 1 ] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
		matrix [ 2 ][ 1 ] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;

		matrix [ 0 ][ 2 ] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
		matrix [ 1 ][ 2 ] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
		matrix [ 2 ][ 2 ] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

		matrix [ 0 ][ 3 ] = pos.x;
		matrix [ 1 ][ 3 ] = pos.y;
		matrix [ 2 ][ 3 ] = pos.z;
	};

	// missing some parts with 0xF flag
	c_matrix3x4 bonematrix;
	for ( j = chainlength - 1; j >= 0; j-- ) {
		i = chain [ j ];

		if ( ( ( 1 << ( i & 0x1F ) ) & bone_computed [ i >> 5 ] ) )
			continue;

		int flag = boneFlags->at ( i );
		if ( flag & bone_mask && q ) {
			quaternion_matrix ( bonematrix , q [ i ] , pos [ i ] );

			auto parent = boneParent->at ( i );
			if ( parent == -1 ) {
				concat_transforms ( bone_to_world [ i ] , rotationmatrix , bonematrix );
				// bone_to_world [ i ] = rotationmatrix.ConcatTransforms ( bonematrix );
			} else {
				concat_transforms ( bone_to_world [ i ] , bone_to_world [ parent ] , bonematrix );
				// bone_to_world [ i ] = bone_to_world [ parent ].ConcatTransforms ( bonematrix );
			}
		}
	}
}

sdk::c_weapon * sdk::c_player::active_weapon ( ) {
	if ( !this || !this->valid ( ) )
		return nullptr;

	const auto wep = hack::game::interfaces::entity_list->get_by_handle<sdk::c_weapon *> ( this->weapon_handle ( ) );

	if ( !wep )
		return nullptr;

	return wep;
}

void sdk::foreach_player ( std::function<void ( c_player * )> fn , bool include_invalid_players ) {
	for ( auto i = 1; i < 65; i++ ) {
		const auto player = hack::game::interfaces::entity_list->get<c_player *> ( i );

		if ( !player )
			continue;

		if ( !include_invalid_players )
			if ( !player->valid ( ) )
				continue;

		fn ( player );
	}
}

void sdk::animstate_t::reset ( ) {
	const static auto reset_anim_state = reinterpret_cast< void ( __thiscall * )( animstate_t * ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "56 6A 01 68 ? ? ? ? 8B F1" ) ) );
	reset_anim_state ( this );
}

void sdk::animstate_t::update ( c_vector3 angles ) {
	if ( !this ) return;
	const static auto ret = reinterpret_cast< void ( __vectorcall * )( void * , void * , float , float , float , void * ) >( hack::game::addresses::functions::animstateupdate_address );

	if ( !ret )
		return;

	ret ( this , nullptr , 0.f , angles.y , angles.x , nullptr );
}