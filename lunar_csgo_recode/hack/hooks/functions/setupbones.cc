#include "../handler.hh"

#include "../../modules/animations/animations.hh"

bool __fastcall hack::hooks::setupbones_hk ( x86reg , c_matrix3x4 * bone_to_world , int max_bones , int bone_mask , float time ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) ||
		 !hack::game::globals::local )
		return originals::o_setupbones ( x86regout , bone_to_world , max_bones , bone_mask , time );

	if ( !ecx )
		return originals::o_setupbones ( x86regout , bone_to_world , max_bones , bone_mask , time );

	if ( GetAsyncKeyState ( VK_LSHIFT ) )
		return true;

	/* grab player from iclientrenderable */
	const auto player = reinterpret_cast< sdk::c_player * >( std::uintptr_t ( ecx ) - 0x4 );

	/* player check */
	if ( !player->index ( ) || player->index ( ) > 64 )
		return originals::o_setupbones ( x86regout , bone_to_world , max_bones , bone_mask , time );

	/* if we allow setup bones , call original */
	if ( hack::modules::animations::allow_setup_bones )
		return originals::o_setupbones ( x86regout , bone_to_world , max_bones , bone_mask , time );

	/* if we're local , copy last calculated bones and return */
	if ( player == hack::game::globals::local ) {
		/* what the game does */
		if ( max_bones < player->bone_cache ( ).get_count ( ) )
			return false;
		
		if ( bone_to_world )
			std::memcpy ( bone_to_world , player->bone_cache ( ).base ( ) , sizeof ( c_matrix3x4 ) * player->bone_cache ( ).get_count ( ) );

	 	/* return */
		return true;
	}

	return originals::o_setupbones ( x86regout , bone_to_world , max_bones , bone_mask , time );
}