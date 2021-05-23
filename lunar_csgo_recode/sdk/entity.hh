#pragma once

#ifndef LUNAR_ENTITY
#define LUNAR_ENTITY

#include "../sdk/math/matrix3x4.hh"

#include "../hack/game/netvars.hh"

/* don't worry about me :) */
using csgo_handle = unsigned long;

/* HUGE NOTE: next time i write a hack i should use the netvar names as the function names as well as use the original class/interface names and stop being a retard ~swoopae */
namespace sdk {
	class c_collision {
	public:
		/* netvars */
		netvar_fn ( c_vector3 , mins , "DT_CollisionProperty->m_vecMins" );
		netvar_fn ( c_vector3 , maxs , "DT_CollisionProperty->m_vecMaxs" );
		netvar_fn ( char , solid_type , "DT_CollisionProperty->m_nSolidType" );
		netvar_fn ( unsigned short , solid_flags , "DT_CollisionProperty->m_usSolidFlags" );
	};

	/* custom entity class */
	class c_entity {
	public:
		/* offsets */
		offset ( std::uint32_t , index , 0x64 );
		offset ( std::int32_t , take_damage , 0x280 );

		/* offsets to pointers */
		poffset ( void * , renderable , 0x4 );
		poffset ( void * , networkable , 0x8 );
		poffset ( void * , unknown , 0xC );

		/* netvars */
		netvar_fn ( c_vector3 , origin , "DT_BaseEntity->m_vecOrigin" );
		netvar_fn ( std::int32_t , team , "DT_BaseEntity->m_iTeamNum" );
		netvar_fn ( float , simulation_time , "DT_BaseEntity->m_flSimulationTime" );
		netvar_fn ( c_collision , collision , "DT_BaseEntity->m_Collision" );

		/* values offset'd from netvars */
		netvar_offset ( float , old_simulation_time , "DT_BaseEntity->m_flSimulationTime" , 0x4 );
		netvar_offset ( c_matrix3x4 , coordonate_frame , "DT_BaseEntity->m_CollisionGroup" , -48 );

		/* vfuncs and others */
		bool dormant ( );

		sdk::valve::client_class_t * client_class ( ) {
			return shared::memory::v_func< sdk::valve::client_class_t * ( __thiscall * )( void * ) > ( this , 2 )( this );
		}

		c_vector3 & absolute_origin ( ) {
			return shared::memory::v_func< c_vector3 & ( __thiscall * )( void * ) > ( this , 10 )( this );
		}

		c_vector3 & render_origin ( ) {
			return shared::memory::v_func< c_vector3 & ( __thiscall * )( void * ) > ( this->renderable ( ) , 1 )( this->renderable ( ) );
		}

		void on_pre_data_change ( int update_type ) {
			shared::memory::v_func< void ( __thiscall * )( void * , int ) > ( this->networkable ( ) , 4 )( this->networkable ( ) , update_type );
		}

		void on_post_data_change ( int update_type ) {
			shared::memory::v_func< void ( __thiscall * )( void * , int ) > ( this->networkable ( ) , 5 )( this->networkable ( ) , update_type );
		}

		void pre_data_update ( int update_type ) {
			shared::memory::v_func< void ( __thiscall * )( void * , int ) > ( this->networkable ( ) , 6 )( this->networkable ( ) , update_type );
		}

		void post_data_update ( int update_type ) {
			shared::memory::v_func< void ( __thiscall * )( void * , int ) > ( this->networkable ( ) , 7 )( this->networkable ( ) , update_type );
		}

		/* int __thiscall sub_10199650(char *this) { return (int)(this + 800); } */
		csgo_handle handle ( ) {
			return shared::memory::v_func< csgo_handle ( __thiscall * )( void * ) > ( this , 3 )( this );
		}

		c_vector3 & absolute_angles ( ) {
			return shared::memory::v_func< c_vector3 & ( __thiscall * )( void * ) > ( this , 11 )( this );
		}
	};

	/* rain */
	enum e_precipitation_types : int {
		precipitation_type_rain = 0 ,
		precipitation_type_snow ,
		precipitation_type_ash ,
		precipitation_type_snowfall ,
		precipitation_type_particlerain ,
		precipitation_type_particleash ,
		precipitation_type_particlerainstorm ,
		precipitation_type_particlesnow ,
		num_precipitation_types
	};

	class c_precipitation_entity : public c_entity {
	public:
		/* netvars */
		netvar_fn ( e_precipitation_types , precipitation_type , "DT_Precipitation->m_nPrecipType" );

		netvar_offset ( void * , material_handle , "DT_Precipitation->m_nPrecipType" , -36 );
	};
}

#endif /* LUNAR_ENTITY */