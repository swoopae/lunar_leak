#pragma once

#ifndef LUNAR_ADDRESSES
#define LUNAR_ADDRESSES

#include "modules.hh"

#include "../../shared/memory/memory.hh"

namespace hack::game::addresses {
	namespace functions {
		/* vtable addresses */
		inline std::uintptr_t ccsplayervtable_address;
		inline std::uintptr_t ccsanimstate_address;
		inline std::uintptr_t iclientrenderablevtable_address;
		inline std::uintptr_t imaterialvtable_address;

		/* functions we hook addresses */
		inline std::uintptr_t createmove_address;
		inline std::uintptr_t overrideview_address;
		inline std::uintptr_t buildtransofrmations_address;
		inline std::uintptr_t standardblendingrules_address;
		inline std::uintptr_t fsn_address;
		inline std::uintptr_t doenginepostprocessing_address;
		inline std::uintptr_t lockcursor_address;
		inline std::uintptr_t getviewangles_address;
		inline std::uintptr_t doproceduralfootplant_address;
		inline std::uintptr_t extraboneprocessing_address;
		inline std::uintptr_t senddatagram_address;
		inline std::uintptr_t shouldskipanimframe_address;
		inline std::uintptr_t staticpropdebug_address;
		inline std::uintptr_t setupbones_address;
		inline std::uintptr_t calculateviewbob_address;
		inline std::uintptr_t animstateupdate_address;
		inline std::uintptr_t createentity_address;
		inline std::uintptr_t clmove_address;
		inline std::uintptr_t processpacket_address;
		inline std::uintptr_t performprediction_address;
		inline std::uintptr_t shouldinterpolate_address;
		inline std::uintptr_t doanimationevent_address;
		inline std::uintptr_t vertexbufferdx10lock_address;
		inline std::uintptr_t interpolateserverentities_address;
	}

	namespace misc {
		/* interfaces */
		inline std::uintptr_t cinput_address;
		inline std::uintptr_t cmovehelper_address;
		inline std::uintptr_t cviewrenderbeams_address;
		inline std::uintptr_t idx9_address;
	}

	namespace winapi {
		inline std::uintptr_t ntprotectvirtualmemory_address;
	}

