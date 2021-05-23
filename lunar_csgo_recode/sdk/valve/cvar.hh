#pragma once

#include "app_system.hh"

#include "../math/matrix3x4.hh"

#include "../../shared/memory/memory.hh"
#include "../../shared/color/color.hh"

namespace sdk::valve {
	enum cvar_flags {
		fcvar_none = 0 ,
		fcvar_unregistered = ( 1 << 0 ) ,
		fcvar_developmentonly = ( 1 << 1 ) ,
		fcvar_gamedll = ( 1 << 2 ) ,
		fcvar_clientdll = ( 1 << 3 ) ,
		fcvar_hidden = ( 1 << 4 ) ,
		fcvar_protected = ( 1 << 5 ) ,
		fcvar_sponly = ( 1 << 6 ) ,
		fcvar_archive = ( 1 << 7 ) ,
		fcvar_notify = ( 1 << 8 ) ,
		fcvar_userinfo = ( 1 << 9 ) ,
		fcvar_printableonly = ( 1 << 10 ) ,
		fcvar_unlogged = ( 1 << 11 ) ,
		fcvar_never_as_string = ( 1 << 12 ) ,
		fcvar_replicated = ( 1 << 13 ) ,
		fcvar_cheat = ( 1 << 14 ) ,
		fcvar_ss = ( 1 << 15 ) ,
		fcvar_demo = ( 1 << 16 ) ,
		fcvar_dontrecord = ( 1 << 17 ) ,
		fcvar_ss_added = ( 1 << 18 ) ,
		fcvar_release = ( 1 << 19 ) ,
		fcvar_reload_materials = ( 1 << 20 ) ,
		fcvar_reload_textures = ( 1 << 21 ) ,
		fcvar_not_connected = ( 1 << 22 ) ,
		fcvar_material_system_thread = ( 1 << 23 ) ,
		fcvar_archive_xbox = ( 1 << 24 ) ,
		fcvar_accessible_from_threads = ( 1 << 25 ) ,
		fcvar_server_can_execute = ( 1 << 28 ) ,
		fcvar_server_cannot_query = ( 1 << 29 ) ,
		fcvar_clientcmd_can_execute = ( 1 << 30 ) ,
		fcvar_meme_dll = ( 1 << 31 ) ,
		fcvar_material_thread_mask = ( fcvar_reload_materials | fcvar_reload_textures | fcvar_material_system_thread )
	};

	class c_convar {
	public:
		void set_value ( const char * value ) {
			using original_fn = void ( __thiscall * )( c_convar * , const char * );
			return ( *( original_fn ** ) this ) [ 14 ] ( this , value );
		}
		void set_value ( float value ) {
			using original_fn = void ( __thiscall * )( c_convar * , float );
			return ( *( original_fn ** ) this ) [ 15 ] ( this , value );
		}
		void set_value ( int value ) {
			using original_fn = void ( __thiscall * )( c_convar * , int );
			return ( *( original_fn ** ) this ) [ 16 ] ( this , value );
		}
		void set_value ( bool value ) {
			using original_fn = void ( __thiscall * )( c_convar * , int );
			return ( *( original_fn ** ) this ) [ 16 ] ( this , static_cast< int >( value ) );
		}

		float get_float ( ) {
			const auto val = *reinterpret_cast< std::uint32_t * >( &float_value );
			auto xored = static_cast< std::uint32_t >( val ^ reinterpret_cast< std::uint32_t >( this ) );
			return *reinterpret_cast< float * >( &xored );
		}

		std::int32_t get_int ( ) {
			const auto val = *reinterpret_cast< std::uint32_t * >( &numerical_value );
			auto xored = static_cast< std::uint32_t >( val ^ reinterpret_cast< std::uint32_t >( this ) );
			return *reinterpret_cast< std::int32_t * >( &xored );
		}

	public:
		pad ( 4 );
		c_convar * next;
		std::int32_t is_registered;
		char * name;
		char * help_string;
		std::int32_t flags;
		pad ( 4 );
		c_convar * parent;
		char * default_value;
		char * string;
		std::int32_t string_length;
		float float_value;
		std::int32_t numerical_value;
		std::int32_t has_min;
		float min;
		std::int32_t has_max;
		float max;
		void * change_callbacks;
	};

	class c_console_display_func {
	public:
		virtual void color_print ( const std::uint8_t * clr , const char * pMessage ) = 0;
		virtual void print ( const char * pMessage ) = 0;
		virtual void d_print ( const char * pMessage ) = 0;
	};

	class c_convar_system : public c_app_system {
	public:
		virtual int allocate_dll_indentifier ( ) = 0;
		virtual void register_con_command ( void * pCommandBase ) = 0;
		virtual void unregister_con_command ( void * pCommandBase ) = 0;
		virtual void unregister_con_commands ( int id ) = 0;
		virtual const char * get_command_line_value ( const char * pVariableName ) = 0;
		virtual void * find_command_base ( const char * name ) = 0;
		virtual const void * find_command_base ( const char * name ) const = 0;
		virtual c_convar * get_convar ( const char * var_name ) = 0;
		virtual const c_convar * get_convar ( const char * var_name ) const = 0;
		virtual void * find_command ( const char * name ) = 0;
		virtual const void * find_command ( const char * name ) const = 0;
		virtual void install_global_change_callback ( void * callback ) = 0;
		virtual void remove_global_change_callback ( void * callback ) = 0;
		virtual void call_global_change_callbacks ( c_convar * var , const char * pOldString , float flOldValue ) = 0;
		virtual void install_console_display_func ( c_console_display_func * pDisplayFunc ) = 0;
		virtual void remove_console_display_func ( c_console_display_func * pDisplayFunc ) = 0;
		virtual void console_color_printf ( const c_color & clr , const char * pFormat , ... ) const = 0;
		virtual void console_printf ( const char * pFormat , ... ) const = 0;
		virtual void donsole_dprintf ( const char * pFormat , ... ) const = 0;
		virtual void rever_flagged_convars ( int nFlag ) = 0;
	};
}