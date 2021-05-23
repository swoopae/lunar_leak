#include "../handler.hh"

#include "../../game/globals.hh"

#include <intrin.h>

bool __fastcall hack::hooks::svcheatsgetbool_hk ( x86reg ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) || !hack::game::globals::local )
		return originals::o_svcheatsgetbool ( x86regout );

	static const auto cam_think = sig ( MODULE_CLIENT_PANORAMA , STR ( "85 C0 75 30 38 86" ) );

	if ( _ReturnAddress ( ) == reinterpret_cast< void * >( cam_think ) )
		return true;

	return originals::o_svcheatsgetbool ( x86regout );
}