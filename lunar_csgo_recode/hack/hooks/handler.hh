#pragma once

#include "../game/interfaces.hh"

#include "../../external/hooking/minhook.hh"

#include <d3d9.h>
#include "../game/globals.hh"

namespace hack::hooks {
	/* function prototypes */
	bool __fastcall createmove_hk ( x86reg , float sampletime , sdk::valve::ucmd_t * cmd );
	void __fastcall lockcursor_hk ( x86reg );
	void __fastcall levelshutdown_hk ( x86reg );
	void __fastcall levelinitpostentity_hk ( x86reg );
	void __fastcall levelinitpreentity_hk ( x86reg );
	bool __fastcall svcheatsgetbool_hk ( x86reg );
	long __fastcall endscene_hk ( x86reg , IDirect3DDevice9 * device );
	long __fastcall reset_hk ( x86reg , IDirect3DDevice9 * device , D3DPRESENT_PARAMETERS * presentation_parameters );
	void __fastcall framestagenotify_hk ( x86reg , int stage );
	void __fastcall painttraverse_hk ( x86reg , std::uint32_t vguipanel , bool repaint , bool force );
	void __fastcall overrideview_hk ( x86reg , sdk::valve::c_viewsetup * setup );
	void __fastcall calcviewbob_hk ( x86reg , c_vector3 & eye_origin );
	void __fastcall doproceduralfootplant_hk ( x86reg , void * bonetoworld , void * leftfootchain , void * rightfootchain , void * pos );
	bool __fastcall fireevent_hk ( x86reg );
	bool __fastcall shouldskipanimframe_hk ( x86reg );
	bool __fastcall ishltv_hk ( x86reg );
	bool __fastcall setupbones_hk ( x86reg , c_matrix3x4 * bone_to_world , int max_bones , int bone_mask , float time );
	bool __fastcall doenginepostprocessing_hk ( x86reg , int x , int y , int w , int h , bool flashlight , bool postvgui );
	void __fastcall updateclientsideanimations_hk ( x86reg );
	void __fastcall doextraboneprocessing_hk ( x86reg , int a2 , int a3 , int a4 , int a5 , int a6 , int a7 );
	void __fastcall buildtransformations_hk ( x86reg , int a2 , c_vector3 * a3 , c_quaternion * a4 , int a5 , int a6 , int a7 );
	void __fastcall standardblendingrules_hk ( x86reg , int a2 , int a3 , int a4 , int a5 , int a6 );
	bool __fastcall writeucmddeltatobuffer_hk ( x86reg , int a1 , void * a2 , int a3 , int a4 , bool a5 );
	bool __fastcall inprediction_hk ( x86reg );
	void __fastcall runcommand_hk ( x86reg , sdk::c_player * player , sdk::valve::ucmd_t * cmd , sdk::valve::c_move_helper * helper );
	void __fastcall performprediction_hk ( x86reg , int slot , sdk::c_player * local , bool received_new_world_update , int incoming_acknowledged , int outgoing_command );
	void __fastcall drawprinttext_hk ( x86reg , const wchar_t * text , int text_length , void * draw_type );
	bool __fastcall shouldinterpolate_hk ( x86reg );
	void __fastcall interpolateserverentities_hk ( x86reg );
	void __fastcall doanimationevent_hk ( x86reg , void * anim_event , int data );
	void __fastcall vertexbufferdx10lock_hk ( x86reg , int max_vertex_count , bool append , void * unk );
	void __fastcall getcolormodulation_hk ( x86reg , float * r , float * g , float * b );
	bool __fastcall isusingstaticpropdebug_hk ( x86reg );

	void __vectorcall update_hk ( x86reg , float a1 , float a2 , float a3 , void * a4 );
	void __cdecl clmove_hk ( float samples , bool final_tick );
	void * __fastcall createentity_hk ( x86reg , const char * mapname );
	c_vector3 * __fastcall getviewangles_hk ( x86reg );
	float __fastcall getalphamodulation_hk ( x86reg );

