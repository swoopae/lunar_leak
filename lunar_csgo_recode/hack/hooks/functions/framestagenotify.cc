#include "../handler.hh"

#include "../../modules/animations/animations.hh"
#include "../../modules/misc/others.hh"

#include "../../menu/framework.hh"

void __fastcall hack::hooks::framestagenotify_hk ( x86reg , int stage ) {
	hack::modules::animations::in_framestagenotify ( stage );

	originals::o_framestagenotify ( x86regout , stage );

	hack::modules::misc::others::in_fsn ( stage );
}