#include "../handler.hh"

bool __fastcall hack::hooks::shouldinterpolate_hk ( x86reg ) {
	if ( !ecx )
		return originals::o_shouldinterpolate ( x86regout );

	const auto entity = reinterpret_cast< sdk::c_player * >( ecx );

	if ( entity->index ( ) < 1 && entity->index ( ) > 64 )
		return originals::o_shouldinterpolate ( x86regout );

	return false;
}