	/* original functions */
	namespace originals {
		inline decltype( &createmove_hk ) o_createmove = nullptr;
		inline decltype( &lockcursor_hk ) o_lockcursor = nullptr;
		inline decltype( &svcheatsgetbool_hk ) o_svcheatsgetbool = nullptr;
		inline decltype( &endscene_hk ) o_endscene = nullptr;
		inline decltype( &reset_hk ) o_reset = nullptr;
		inline decltype( &framestagenotify_hk ) o_framestagenotify = nullptr;
		inline decltype( &painttraverse_hk ) o_painttraverse = nullptr;
		inline decltype( &overrideview_hk ) o_overrideview = nullptr;
		inline decltype( &calcviewbob_hk ) o_calcviewbob = nullptr;
		inline decltype( &doproceduralfootplant_hk ) o_doproceduralfootplant = nullptr;
		inline decltype( &fireevent_hk ) o_fireevent = nullptr;
		inline decltype( &shouldskipanimframe_hk ) o_shouldskipanimframe = nullptr;
		inline decltype( &ishltv_hk ) o_ishltv = nullptr;
		inline decltype( &setupbones_hk ) o_setupbones = nullptr;
		inline decltype( &updateclientsideanimations_hk ) o_updateclientsideanimations = nullptr;
		inline decltype( &doenginepostprocessing_hk ) o_doenginepostprocessing = nullptr;
		inline decltype( &getviewangles_hk ) o_getviewangles = nullptr;
		inline decltype( &levelshutdown_hk ) o_levelshutdown = nullptr;
		inline decltype( &levelinitpostentity_hk ) o_levelinitpostentity = nullptr;
		inline decltype( &levelinitpreentity_hk ) o_levelinitpreentity = nullptr;
		inline decltype( &createentity_hk ) o_createentity = nullptr;
		inline decltype( &doextraboneprocessing_hk ) o_doextraboneprocessing = nullptr;
		inline decltype( &buildtransformations_hk ) o_buildtransformations = nullptr;
		inline decltype( &standardblendingrules_hk ) o_standardblendingrules = nullptr;
		inline decltype( &writeucmddeltatobuffer_hk ) o_writeucmddelta = nullptr;
		inline decltype( &clmove_hk ) o_clmove = nullptr;
		inline decltype( &inprediction_hk ) o_inprediction = nullptr;
		inline decltype( &runcommand_hk ) o_runcommand = nullptr;
		inline decltype( &update_hk ) o_update = nullptr;
		inline decltype( &performprediction_hk ) o_performprediction = nullptr;
		inline decltype( &drawprinttext_hk ) o_drawprinttext = nullptr;
		inline decltype( &shouldinterpolate_hk ) o_shouldinterpolate = nullptr;
		inline decltype( &doanimationevent_hk ) o_doanimationevent = nullptr;
		inline decltype( &vertexbufferdx10lock_hk ) o_vertexbufferlock = nullptr;
		inline decltype( &getcolormodulation_hk ) o_getcolormodulation = nullptr;
		inline decltype( &isusingstaticpropdebug_hk ) o_isusingstaticpropdebug = nullptr;
		inline decltype( &getalphamodulation_hk ) o_getalphamodulation = nullptr;
		inline decltype( &interpolateserverentities_hk ) o_interpolateserverentities = nullptr;
	}

