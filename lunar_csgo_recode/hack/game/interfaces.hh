#pragma once

#ifndef LUNAR_INTERFACES
#define LUNAR_INTERFACES

#include "addresses.hh"

#include "../../sdk/sdk.hh"

#include <d3d9.h>

namespace hack::game::interfaces {
	/* interfaces */
	inline sdk::valve::c_client * client;
	inline sdk::valve::c_engine * engine;
	inline sdk::valve::c_input_system * input_system;
	inline sdk::valve::c_panel * panel;
	inline sdk::valve::c_entity_list * entity_list;
	inline sdk::valve::c_surface * surface;
	inline sdk::valve::c_input * input;
	inline sdk::valve::c_renderview * render_view;
	inline sdk::valve::c_prediction * prediction;
	inline sdk::valve::c_game_movement * game_movement;
	inline sdk::valve::c_move_helper * move_helper;
	inline sdk::valve::c_global_vars * global_vars;
	inline sdk::valve::c_surface_props * surface_props;
	inline sdk::valve::c_convar_system * convar_system;
	inline sdk::valve::c_model_info * model_info;
	inline sdk::valve::c_debug_overlay * debug_overlay;
	inline sdk::valve::c_event_manager * event_manager;
	inline sdk::valve::c_view_render_beams * view_render_beams;
	inline sdk::valve::c_engine_trace * engine_trace;
	inline sdk::valve::c_clientstate * clientstate;
	inline sdk::valve::c_materialsystem * material_system;
	inline sdk::valve::c_classmap * class_map;
	inline void * dxdevice;

	/* util functions */

	typedef void * ( *create_t )( );

	class c_interface_reg {
	public:
		create_t create;
		const char * name;
		c_interface_reg * next;
	};

	static std::string get_first_number ( std::string const & str ) {
		std::size_t const n = str.find_first_of ( STR ( "0123456789" ) );
		if ( n != std::string::npos ) {
			std::size_t const m = str.find_first_not_of ( STR ( "0123456789" ) , n );
			return str.substr ( n , m != std::string::npos ? m - n : m );
		}
		return std::string ( );
	}

	static std::uintptr_t get_jmp_target ( uintptr_t ptr ) {
		const auto relative = *reinterpret_cast< int * >( ptr + 1 );
		ptr += 5;
		ptr += relative;
		return ptr;
	}

	__forceinline static void * find_interface ( std::uintptr_t module_data , std::string name ) {
		auto itf = fnv1a_rt ( name.data ( ) );

		const auto jmp = shared::memory::get_proc_address ( module_data , fnv1a ( "CreateInterface" ) ) + 4;
		const auto offset = *reinterpret_cast< std::int32_t * >( jmp + 1 );
		const auto internal = jmp + offset + 5;
		const auto interface_list = **reinterpret_cast< c_interface_reg *** >( internal + 6 );

		c_interface_reg * highest_interface_ptr = nullptr;
		auto highest_interface_version = -1;

		for ( auto current = interface_list; current; current = current->next ) {
			char versionless_name [ 64 ] = {};
			std::size_t size;

			for ( size = 0; current->name [ size ] != '\0' && !std::isdigit ( current->name [ size ] ); ++size )
				versionless_name [ size ] = current->name [ size ];
			versionless_name [ size ] = '\0';

			if ( fnv1a_rt ( versionless_name ) == itf ) {
				const auto version = atoi ( get_first_number ( current->name ).data ( ) ); /* no error handling boss */
				if ( version > highest_interface_version ) {
					highest_interface_version = version;
					highest_interface_ptr = current;
				}
			}
		}

		if ( !highest_interface_ptr ) {
		#if defined(_DEBUG) || defined(LUNAR_DEV)
			std::cout << "[ERROR] - couldn't find interface " << name << "\n";
		#endif
			return nullptr;
		}

		auto to_ret = highest_interface_ptr->create ( );

	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[info] - found interface " << highest_interface_ptr->name << " in module " << module_data << " at address " << to_ret << "\n";
	#endif

		return highest_interface_ptr->create ( );
	}

