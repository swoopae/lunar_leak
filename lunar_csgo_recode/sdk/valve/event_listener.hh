#pragma once

namespace sdk::valve {
	class event_t {
	public:
		virtual ~event_t ( ) = 0;
		virtual const char * get_name ( ) const = 0;

		virtual bool is_reliable ( ) const = 0;
		virtual bool is_local ( ) const = 0;
		virtual bool is_empty ( const char * key = nullptr ) = 0;

		virtual bool get_bool ( const char * key = nullptr , bool v = false ) = 0;
		virtual int get_int ( const char * key = nullptr , int v = 0 ) = 0;
		virtual unsigned long long get_uint64 ( const char * key = nullptr , unsigned long v = 0 ) = 0;
		virtual float get_float ( const char * key = nullptr , float v = 0.0f ) = 0;
		virtual const char * get_string ( const char * key = nullptr , const char * v = "" ) = 0;
		virtual const wchar_t * get_w_string ( const char * key , const wchar_t * v = L"" ) = 0;

		virtual void set_bool ( const char * key , bool value ) = 0;
		virtual void set_int ( const char * key , int value ) = 0;
		virtual void set_uint_64 ( const char * key , unsigned long value ) = 0;
		virtual void set_float ( const char * key , float value ) = 0;
		virtual void set_string ( const char * key , const char * value ) = 0;
		virtual void set_w_string ( const char * key , const wchar_t * value ) = 0;
	};

	class event_listener {
	public:
		virtual ~event_listener ( void ) { };

		virtual void fire_game_event ( event_t * event ) = 0;
		virtual int get_event_debug_id ( void ) = 0;
	};

	class c_event_manager {
	public:
		virtual ~c_event_manager ( ) = 0;
		virtual int load_events_from_file ( const char * filename ) = 0;
		virtual void reset ( ) = 0;
		virtual bool add_listener ( event_listener * listener , const char * name , bool server_side ) = 0;
		virtual bool find_listener ( event_listener * listener , const char * name ) = 0;
		virtual int remove_listener ( event_listener * listener ) = 0;
	};
}