	/* init */
	static bool enable_hooks ( ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[info] - hooking all required functions...\n";
	#endif
		/* TODO: ignore this and replace with proper variant ~swoopae */
		/* lolll enjoy vac banne AXAXAX */
		const auto remove_choked_limit = [ ] ( ) {
			/* hey tommy & uber do u remember when we added this to wanheda and then like a few days later it was public on uc? we were like the only cheat to have 16 tick fl back then lmao throwback */
			const static auto fuck = sig ( MODULE_ENGINE , STR ( "B8 ? ? ? ? 3B F0 0F 4F F0 89 5D FC" ) ) + 1;

			unsigned long protect = 0;

			VirtualProtect ( ( void * ) fuck , 4 , PAGE_EXECUTE_READWRITE , &protect );
			*( std::uint32_t * )fuck = 62;
			VirtualProtect ( ( void * ) fuck , 4 , protect , &protect );
		};

		/* grab extra addresses */
		const auto devicereset_address = reinterpret_cast< decltype ( &reset_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::dxdevice , 16 ) );
		const auto deviceendscene_address = reinterpret_cast< decltype ( &endscene_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::dxdevice , 42 ) );
		const auto levelinitpreentity_address = reinterpret_cast< decltype ( &levelinitpreentity_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::client , 5 ) );
		const auto levelinitpostentity_address = reinterpret_cast< decltype ( &levelinitpostentity_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::client , 6 ) );
		const auto levelshutdown_address = reinterpret_cast< decltype ( &levelshutdown_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::client , 7 ) );
		const auto painttraverse_address = reinterpret_cast< decltype ( &painttraverse_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::panel , 41 ) );
		const auto svcheatsgetbool_address = reinterpret_cast< decltype ( &svcheatsgetbool_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::convar_system->get_convar ( STR ( "sv_cheats" ) ) , 13 ) );
		const auto fireevent_address = reinterpret_cast< decltype( &fireevent_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::engine , 59 ) );
		const auto ishltv_address = reinterpret_cast< decltype( &ishltv_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::engine , 93 ) );
		const auto updateclientsideanimations_address = reinterpret_cast< decltype( &updateclientsideanimations_hk ) >( shared::memory::v_func<std::uintptr_t> ( reinterpret_cast< void * >( hack::game::addresses::functions::ccsplayervtable_address ) , 223 ) );
		const auto doanimationevent_address = reinterpret_cast< decltype( &doanimationevent_hk ) >( shared::memory::v_func<std::uintptr_t> ( reinterpret_cast< void * >( hack::game::addresses::functions::ccsanimstate_address ) , 6 ) );
		const auto getcolormodulation_address = reinterpret_cast< decltype( &getcolormodulation_hk ) >( shared::memory::v_func<std::uintptr_t> ( reinterpret_cast< void * >( hack::game::addresses::functions::imaterialvtable_address ) , 45 ) );
		const auto getalphamodulation_address = reinterpret_cast< decltype( &getalphamodulation_hk ) >( shared::memory::v_func<std::uintptr_t> ( reinterpret_cast< void * >( hack::game::addresses::functions::imaterialvtable_address ) , 44 ) );
		const auto writeucmd_address = reinterpret_cast< decltype( &writeucmddeltatobuffer_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::client , 24 ) );
		const auto inprediction_address = reinterpret_cast< decltype( &inprediction_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::prediction , 14 ) );
		const auto runcommand_address = reinterpret_cast< decltype( &runcommand_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::prediction , 19 ) );
		const auto drawprinttext_address = reinterpret_cast< decltype( &drawprinttext_hk ) >( shared::memory::v_func<std::uintptr_t> ( hack::game::interfaces::surface , 28 ) );

		if ( init_minhook ( ) != MH_OK )
			return false;

		/* create hooks with addresses from the server */
		create_detour_hook ( reinterpret_cast< decltype ( &createmove_hk ) >( hack::game::addresses::functions::createmove_address ) , createmove_hk , ( void ** ) &originals::o_createmove );
		create_detour_hook ( reinterpret_cast< decltype ( &framestagenotify_hk ) >( hack::game::addresses::functions::fsn_address ) , framestagenotify_hk , ( void ** ) &originals::o_framestagenotify );
		create_detour_hook ( reinterpret_cast< decltype ( &lockcursor_hk ) >( hack::game::addresses::functions::lockcursor_address ) , lockcursor_hk , ( void ** ) &originals::o_lockcursor );
		create_detour_hook ( reinterpret_cast< decltype ( &overrideview_hk ) >( hack::game::addresses::functions::overrideview_address ) , overrideview_hk , ( void ** ) &originals::o_overrideview );
		create_detour_hook ( reinterpret_cast< decltype ( &calcviewbob_hk ) >( hack::game::addresses::functions::calculateviewbob_address ) , calcviewbob_hk , ( void ** ) &originals::o_calcviewbob );
		create_detour_hook ( reinterpret_cast< decltype ( &doproceduralfootplant_hk ) >( hack::game::addresses::functions::doproceduralfootplant_address ) , doproceduralfootplant_hk , ( void ** ) &originals::o_doproceduralfootplant );
		create_detour_hook ( reinterpret_cast< decltype ( &shouldskipanimframe_hk ) >( hack::game::addresses::functions::shouldskipanimframe_address ) , shouldskipanimframe_hk , ( void ** ) &originals::o_shouldskipanimframe );
		create_detour_hook ( reinterpret_cast< decltype ( &setupbones_hk ) >( hack::game::addresses::functions::setupbones_address ) , setupbones_hk , ( void ** ) &originals::o_setupbones );
		create_detour_hook ( reinterpret_cast< decltype ( &getviewangles_hk ) >( hack::game::addresses::functions::getviewangles_address ) , getviewangles_hk , ( void ** ) &originals::o_getviewangles );
		create_detour_hook ( reinterpret_cast< decltype ( &doextraboneprocessing_hk ) >( hack::game::addresses::functions::extraboneprocessing_address ) , doextraboneprocessing_hk, ( void ** ) &originals::o_doextraboneprocessing );
		create_detour_hook ( reinterpret_cast< decltype ( &buildtransformations_hk ) >( hack::game::addresses::functions::buildtransofrmations_address ) , buildtransformations_hk, ( void ** ) &originals::o_buildtransformations );
		create_detour_hook ( reinterpret_cast< decltype ( &standardblendingrules_hk ) >( hack::game::addresses::functions::standardblendingrules_address ) , standardblendingrules_hk, ( void ** ) &originals::o_standardblendingrules );
		create_detour_hook ( reinterpret_cast< decltype ( &clmove_hk ) >( hack::game::addresses::functions::clmove_address ) , clmove_hk , ( void ** ) &originals::o_clmove );
		create_detour_hook ( reinterpret_cast< decltype ( &update_hk ) >( hack::game::addresses::functions::animstateupdate_address ) , update_hk , ( void ** ) &originals::o_update );
		create_detour_hook ( reinterpret_cast< decltype ( &vertexbufferdx10lock_hk ) >( hack::game::addresses::functions::vertexbufferdx10lock_address ) , vertexbufferdx10lock_hk , ( void ** ) &originals::o_vertexbufferlock );
		create_detour_hook ( reinterpret_cast< decltype ( &isusingstaticpropdebug_hk ) >( hack::game::addresses::functions::staticpropdebug_address ) , isusingstaticpropdebug_hk , ( void ** ) &originals::o_isusingstaticpropdebug );
		create_detour_hook ( reinterpret_cast< decltype ( &interpolateserverentities_hk ) >( hack::game::addresses::functions::interpolateserverentities_address ) , interpolateserverentities_hk , ( void ** ) &originals::o_interpolateserverentities );
		
		// create_detour_hook ( reinterpret_cast< decltype ( &shouldinterpolate_hk ) >( hack::game::addresses::functions::shouldinterpolate_address ) , shouldinterpolate_hk , ( void ** ) &originals::o_shouldinterpolate );
		// create_detour_hook ( reinterpret_cast< decltype ( &createentity_hk ) >( hack::game::addresses::functions::createentity_address ) , createentity_hk , ( void ** ) &originals::o_createentity );
		// create_detour_hook ( reinterpret_cast< decltype ( &doanimationevent_hk ) >( hack::game::addresses::functions::doanimationevent_address ) , doanimationevent_hk , ( void ** ) &originals::o_doanimationevent );
		// create_detour_hook ( reinterpret_cast< decltype ( &performprediction_hk ) >( hack::game::addresses::functions::performprediction_address ) , performprediction_hk , ( void ** ) &originals::o_performprediction );
		/* separate thread so it crashes game instantly with no debugger message thanks valve */
		// create_detour_hook ( reinterpret_cast< decltype ( &doenginepostprocessing_hk ) >( hack::game::addresses::functions::doenginepostprocessing_address ) , doenginepostprocessing_hk , ( void** ) &originals::o_doenginepostprocessing );

		/* create hooks with "static" addresses */
		create_detour_hook ( devicereset_address , reset_hk , ( void ** ) &originals::o_reset );
		create_detour_hook ( deviceendscene_address , endscene_hk , ( void ** ) &originals::o_endscene );
		create_detour_hook ( svcheatsgetbool_address , svcheatsgetbool_hk , ( void ** ) &originals::o_svcheatsgetbool );
		create_detour_hook ( fireevent_address , fireevent_hk , ( void ** ) &originals::o_fireevent );
		create_detour_hook ( ishltv_address , ishltv_hk , ( void ** ) &originals::o_ishltv );
		create_detour_hook ( updateclientsideanimations_address , updateclientsideanimations_hk , ( void ** ) &originals::o_updateclientsideanimations );
		/* TODO: these 2 functions */
		// create_detour_hook ( levelinitpreentity_address , levelinitpreentity_hk , ( void ** ) &originals::o_levelinitpreentity );
		// create_detour_hook ( levelinitpostentity_address , levelinitpostentity_hk , ( void ** ) &originals::o_levelinitpostentity );
		create_detour_hook ( levelshutdown_address , levelshutdown_hk , ( void ** ) &originals::o_levelshutdown );
		create_detour_hook ( writeucmd_address , writeucmddeltatobuffer_hk , ( void ** ) &originals::o_writeucmddelta );
		create_detour_hook ( inprediction_address , inprediction_hk , ( void ** ) &originals::o_inprediction );
		create_detour_hook ( runcommand_address , runcommand_hk , ( void ** ) &originals::o_runcommand );
		create_detour_hook ( drawprinttext_address , drawprinttext_hk , ( void ** ) &originals::o_drawprinttext );
		// create_detour_hook ( doanimationevent_address , doanimationevent_hk , ( void ** ) &originals::o_doanimationevent );
		create_detour_hook ( getcolormodulation_address , getcolormodulation_hk , ( void ** ) &originals::o_getcolormodulation );
		create_detour_hook ( getalphamodulation_address , getalphamodulation_hk , ( void ** ) &originals::o_getalphamodulation );
		// create_detour_hook ( painttraverse_address , painttraverse_hk , ( void** ) &originals::o_painttraverse );

		if ( enable_detour_hook ( MH_ALL_HOOKS ) != MH_OK )
			return false;

	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[info] - hooked all functions\n";
	#endif

		remove_choked_limit ( );

		return true;
	}

	/* uninit */
	static void disable_hooks ( ) {
		remove_detour_hook ( MH_ALL_HOOKS );

		uninit_minhook ( );

		if ( hack::game::globals::local )
			hack::game::globals::local->clientanims ( ) = true;

		hack::game::interfaces::input_system->enable_input ( true );
	}
}