	/* boss */
	/* we should also download these :) */
	static bool grab_interfaces ( ) {
		client = reinterpret_cast< sdk::valve::c_client * >( find_interface ( GRAB_MODULE ( MODULE_CLIENT_PANORAMA ) , STR ( "VClient" ) ) );
		if ( !client )
			return false;

		entity_list = reinterpret_cast< sdk::valve::c_entity_list * >( find_interface ( GRAB_MODULE ( MODULE_CLIENT_PANORAMA ) , STR ( "VClientEntityList" ) ) );
		if ( !entity_list )
			return false;

		engine = reinterpret_cast< sdk::valve::c_engine * >( find_interface ( GRAB_MODULE ( MODULE_ENGINE ) , STR ( "VEngineClient" ) ) );
		if ( !engine )
			return false;

		input_system = reinterpret_cast< sdk::valve::c_input_system * >( find_interface ( GRAB_MODULE ( MODULE_INPUT_SYSTEM ) , STR ( "InputSystemVersion" ) ) );
		if ( !input_system )
			return false;

		panel = reinterpret_cast< sdk::valve::c_panel * >( find_interface ( GRAB_MODULE ( MODULE_VGUI ) , STR ( "VGUI_Panel" ) ) );
		if ( !panel )
			return false;

		surface = reinterpret_cast< sdk::valve::c_surface * >( find_interface ( GRAB_MODULE ( MODULE_VGUI_MAT_SURFACE ) , STR ( "VGUI_Surface" ) ) );
		if ( !surface )
			return false;

		surface_props = reinterpret_cast< sdk::valve::c_surface_props * >( find_interface ( GRAB_MODULE ( MODULE_VPHYSICS ) , STR ( "VPhysicsSurfaceProps" ) ) );
		if ( !surface_props )
			return false;

		convar_system = reinterpret_cast< sdk::valve::c_convar_system * >( find_interface ( GRAB_MODULE ( MODULE_VSTDLIB ) , STR ( "VEngineCvar" ) ) );
		if ( !convar_system )
			return false;

		model_info = reinterpret_cast< sdk::valve::c_model_info * >( find_interface ( GRAB_MODULE ( MODULE_ENGINE ) , STR ( "VModelInfoClient" ) ) );
		if ( !model_info )
			return false;

		engine_trace = reinterpret_cast< sdk::valve::c_engine_trace * >( find_interface ( GRAB_MODULE ( MODULE_ENGINE ) , STR ( "EngineTraceClient" ) ) );
		if ( !engine_trace )
			return false;

		debug_overlay = reinterpret_cast< sdk::valve::c_debug_overlay * >( find_interface ( GRAB_MODULE ( MODULE_ENGINE ) , STR ( "VDebugOverlay" ) ) );
		if ( !debug_overlay )
			return false;

		render_view = reinterpret_cast< sdk::valve::c_renderview * >( find_interface ( GRAB_MODULE ( MODULE_ENGINE ) , STR ( "VEngineRenderView" ) ) );
		if ( !render_view )
			return false;

		game_movement = reinterpret_cast< sdk::valve::c_game_movement * >( find_interface ( GRAB_MODULE ( MODULE_CLIENT_PANORAMA ) , STR ( "GameMovement" ) ) );
		if ( !game_movement )
			return false;

		prediction = reinterpret_cast< sdk::valve::c_prediction * >( find_interface ( GRAB_MODULE ( MODULE_CLIENT_PANORAMA ) , STR ( "VClientPrediction" ) ) );
		if ( !prediction )
			return false;

		event_manager = reinterpret_cast< sdk::valve::c_event_manager * >( find_interface ( GRAB_MODULE ( MODULE_ENGINE ) , STR ( "GAMEEVENTSMANAGER" ) ) );
		if ( !event_manager )
			return false;	
		
		material_system = reinterpret_cast< sdk::valve::c_materialsystem * >( find_interface ( GRAB_MODULE ( MODULE_MATERIAL_SYSTEM ) , STR ( "VMaterialSystem" ) ) );
		if ( !material_system )
			return false;

		clientstate = **reinterpret_cast< sdk::valve::c_clientstate *** > ( shared::memory::v_func<std::uintptr_t> ( engine , 12 ) + 0x10 );
		if ( !clientstate )
			return false;

		input = *reinterpret_cast< sdk::valve::c_input ** >( hack::game::addresses::misc::cinput_address );
		if ( !input )
			return false;

		move_helper = **reinterpret_cast< sdk::valve::c_move_helper *** >( hack::game::addresses::misc::cmovehelper_address );
		if ( !move_helper )
			return false;

		view_render_beams = *reinterpret_cast< sdk::valve::c_view_render_beams ** >( hack::game::addresses::misc::cviewrenderbeams_address );
		if ( !view_render_beams )
			return false;

		dxdevice = **reinterpret_cast< IDirect3DDevice9 *** >( hack::game::addresses::misc::idx9_address );
		if ( !dxdevice )
			return false;
		
		global_vars = **reinterpret_cast< sdk::valve::c_global_vars *** >( ( *reinterpret_cast< std::uintptr_t ** >( hack::game::interfaces::client ) ) [ 0 ] + 0x1Fu );
		if ( !global_vars )
			return false;

		return true;
	}
}

#endif /* LUNAR_INTERFACES */