#pragma once

namespace sdk::valve {
	typedef void * ( *make_interface_fn )( const char * name , int * return_code );

	class c_app_system {
	public:
		virtual bool connect ( make_interface_fn factory ) = 0;
		virtual void disconnect ( ) = 0;
		virtual void * query_interface ( const char * p_interface_name ) = 0;
		virtual int init ( ) = 0;
		virtual void shutdown ( ) = 0;
		virtual const void * get_dependencies ( ) = 0;
		virtual int get_tier ( ) = 0;
		virtual void reconnect ( make_interface_fn factory , const char * p_interface_name ) = 0;
		virtual void unk_func ( ) = 0;
	};
}