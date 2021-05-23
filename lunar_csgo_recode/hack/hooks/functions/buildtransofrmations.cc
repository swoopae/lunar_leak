#include "../handler.hh"

#include "../../modules/animations/animations.hh"

void __fastcall hack::hooks::buildtransformations_hk ( x86reg , int a2 , c_vector3 * a3 , c_quaternion * a4 , int a5 , int a6 , int a7 ) {
	/* sanity check */
	if ( !ecx )
		return originals::o_buildtransformations ( x86regout , a2 , a3 , a4 , a5 , a6 , a7 );

	const auto entity = reinterpret_cast< sdk::c_player * >( ecx );

	/* player check */
	if ( entity->index ( ) > 64 )
		return originals::o_buildtransformations ( x86regout , a2 , a3 , a4 , a5 , a6 , a7 );

	/* force jiggle bones off */
	const auto backup_jiggle = *( std::uint32_t * ) ( ( std::uintptr_t ) entity + 0x292C );
	*( std::uint32_t * ) ( ( std::uintptr_t ) entity + 0x292C ) = 0;

	/* call original */
	originals::o_buildtransformations ( x86regout , a2 , a3 , a4 , a5 , a6 , a7 );

	/* restore jiggle bones */
	*( std::uint32_t * ) ( ( std::uintptr_t ) entity + 0x292C ) = backup_jiggle;

	if ( entity == hack::game::globals::local ) {
		hack::modules::animations::local_bones_pos = a3;
		hack::modules::animations::local_bones_quat = a4;
	}
}