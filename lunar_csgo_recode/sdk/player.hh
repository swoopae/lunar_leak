#pragma once

#ifndef LUNAR_PLAYER
#define LUNAR_PLAYER

#include "entity.hh"

#include <functional>

/* fuck macros */
using pose_params = std::array< float , 24 >;

namespace sdk {
	namespace valve {
		struct model_t;
		class c_studio_hdr;
		class c_util_vector;
	}

	class c_weapon;

	/* dunno where to toss these */
	enum class movetypes {
		movetype_none = 0 ,
		movetype_isometric ,
		movetype_walk ,
		movetype_step ,
		movetype_fly ,
		movetype_flygravity ,
		movetype_vphysics ,
		movetype_push ,
		movetype_noclip ,
		movetype_ladder ,
		movetype_observer ,
		movetype_custom ,
		movetype_last = movetype_custom ,
		movetype_max_bits = 4
	};

	enum class hitboxes {
		head ,
		neck ,
		pelvis ,
		body ,
		thorax ,
		chest ,
		upper_chest ,
		left_thigh ,
		right_thigh ,
		left_calf ,
		right_calf ,
		left_foot ,
		right_foot ,
		left_hand ,
		right_hand ,
		left_upper_arm ,
		left_forearm ,
		right_upper_arm ,
		right_forearm ,
		max
	};

	/* plop this shit here */
	/* perfectly aware that most names here are incorrect, last time i took a look at this struct was way before the source leak so please leave me alone */
	struct animstate_t {
		void reset ( );
		void update ( c_vector3 angles );

		void * m_this_ptr;
		pad ( 91 );
		void * m_base_entity;
		void * m_active_weapon;
		void * m_last_active_weapon;
		float m_last_client_side_anim_update_time;
		int m_last_client_side_update_frame_count;
		float m_last_client_side_anim_update_delta;
		float m_eye_yaw;
		float m_pitch;
		float m_goal_feet_yaw;
		float m_cur_feet_yaw;
		float m_cur_move_dir_goal_feet_delta;
		float m_goal_move_dir_goal_feet_delta;
		float m_lean_amount;
		pad ( 4 );
		float m_feet_cycle;
		float m_feet_yaw_rate;
		float m_unknown_2;
		float m_duck_amount;
		float m_landing_duck_additive;
		float m_unknown_3;
		c_vector3 m_origin;
		c_vector3 m_last_origin;
		c_vector2 m_velocity;
		pad ( 4 );
		float m_unknown_4;
		pad ( 8 );
		float m_unknown_5;
		float m_unknown_6;
		float m_unknown_7;
		float m_speed_2d;
		float m_abs_velocity_z;
		float m_speed_normalized;
		float m_feet_speed_forwards_sideways;
		float m_duck_speed;
		float m_time_since_started_moving;
		float m_time_since_stopped_moving;
		bool m_on_ground;
		bool m_hit_on_ground_animation;
		pad ( 10 );
		float m_last_origin_z;
		float m_head_height_offset;
		float m_ground_fraction;
		pad ( 4 );
		float m_magic_fraction;
		pad ( 4 );
		float m_unknown_8;
		pad ( 528 );
	};

	/* also this */
	struct animlayer_t {
		pad ( 20 );
		int	m_order;
		int	m_sequence;
		float m_previous_cycle;
		float m_weight;
		float m_weight_delta_rate;
		float m_playback_rate;
		float m_cycle;
		void * m_owner;
		pad ( 4 );
	};

	class c_bone_accessor {
	public:
		void * m_animating;
		c_matrix3x4 * m_bones;
		int m_readable;
		int m_writeable;
	};

	/* custom player class */
	class c_local_exclusive {
	public:
		netvar_fn ( float , fall_velocity , "DT_Local->m_flFallVelocity" );
		netvar_fn ( c_vector3 , aimpunch , "DT_Local->m_aimPunchAngle" );
		netvar_fn ( c_vector3 , aimpunch_velocity , "DT_Local->m_aimPunchAngleVel" );
		netvar_fn ( c_vector3 , viewpunch , "DT_Local->m_viewPunchAngle" );
	};

	class c_player : public c_entity {
	public:
		/* netvars */

