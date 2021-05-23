#include "../handler.hh"

#include "../../modules/animations/animations.hh"

void __fastcall hack::hooks::updateclientsideanimations_hk ( x86reg ) {
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return originals::o_updateclientsideanimations ( x86regout );

	if ( !hack::game::globals::local || !hack::game::globals::local->valid ( ) || !hack::game::globals::cmd )
		return originals::o_updateclientsideanimations ( x86regout );

	/* not interested in teammates so let them update as normal */
	if ( reinterpret_cast< sdk::c_player * >( ecx )->teammate ( ) )
		return originals::o_updateclientsideanimations ( x86regout );

	/* special for local */
	if ( ecx == hack::game::globals::local && !hack::modules::animations::in_local_animfix )
		return;
	
	/* only run update if we allow it */
	if ( !hack::modules::animations::allow_clientside_anims )
		return;

	originals::o_updateclientsideanimations ( x86regout );
}