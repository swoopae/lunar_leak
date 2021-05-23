#pragma once

#include "user_cmd.hh"

namespace sdk::valve {
	class c_move_helper {
	private:
		virtual void _vpad ( ) = 0;
	public:
		virtual void set_host ( void * host ) = 0;
	private:
		virtual void unknown1 ( ) = 0;
		virtual void unknown2 ( ) = 0;
	public:
		virtual bool process_impacts ( ) = 0;
	};

	struct movedata_t {
	public:
		bool m_bfirstrunoffunctions : 1;
		bool m_bgamecodemovedplayer : 1;
		bool m_bnoaircontrol : 1;
		void* m_nplayerhandle;    // edict index on server, client entity handle on client
		int m_nimpulsecommand;    // impulse command issued.
		c_vector3 m_vecviewangles;    // command view angles (local space)
		c_vector3 m_vecabsviewangles;    // command view angles (world space)
		int m_nbuttons;            // attack buttons.
		int m_noldbuttons;        // from host_client->oldbuttons;
		float m_flforwardmove;
		float m_flsidemove;
		float m_flupmove;
		float _m_flmaxspeed;
		float m_flclientmaxspeed;
		// variables from the player edict (sv_player) or entvars on the client.
		// these are copied in here before calling and copied out after calling.
		c_vector3 m_vecvelocity_;        // edict::velocity        // current movement direction.
		c_vector3 m_vecoldvelocity;
		float somefloat;
		c_vector3 m_vecangles;        // edict::angles
		c_vector3 m_vecoldangles;
		// output only
		float m_outstepheight;    // how much you climbed this move
		c_vector3 m_outwishvel;        // this is where you tried 
		c_vector3 m_outjumpvel;        // this is your jump velocity
		// movement constraints    (radius 0 means no constraint)
		c_vector3 m_vecconstraintcenter;
		float m_flconstraintradius;
		float m_flconstraintwidth;
		float m_flconstraintspeedfactor;
		bool m_bconstraintpastradius;        ///< if no, do no constraining past radius.  if yes, cap them to speedfactor past radius
		void setabsorigin ( const c_vector3 & vec );
		const c_vector3 & getabsorigin ( ) const;
	public:
		c_vector3            _m_vecabsorigin;        // edict::origin
		//pad ( 184 )
	};

	class c_game_movement {
	public:
		virtual	~c_game_movement ( void ) { }

		virtual void process_movement ( void * player , movedata_t * movement ) = 0;
		virtual void reset ( void ) = 0;
		virtual void start_track_prediction_errors ( void * player ) = 0;
		virtual void finish_track_prediction_errors ( void * player ) = 0;
		virtual void difference_print ( char const * fmt , ... ) = 0;
		virtual c_vector3 const & player_minimums ( bool ducked ) const = 0;
		virtual c_vector3 const & player_maximums ( bool ducked ) const = 0;
		virtual c_vector3 const & player_view_offset ( bool ducked ) const = 0;
		virtual bool is_moving_player_stuck ( void ) const = 0;
		virtual void * get_moving_player ( void ) const = 0;
		virtual void unblock_pusher ( void * player , void * pusher ) = 0;
		virtual void setup_movement_bounds ( movedata_t * movement ) = 0;
	};

	class c_prediction {
	public:
		virtual ~c_prediction ( void ) = 0;
		virtual void initialize ( void ) = 0;
		virtual void shutdown ( void ) = 0;
	public:
		virtual void update ( int startframe , bool validframe , int incoming_acknowledged , int outgoing_command );
		virtual void pre_entity_packet_recieved ( int commands_acknowledged , int current_world_update_packet );
		virtual void post_entity_packet_recieved ( void );
		virtual void post_network_data_recieved ( int commands_acknowledged );
		virtual void on_recieve_uncompressed_packed ( void );
		virtual void get_view_origin ( c_vector3 & org );
		virtual void set_view_origin ( c_vector3 & org );
		virtual void get_view_angles ( c_vector3 & ang );
		virtual void set_view_angles ( c_vector3 & ang );
		virtual void get_local_view_angles ( c_vector3 & ang );
		virtual void set_local_view_angles ( c_vector3 & ang );
		virtual bool in_prediction ( void ) const;
		virtual bool is_first_time_predicted ( void ) const;
		virtual int get_last_acknowledged_cmd_number ( void ) const;
		virtual int get_incoming_packet_number ( void ) const;
		virtual void check_moving_ground ( void * player , double frametime );
		virtual void run_command ( void * player , void * cmd , c_move_helper * moveHelper );
		virtual void setup_move ( void * player , void * cmd , c_move_helper * helper , movedata_t * movement );
		virtual void finish_move ( void * player , void * cmd , movedata_t * movement );
		virtual void set_ideal_pitch ( int slot , void * player , const c_vector3 & origin , const c_vector3 & angles , const c_vector3 & viewheight );
		virtual void check_error ( int slot , void * player , int commands_acknowledged );
	public:
		virtual void _update ( int nSlot , bool received_new_world_update , bool validframe , int incoming_acknowledged , int outgoing_command );
		bool perform_prediction ( int slot , void * local_player , bool received_new_world_update , int incoming_acknowledged , int outgoing_command );
		void shift_intermediate_data_forward ( int slot , int slots_to_remove , int previous_last_slot );
		void restore_entity_to_predicted_frame ( int slot , int predicted_frame );
		int first_command_to_execute ( int slot , bool received_new_world_update , int incoming_acknowledged , int outgoing_command );
		void dump_entity ( void * ent , int commands_acknowledged );
		void shutdown_predictables ( void );
		void reinit_predictables ( void );
		void remove_stale_predicted_entities ( int slot , int last_command_packet );
		void restore_original_ent_state ( int slot );
		void run_simulation ( int current_command , float curtime , ucmd_t * cmd , void * local_player );
		void untouch ( int slot );
		void store_prediction_results ( int slot , int predicted_frame );
		bool should_dump_entity ( void * ent );
		void smooth_view_on_moving_platform ( void * player , c_vector3 & offset );
		void reset_simulation_tick ( );
		void show_prediction_list_entry ( int list_row , int show_list , void * ent , int & totalsize , int & totalsize_intermediate );
		void finish_prediction_list ( int list_row , int show_list , int totalsize , int totalsize_intermediate );
		void check_predict_convar ( );

		/* values */
		pad ( 8 )
		bool m_in_prediction;
		pad ( 1 )
		bool m_engine_paused;
		pad ( 13 )
		bool m_is_first_time_predicted;
	};
}