		/* local player exclusives */
		/* DT_LocalPlayerExclusive->m_flNextDecalTime */
		netvar_fn ( c_local_exclusive , local_exclusive , "DT_LocalPlayerExclusive->m_Local" );
		netvar_fn ( std::int32_t , tickbase , "DT_LocalPlayerExclusive->m_nTickBase" );
		netvar_fn ( float , next_attack , "DT_BCCLocalPlayerExclusive->m_flNextAttack" );
		netvar_fn ( std::int32_t , next_think_tick , "DT_LocalActiveWeaponData->m_nNextThinkTick" );
		netvar_fn ( c_vector3 , view_offset , "DT_LocalPlayerExclusive->m_vecViewOffset[0]" );
		netvar_fn ( c_vector3 , velocity , "DT_LocalPlayerExclusive->m_vecVelocity[0]" ); /* not exclusive to local */
		netvar_fn ( c_vector3 , velocity_base , "DT_LocalPlayerExclusive->m_vecBaseVelocity" ); 
		netvar_fn ( float , velocity_modifier , "DT_CSLocalPlayerExclusive->m_flVelocityModifier" ); 
		netvar_fn ( float , stamina , "DT_CSLocalPlayerExclusive->m_flStamina" ); /* unsure */

		/* player */
		netvar_fn ( std::uint32_t , flags , "DT_BasePlayer->m_fFlags" );
		netvar_fn ( std::int8_t , lifestate , "DT_BasePlayer->m_lifeState" );
		netvar_fn ( std::int32_t , observer_mode , "DT_BasePlayer->m_iObserverMode" );
		netvar_fn ( int , collision_state , "DT_BasePlayer->m_vphysicsCollisionState" );
		netvar_fn ( int , health , "DT_BasePlayer->m_iHealth" );
		netvar_fn ( int , armor , "DT_CSPlayer->m_ArmorValue" );
		netvar_fn ( c_vector3 , eyeangles , "DT_CSPlayer->m_angEyeAngles[0]" );
		netvar_fn ( bool , helmet , "DT_CSPlayer->m_bHasHelmet" );
		netvar_fn ( bool , heavy_armor , "DT_CSPlayer->m_bHasHeavyArmor" );
		netvar_fn ( bool , immune , "DT_CSPlayer->m_bGunGameImmunity" );
		netvar_fn ( bool , defuser , "DT_CSPlayer->m_bHasDefuser" );
		netvar_fn ( float , lby , "DT_CSPlayer->m_flLowerBodyYawTarget" );
		netvar_fn ( float , duck , "DT_BasePlayer->m_flDuckAmount" );
		netvar_fn ( float , flash_duration , "DT_CSPlayer->m_flFlashDuration" );
		netvar_fn ( float , flash_alpha , "DT_CSPlayer->m_flFlashMaxAlpha" );
		netvar_fn ( float , health_boost_time , "DT_CSPlayer->m_flHealthShotBoostExpirationTime" );
		netvar_fn ( bool , scoped , "DT_CSPlayer->m_bIsScoped" );
		netvar_fn ( bool , clientanims , "DT_BaseAnimating->m_bClientSideAnimation" );
		netvar_fn ( int , hitbox_set , "DT_BaseAnimating->m_nHitboxSet" );

		/* combatcharacter */
		netvar_fn ( csgo_handle , weapon_handle , "DT_BaseCombatCharacter->m_hActiveWeapon" );

		/* offsets */
		netvar_offset ( movetypes , movetype , "DT_BaseEntity->m_nRenderMode" , 1 );

		/* hardcoded shit */
		offset ( c_vector3 , absolute_velocity , 0x94 );
		offset ( animlayer_t * , anim_overlays , 0x2980 );
		offset ( int , num_anim_overlays , 0x298C );
		offset ( float , airtime , 0x3A10 );
		offset ( float , spawntime , 0xA370 );
		offset ( animstate_t * , animstate , 0x3914 );
		offset ( pose_params , pose_parameter , 0x2774 );
		offset ( void * , inverse_kinematics , 0x2674 );
		offset ( sdk::valve::c_util_vector , bone_cache , 0x2910 ); // 0x290C

