#pragma once

#include <Windows.h>
#include <unordered_map>
#include <iostream>

#include "../../external/hasher/hasher.hh"
#include "../../external/xor/xor.hh"

/* game module name macros */
#define MODULE_CLIENT_PANORAMA 0
#define MODULE_ENGINE 1
#define MODULE_INPUT_SYSTEM 2
#define MODULE_VGUI 3
#define MODULE_GAME_OVERLAY_RENDERER 4
#define MODULE_TIER0 5
#define MODULE_VSTDLIB 6
#define MODULE_VPHYSICS 7
#define MODULE_MATERIAL_SYSTEM 8
#define MODULE_VGUI_MAT_SURFACE 9
#define MODULE_SHADER_API_DX9 10
#define MODULE_SERVER 11
#define MODULE_DATACACHE 12

/* find the address */
#define GRAB_MODULE( x ) hack::game::modules::game_modules.find ( x )._Ptr->_Myval.second

/* game modules */
namespace hack::game::modules {
	inline std::unordered_map<std::uint32_t , std::uintptr_t> game_modules;

	static bool emplace_module ( std::string_view module_name , std::uint32_t module_index ) {
		const auto module_addy = reinterpret_cast< std::uintptr_t >( GetModuleHandleA ( module_name.data ( ) ) );

		if ( !module_addy )
			return false;

		game_modules.emplace ( module_index , module_addy );

		return true;
	}

	static bool grab_modules ( ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[info] - grabbing all module addresses...\n";
	#endif

		/* lol wtf was i thinking lmaooooo -swoopae from the future */
		if ( !emplace_module ( STR ( "client.dll" ) , MODULE_CLIENT_PANORAMA ) )
			return false;

		if ( !emplace_module ( STR ( "engine.dll" ) , MODULE_ENGINE ) )
			return false;

		if ( !emplace_module ( STR ( "inputsystem.dll" ) , MODULE_INPUT_SYSTEM ) )
			return false;

		if ( !emplace_module ( STR ( "vgui2.dll" ) , MODULE_VGUI ) )
			return false;

		if ( !emplace_module ( STR ( "GameOverlayRenderer.dll" ) , MODULE_GAME_OVERLAY_RENDERER ) )
			return false;

		if ( !emplace_module ( STR ( "tier0.dll" ) , MODULE_TIER0 ) )
			return false;

		if ( !emplace_module ( STR ( "vstdlib.dll" ) , MODULE_VSTDLIB ) )
			return false;

		if ( !emplace_module ( STR ( "vphysics.dll" ) , MODULE_VPHYSICS ) )
			return false;

		if ( !emplace_module ( STR ( "materialsystem.dll" ) , MODULE_MATERIAL_SYSTEM ) )
			return false;

		if ( !emplace_module ( STR ( "vguimatsurface.dll" ) , MODULE_VGUI_MAT_SURFACE ) )
			return false;

		if ( !emplace_module ( STR ( "shaderapidx9.dll" ) , MODULE_SHADER_API_DX9 ) )
			return false;

		if ( !emplace_module ( STR ( "server.dll" ) , MODULE_SERVER ) )
			return false;

		if ( !emplace_module ( STR ( "datacache.dll" ) , MODULE_DATACACHE ) )
			return false;

	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[info] - grabbed all module addresses\n";
	#endif
		return true;
	}
}