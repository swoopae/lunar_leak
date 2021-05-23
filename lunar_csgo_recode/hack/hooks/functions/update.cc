#include "../handler.hh"

#include "../../modules/animations/animations.hh"

void __vectorcall hack::hooks::update_hk ( x86reg , float a1 , float a2 , float a3 , void * a4 ) {
	if ( !ecx )
		return;

	if ( hack::game::globals::local && ecx == hack::game::globals::local->animstate ( ) && !hack::modules::animations::in_local_animfix )
		return;

	if ( hack::modules::animations::allow_clientside_anims )
		originals::o_update ( x86regout , a1 , a2 , a3 , a4 );
}