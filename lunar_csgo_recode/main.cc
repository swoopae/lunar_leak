#include <windows.h>
#include <fstream>
#include <thread>

#include "hack/game/modules.hh"
#include "hack/game/globals.hh"
#include "hack/game/addresses.hh"
#include "hack/game/interfaces.hh"
#include "hack/game/netvars.hh"

#include "hack/hooks/handler.hh"

#include "hack/menu/framework.hh"

void open_debug_console ( ) {
#if defined(_DEBUG) || defined(LUNAR_DEV)
	AllocConsole ( );

	freopen_s ( ( FILE ** ) stdin , STR ( "CONIN$" ) , STR ( "r" ) , stdin );
	freopen_s ( ( FILE ** ) stdout , STR ( "CONOUT$" ) , STR ( "w" ) , stdout );

	/* boss */
	SetConsoleTitleA ( STR ( "[redacted cheatcodes]" ) );
#endif
}

void close_debug_console ( ) {
#if defined(_DEBUG) || defined(LUNAR_DEV)
	fclose ( ( FILE * ) stdin );
	fclose ( ( FILE * ) stdout );

	HWND console_hwnd = GetConsoleWindow ( );

	FreeConsole ( );
	PostMessageW ( console_hwnd , WM_CLOSE , 0 , 0 );
#endif
}

int dll_setup ( HMODULE this_module ) {
	/* grab this module */
	hack::game::globals::this_instance = this_module;

	/* don't do anything until game is fully loaded up */
	while ( !GetModuleHandleA ( STR ( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for ( std::chrono::seconds ( 1 ) );

	/* open debug console */
	open_debug_console ( );

#if defined(_DEBUG) || defined(LUNAR_DEV)
	std::cout << "[info] - cheatname - hello, swoopae!\n";
	std::cout << "[info] - compile date: " << __DATE__ << ' ' << __TIME__ << '\n';
#endif
	/* init our stuff */

	/* game modules */
	if ( !hack::game::modules::grab_modules ( ) ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[ERROR] - couldn't initialize game modules!\n";
	#endif
		goto uninit;
	}

	/* addresses */
	if ( !hack::game::addresses::grab_addresses ( ) ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[ERROR] - couldn't initialize memory addresses!\n";
	#endif
		goto uninit;
	}

	/* interfaces */
	if ( !hack::game::interfaces::grab_interfaces ( ) ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[ERROR] - couldn't initialize game interfaces!\n";
	#endif
		goto uninit;
	}

	/* netvars */
	if ( !hack::game::netvars::grab_netvars ( ) ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[ERROR] - couldn't dump game netvars!\n";
	#endif
		goto uninit;
	}

	/* menu */
	hack::menu::impl::init ( );

	/* hooks */
	if ( !hack::hooks::enable_hooks ( ) ) {
	#if defined(_DEBUG) || defined(LUNAR_DEV)
		std::cout << "[ERROR] - couldn't create hooks!\n";
	#endif
		goto uninit;
	}

	/* wait for uninit */
#if defined(_DEBUG) || defined(LUNAR_DEV)
	while ( !hack::menu::input::key_down ( VK_DELETE ) )
	#else
	while ( !hack::game::globals::should_uninject )
	#endif
		std::this_thread::sleep_for ( std::chrono::seconds ( 1 ) );

uninit:
	/* unhook */
	hack::hooks::disable_hooks ( );

	std::this_thread::sleep_for ( std::chrono::seconds ( 1 ) );

	/* close debug console */
	close_debug_console ( );

	std::this_thread::sleep_for ( std::chrono::seconds ( 1 ) );

	FreeLibraryAndExitThread ( hack::game::globals::this_instance , 0 );

	/* return */
	return 0;
}

int __stdcall DllMain ( HMODULE this_module , unsigned long reason_for_call , void * reserved ) {
	switch ( reason_for_call ) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls ( this_module );
			if ( auto handle = CreateThread ( nullptr , 0 , ( LPTHREAD_START_ROUTINE ) dll_setup , this_module , 0 , nullptr ) )
				CloseHandle ( handle );
			break;
	}

	return 1;
}