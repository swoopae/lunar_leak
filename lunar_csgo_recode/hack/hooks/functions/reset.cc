#include "../handler.hh"

#include <intrin.h>
#include "../../game/renderer.hh"

long __fastcall hack::hooks::reset_hk ( x86reg , IDirect3DDevice9 * device , D3DPRESENT_PARAMETERS * presentation_parameters ) {
	if ( !hack::game::renderer::init )
		return originals::o_reset ( x86regout , device , presentation_parameters );

	hack::game::renderer::reset ( );

	const auto orig = originals::o_reset ( x86regout , device , presentation_parameters );

	hack::game::renderer::do_init ( device );

	return orig;
}