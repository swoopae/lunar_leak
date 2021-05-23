#include "../handler.hh"

#include "../../menu/framework.hh"

void __fastcall hack::hooks::lockcursor_hk ( x86reg ) {
	if ( hack::menu::impl::menu_opened ) {
		hack::game::interfaces::surface->unlock_cursor ( );
		return;
	}

	originals::o_lockcursor ( x86regout );
}