	/* TODO: network me for le anticrack ( or onetap edition: crack gets updated for months by russians ) */
	/* might wanna download and erase these strings :) */
	static bool grab_addresses ( ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[info] - grabbing all required addresses...\n";
	#endif
		/* vtable addresses */
		functions::ccsplayervtable_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) ) + 71;
		if ( !functions::ccsplayervtable_address || functions::ccsplayervtable_address == 71 )
			return false;

		functions::ccsanimstate_address = *reinterpret_cast< std::uintptr_t * >( sig ( MODULE_CLIENT_PANORAMA , STR ( "E8 ? ? ? ? A1 ? ? ? ? C7 01 ? ? ? ? C7 81 ? ? ? ? ? ? ? ?" ) ) + 12 ) + 8; /* originally 12 but + 8 because we need IPlayerAnimState */
		if ( !functions::ccsanimstate_address || functions::ccsanimstate_address == 12 )
			return false;

		functions::iclientrenderablevtable_address = functions::ccsplayervtable_address + 7;
		if ( !functions::iclientrenderablevtable_address )
			return false;

		functions::imaterialvtable_address = sig ( MODULE_MATERIAL_SYSTEM , STR ( "C7 06 ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? 89 4D E8" ) ) + 2;
		if ( !functions::imaterialvtable_address || functions::imaterialvtable_address == 2 )
			return false;

		/* interfaces */
		misc::cinput_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) ) + 1;
		if ( !misc::cinput_address )
			return false;

		misc::cmovehelper_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) ) + 2;
		if ( !misc::cmovehelper_address )
			return false;

		misc::cviewrenderbeams_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) ) + 1;
		if ( !misc::cviewrenderbeams_address )
			return false;

		misc::idx9_address = sig ( MODULE_SHADER_API_DX9 , STR ( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ) + 1;
		if ( !misc::idx9_address )
			return false;

		/* functions we hook addresses */
		functions::createmove_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 8B 0D ? ? ? ? 85 C9 75 06 B0" ) );
		if ( !functions::createmove_address )
			return false;

		functions::overrideview_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 83 EC 58 56 57 8B 3D ? ? ? ? 85 FF 0F 84" ) );
		if ( !functions::overrideview_address )
			return false;

		functions::buildtransofrmations_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 1C" ) );
		if ( !functions::buildtransofrmations_address )
			return false;

		functions::standardblendingrules_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC FF 75 18 F3 0F 10 45 ? 51" ) );
		if ( !functions::standardblendingrules_address )
			return false;

		functions::fsn_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0 A2" ) );
		if ( !functions::fsn_address )
			return false;

		functions::lockcursor_address = sig ( MODULE_VGUI_MAT_SURFACE , STR ( "80 3D ? ? ? ? 00 8B 91 A4 02 00 00 8B 0D ? ? ? ? C6 05 ? ? ? ? 01" ) );
		if ( !functions::lockcursor_address )
			return false;

		functions::getviewangles_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "56 8B F1 85 F6 74 32" ) );
		if ( !functions::getviewangles_address )
			return false;

		functions::doproceduralfootplant_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56 60" ) );
		if ( !functions::doproceduralfootplant_address )
			return false;

		functions::extraboneprocessing_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C" ) );
		if ( !functions::extraboneprocessing_address )
			return false;

		functions::senddatagram_address = sig ( MODULE_ENGINE , STR ( "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 89 7C 24 18" ) );
		if ( !functions::senddatagram_address )
			return false;

		functions::shouldskipanimframe_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3" ) );
		if ( !functions::shouldskipanimframe_address )
			return false;

		functions::doenginepostprocessing_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "E8 ? ? ? ? 8B 74 24 54 83 C4 18" ) );
		if ( !functions::doenginepostprocessing_address )
			return false;

		functions::animstateupdate_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) );
		if ( !functions::animstateupdate_address )
			return false;

		functions::createentity_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 51 53 0F B7 59 16 56 33 F6 57 85 DB 7E 35" ) );
		if ( !functions::createentity_address )
			return false;

		functions::staticpropdebug_address = sig ( MODULE_ENGINE , STR ( "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D" ) );
		if ( !functions::staticpropdebug_address )
			return false;

		functions::setupbones_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 8B 0D" ) );
		if ( !functions::setupbones_address )
			return false;

		functions::calculateviewbob_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9" ) );
		if ( !functions::calculateviewbob_address )
			return false;

		functions::clmove_address = sig ( MODULE_ENGINE , STR ( "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A" ) );
		if ( !functions::clmove_address )
			return false;

		functions::processpacket_address = sig ( MODULE_ENGINE , STR ( "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 56 57 8B 7D 08 8B D9" ) );
		if ( !functions::processpacket_address )
			return false;

		functions::shouldinterpolate_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "F6 81 ? ? ? ? ? 74 2F" ) );
		if ( !functions::shouldinterpolate_address )
			return false;

		functions::doanimationevent_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 51 53 8B D9 56 57 8B 8B" ) );
		if ( !functions::doanimationevent_address )
			return false;

		functions::vertexbufferdx10lock_address = sig ( MODULE_SHADER_API_DX9 , STR ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 57" ) );
		if ( !functions::vertexbufferdx10lock_address )
			return false;

		functions::interpolateserverentities_address = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 EC 1C 8B 0D ? ? ? ? 53 56" ) );
		if ( !functions::interpolateserverentities_address )
			return false;

		// functions::performprediction_address = ref_sig ( MODULE_CLIENT_PANORAMA , STR ( "E8 ? ? ? ? 8B 4D F8 5B" ) );
		// if ( !functions::performprediction_address )
		// 	return false;

	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[info] - grabbed all addresses\n";
	#endif
		return true;
	}
}

#endif