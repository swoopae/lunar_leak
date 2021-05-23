#include "../handler.hh"

#include "../../menu/framework.hh"
#include "../../modules/misc/others.hh"

void __fastcall hack::hooks::painttraverse_hk ( x86reg , std::uint32_t vguipanel , bool repaint , bool force ) {
	originals::o_painttraverse ( x86regout , vguipanel , repaint , force );
}