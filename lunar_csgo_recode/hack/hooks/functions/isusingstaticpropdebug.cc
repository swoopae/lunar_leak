#include "../handler.hh"

#include "../../menu/config.hh"

bool __fastcall hack::hooks::isusingstaticpropdebug_hk ( x86reg ) {
	const auto & fx_world_modulation = CONFIG_GET ( bool , fnv1a ( "fx_world_modulation" ) );

	return fx_world_modulation;
}