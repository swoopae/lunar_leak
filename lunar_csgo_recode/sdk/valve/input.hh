#pragma once

#include "user_cmd.hh"

namespace sdk::valve {
	class c_input {
	public:
		void * m_ptable;
		pad ( 8 );
		bool m_track_ir_available;
		bool m_mouse_initialized;
		bool m_mouse_active;
		bool m_joystick_advanced_init;
		pad ( 44 );
		void * m_keys;
		pad ( 108 );
		bool m_camera_intercepting_mouse;
		bool m_camera_in_thirdperson;
		bool m_camera_moving_with_mouse;
		c_vector3 m_camera_offset;
		bool m_camera_distance_move;
		int m_camera_old_x;
		int m_camera_old_y;
		int m_camera_x;
		int m_camera_y;
		bool m_camera_is_orthographic;
		c_vector3 m_prev_viewangles;
		c_vector3 m_prev_viewangles_tilt;
		float m_last_forward_move;
		int m_clear_input_state;

		ucmd_t * get_usercmd ( int slot , int sequence_number ) {
			c_input * unk = this;

			if ( slot != -1 )
				unk = &this [ 0xDC * slot ];

			auto ret = ( ucmd_t * ) ( *( ( std::uintptr_t * ) unk + 0x3D ) + sizeof ( ucmd_t ) * ( sequence_number % 150 ) );

			if ( ret->m_cmdnum != sequence_number )
				ret = nullptr;

			return ret;
		}

		ucmd_t * get_usercmds ( int sequence_number ) {
			return ( ucmd_t * ) ( *( ( std::uintptr_t * ) this + 0x3D ) + sizeof ( ucmd_t ) * ( sequence_number % 150 ) );
		}
	};
}