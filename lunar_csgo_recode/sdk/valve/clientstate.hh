#pragma once

#include "../../shared/memory/memory.hh"

#include "../math/vector.hh"

namespace sdk::valve {
	class c_clientstate {
	public:
		void force_fullupdate ( ) {
			delta_tick = -1;
		}

		pad ( 156 );
		void * net_channel;
		std::uint32_t challenge_nr;
		pad ( 100 );
		std::uint32_t sign_on_state;
		pad ( 8 );
		float next_cmd_time;
		std::uint32_t server_count;
		std::uint32_t curr_sequence;
		pad ( 84 );
		std::uint32_t delta_tick;
		bool paused;
		pad ( 3 );
		std::uint32_t view_ent;
		std::uint32_t player_slot;
		char level_name [ 260 ];
		char short_level_name [ 80 ];
		char group_name [ 80 ];
		pad ( 92 );
		std::uint32_t max_clients;
		pad ( 18828 );
		float last_server_tick_time;
		bool in_simulation;
		pad ( 3 );
		std::uint32_t oldtickcount;
		float tick_emainder;
		float frame_time;
		int last_outgoing_command;
		int choked_commands;
		int last_command_ack;
		int command_ack;
		int sound_sequence;
		pad ( 80 );
		c_vector3 viewangles;
		pad ( 208 );
		void * events;
	};

	class c_viewsetup {
	public:
		pad ( 176 );
		float m_fov;
		float m_viewmodel_fov;
		c_vector3 m_origin;
		c_vector3 m_angles;
		pad ( 124 );
	};
}