#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include <deque>
#include <functional>

namespace hack::modules::animations {
	/* last resolver state */
	enum e_resolver_state : unsigned char {
		resolver_auto = 0 ,
		resolver_forced_direction ,
		resolver_anti_freestanding ,
		resolver_bruteforce_stage_one ,
		resolver_bruteforce_stage_two ,
		resolver_bruteforce_stage_three ,
		resolver_bruteforce_stage_four ,
		resolver_unknown
	};

	enum e_resolver_side : unsigned char {
		side_auto = 0 ,
		side_left ,
		side_right ,
		side_force_backwards ,
		side_unknown
	};

	/* animation frame / tick */
	struct anim_frame_t {
		void store ( sdk::c_player * player );
		void apply ( sdk::c_player * player );
		void update ( sdk::c_player * player , anim_frame_t * previous = nullptr , int choked = 0 );

		c_vector3 m_origin , m_velocity , m_angs , m_mins , m_maxs , m_absangs;
		float m_simulation_time , m_duck , m_shot;
		int m_flags , m_eflags;
		sdk::animstate_t m_animstate;
		std::array<float , 24> m_poses;
		std::array<sdk::animlayer_t , 15> m_overlays;
		bool m_shot_record;

		c_matrix3x4 m_bones [ 128 ];
		c_matrix3x4 m_resolved_bones [ 128 ]; /* for safepoint */
	};

	/* every player should have one of these , based on their handle ( except for local ) */
	struct anim_info_t {
		/* animation frames */
		std::deque<anim_frame_t> m_recieved_frames;

		/* animation fix related info */
		sdk::c_player * m_player;
		float m_spawntime;
		int m_last_choked; /* for resolver check */

		/* player handle -- needed to check if we should invalidate all records */
		std::uint32_t m_handle;

		/* resolver info */
		bool m_should_resolve; /* we shouldn't resolve bots / legits */
		std::uint8_t m_missed_shots; /* only counting missed shots due to incorrect resolve */
		e_resolver_state m_last_resolver_state;
		e_resolver_side m_resolved_side;
	};

	/* just to make sure everything here is allocated where its supposed to be */
	class c_animationsystem {
	public:
		std::array<anim_info_t , 65> m_animation_info;
		std::vector<std::pair<sdk::valve::ucmd_t , std::pair<c_vector3 , c_vector3>>> m_local_cmds;
	};

	inline std::unique_ptr<c_animationsystem> animation_system = std::make_unique<c_animationsystem> ( );

	inline bool allow_clientside_anims = false , allow_setup_bones = false , in_local_animfix = false;
	inline c_matrix3x4 local_real_bones [ 128 ] , local_desync_bones [ 128 ];
	inline bool new_tick = false;
	inline float last_sent_absyaw = 0.f;
	inline c_vector3 last_origin;
	inline bool got_layers = false;
	inline bool prediction_issue = false;
	inline c_vector3* local_bones_pos;
	inline __declspec( align( 16 ) ) c_quaternion* local_bones_quat;

	inline pose_params networked_poses;
	inline sdk::animlayer_t old_layers [ 15 ] {};

	void build_custom_matrix ( sdk::c_player * player , c_matrix3x4 * m , std::uint32_t max , std::uint32_t mask , float time );
	void in_createmove ( );
	void in_framestagenotify ( int stage );
}