		/* vfuncs and others */
		void pre_think ( ) {
			if ( !this ) return;

			shared::memory::v_func< void ( __thiscall * )( void * ) > ( this , 317 )( this );
		}

		void think ( ) {
			if ( !this ) return;

			shared::memory::v_func< void ( __thiscall * )( void * ) > ( this , 138 )( this );
		}

		void update_anims ( ) {
			if ( !this ) return;

			shared::memory::v_func< void ( __thiscall * )( void * ) > ( this , 223 )( this );
		}

		/* credits: stacker */
		void force_velocity ( const c_vector3& vel ) {
			this->invalidate_physics ( 0x4 );
			this->absolute_velocity ( ) = vel;
		}

		void render_bounds ( c_vector3 & mins , c_vector3 & maxs ) {
			if ( !this ) return;

			const auto renderable = this->renderable ( );
			shared::memory::v_func< void ( __thiscall * )( void * , c_vector3 & , c_vector3 & ) > ( renderable , 17 )( renderable , mins , maxs );
		}

		void standard_blending_rules ( sdk::valve::c_studio_hdr * hdr , c_vector3 * pos , c_quaternion * quat , float time , int mask ) {
			if ( !this ) return;

			shared::memory::v_func< void ( __thiscall * )( void * , sdk::valve::c_studio_hdr * , c_vector3 * , c_quaternion * , float , int ) > ( this , 205 )( this , hdr , pos , quat , time , mask );
		}

		void build_transformations ( sdk::valve::c_studio_hdr * hdr , c_vector3 * pos , c_quaternion * quat , const c_matrix3x4& transsrm , int mask , std::uint8_t * computed ) {
			if ( !this ) return;

			shared::memory::v_func< void ( __thiscall * )( void * , sdk::valve::c_studio_hdr * , c_vector3 * , c_quaternion * , const c_matrix3x4 & , int , uint8_t * ) > ( this , 189 )( this , hdr , pos , quat , transsrm , mask , computed );
		}

		sdk::valve::model_t * anim_model ( ) {
			if ( !this ) return nullptr;

			return shared::memory::v_func< sdk::valve::model_t * ( __thiscall * )( void * ) > ( this->renderable ( ) , 8 )( this->renderable ( ) );
		}

		bool setup_bones ( c_matrix3x4 * out , int max_bones , int mask , float time ) {
			if ( !this ) return false;

			return shared::memory::v_func< bool ( __thiscall * )( void * , c_matrix3x4 * , int , int , float ) > ( this->renderable ( ) , 13 )( this->renderable ( ) , out , max_bones , mask , time );
		}

		bool alive ( ) {
			if ( !this ) return false;
			return health ( ) > 0 || !lifestate ( );
		}

		bool valid ( ) {
			if ( !this ) return false;

			return this && alive ( ) && !dormant ( ) && !lifestate ( ); /* && !immune ( ); */ // was i on fucking crack? ~swoopae
		}

		c_bone_accessor& bone_accesor ( );
		void invalidate_bone_cache ( );
		/* sig: "56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07"
		 * no xref */
		std::int32_t post_think_rebuilt ( );
		bool physics_run_think ( std::int32_t a1 );
		void invalidate_physics ( std::int32_t flags );
		sdk::valve::c_studio_hdr * studio_model ( );
		sdk::valve::c_studio_hdr * studio_hdr ( );
		void lock_studio_hdr ( );
		c_vector3 eye_offset ( );
		bool enemy ( );
		bool teammate ( );
		bool can_shoot ( );
		void set_absolute_angles ( const float & yaw );
		void set_absolute_origin ( const c_vector3 & new_origin );
		void attachment_helper ( );
		void studio_build_matrices ( sdk::valve::c_studio_hdr * hdr , const c_matrix3x4 & rotationmatrix , c_vector3 * pos , c_quaternion * q , int bone_mask , c_matrix3x4 * bone_to_world , std::uint32_t * bone_computed );
		c_weapon * active_weapon ( );
	};

	void foreach_player ( std::function< void ( c_player * ) > fn , bool include_invalid_players = false );
}

#endif /* LUNAR_PLAYER */