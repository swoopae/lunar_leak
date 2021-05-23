#pragma once

#include "../../game/interfaces.hh"
#include "../../game/globals.hh"

#include <functional>
#include <array>

namespace hack::modules::prediction::engine_prediction {
	inline int * random_prediction_seed , * set_prediction_seed;
	inline float old_curtime , old_frametime;
	inline sdk::valve::movedata_t old_movedata;

	inline float correct_tickbase , correct_curtime;
	inline sdk::valve::ucmd_t * last_command;

	/* pretty sure weapon accuracypenalty also gets affected... ~swoopae */
	struct player_data_t {
		c_vector3 m_aimpunch , m_aimpunchvelocity , m_viewoffset , m_viewpunch , m_velocity , m_velocitybase;
		float m_duckamount , m_duckspeed , m_velocitymodifier , m_fallvelocity;
		int m_tickbase;
	};

	void start_prediction ( );
	void end_prediction ( );
	void repredict ( );
	void update_correct_globals ( );

	inline std::array<player_data_t , 155> data;
	inline bool in_custom_prediction;

	void store_netvars ( );
	void apply_netvars ( );

	void run_in_prediction ( std::function< void ( ) > fn );
}