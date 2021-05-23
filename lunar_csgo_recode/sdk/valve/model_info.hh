#pragma once

#include "../math/matrix3x4.hh"

#include "../../shared/memory/memory.hh"

namespace sdk::valve {
	struct model_t {
		char name [ 255 ];
	};

	struct studio_bone_t {
		int name_index;
		inline char * const name ( void ) const {
			return ( ( char * ) this ) + name_index;
		}
		int parent;
		int bone_controller [ 6 ];

		c_vector3 pos;
		float quat [ 4 ];
		float rotation [ 3 ];

		c_vector3 pos_scale;
		c_vector3 rot_scale;

		c_matrix3x4 pose_to_bone;
		float quat_alignement [ 4 ];
		int flags;
		int proc_type;
		int proc_index;
		mutable int physics_bone;

		inline void * procedure ( ) const {
			if ( proc_index == 0 ) return 0;
			else return( void * ) ( ( ( unsigned char * ) this ) + proc_index );
		};

		int surface_prop_idx;
		inline char * const surface_prop ( void ) const {
			return ( ( char * ) this ) + surface_prop_idx;
		}
		inline int get_surface_prop ( void ) const {
			return surf_prop_lookup;
		}

		int contents;
		int surf_prop_lookup;
		int unused [ 7 ];
	};

	struct studio_box_t {
		int bone;
		int group;
		c_vector3 mins;
		c_vector3 maxs;
		int name_index;
		int pad01 [ 3 ];
		float radius;
		int pad02 [ 4 ];
	};

	struct studio_hitbox_set_t {
		int name_index;
		int hitbox_count;
		int hitbox_index;

		inline char * const name ( void ) const {
			return ( ( char * ) this ) + name_index;
		}
		inline studio_box_t * hitbox ( int i ) const {
			return ( studio_box_t * ) ( ( ( unsigned char * ) this ) + hitbox_index ) + i;
		}
	};

	class c_studio_hdr {
	public:
		int id;
		int version;
		long checksum;
		char name_char_array [ 64 ];
		int length;
		c_vector3 eye_pos;
		c_vector3 illium_pos;
		c_vector3 hull_mins;
		c_vector3 hull_maxs;
		c_vector3 mins;
		c_vector3 maxs;
		int flags;
		int bones_count;
		int bone_index;
		int bone_controllers_count;
		int bone_controller_index;
		int hitbox_sets_count;
		int hitbox_set_index;
		int local_anim_count;
		int local_anim_index;
		int local_seq_count;
		int local_seq_index;
		int activity_list_version;
		int events_indexed;
		int textures_count;
		int texture_index;

		studio_hitbox_set_t * hitbox_set ( int i ) {
			if ( i > hitbox_sets_count ) return nullptr;
			return ( studio_hitbox_set_t * ) ( ( uint8_t * ) this + hitbox_set_index ) + i;
		}
		studio_bone_t * bone ( int i ) {
			if ( i > bones_count ) return nullptr;
			return ( studio_bone_t * ) ( ( uint8_t * ) this + bone_index ) + i;
		}
	};

	class c_model_info {
	public:
		model_t * get_model ( int index ) {
			using original_fn = model_t * ( __thiscall * )( c_model_info * , int );
			return shared::memory::v_func<original_fn> ( this , 1 )( this , index );
		}
		int get_model_index ( const char * filename ) {
			using original_fn = int ( __thiscall * )( c_model_info * , const char * );
			return shared::memory::v_func<original_fn> ( this , 2 )( this , filename );
		}
		const char * get_model_name ( const model_t * model ) {
			using original_fn = const char * ( __thiscall * )( c_model_info * , const model_t * );
			return shared::memory::v_func<original_fn> ( this , 3 )( this , model );
		}
		c_studio_hdr * get_studio_model ( const model_t * model ) {
			using original_fn = c_studio_hdr * ( __thiscall * )( c_model_info * , const model_t * );
			return shared::memory::v_func<original_fn> ( this , 32 )( this , model );
		}
	};
}