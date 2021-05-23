#include "../handler.hh"

void __fastcall hack::hooks::standardblendingrules_hk ( x86reg , int a2 , int a3 , int a4 , int a5 , int a6 ) {
	/* sanity check */
	if ( !ecx )
		return originals::o_standardblendingrules ( x86regout , a2 , a3 , a4 , a5 , a6 );

	const auto entity = reinterpret_cast< sdk::c_player * >( ecx );

	/* force disable interpolation for all entities */
	/* call standardblendingrules with EF_NOINTERP */
	*( std::uint32_t * ) ( ( std::uintptr_t ) entity + 0xF0 ) |= 8;

	originals::o_standardblendingrules ( x86regout , a2 , a3 , a4 , a5 , a6 );

	*( std::uint32_t * ) ( ( std::uintptr_t ) entity + 0xF0 ) &= ~8;
}