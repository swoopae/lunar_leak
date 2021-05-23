#include "../handler.hh"

#include "../../game/globals.hh"

// idk what i was trying to do here lol -swoopae from future
void * __fastcall hack::hooks::createentity_hk ( x86reg , const char * mapname ) {
	if ( !hack::game::interfaces::class_map )
		return originals::o_createentity ( x86regout , mapname );

	hack::game::interfaces::class_map = reinterpret_cast< sdk::valve::c_classmap* >( ecx );

	return originals::o_createentity ( x86regout , mapname );
}