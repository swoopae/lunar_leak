#include "../handler.hh"

#include <intrin.h>

bool __fastcall hack::hooks::inprediction_hk ( x86reg ) {
	static const auto cbaseanimunkn_retaddr = sig ( MODULE_CLIENT_PANORAMA , STR ( "84 C0 74 17 8B 87" ) ); /* TODO: check src leak ~swoopae */
	static const auto setupbones_retaddr = sig ( MODULE_CLIENT_PANORAMA , STR ( "84 C0 74 0A F3 0F 10 05 ? ? ? ? EB 05" ) );

	const auto retaddr = reinterpret_cast< std::uintptr_t >( _ReturnAddress ( ) );

	/* return false cuz game overrides our current time with saved prediction time thanks valve */
	if ( retaddr == setupbones_retaddr )
		return false;

	/* fixes some weird ass oddity */
	if ( retaddr == cbaseanimunkn_retaddr )
		return true;

	return originals::o_inprediction ( x86regout );
}