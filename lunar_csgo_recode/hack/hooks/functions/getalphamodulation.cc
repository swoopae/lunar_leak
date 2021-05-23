#include "../handler.hh"

#include "../../menu/config.hh"

float __fastcall hack::hooks::getalphamodulation_hk ( x86reg ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_connected ( ) || !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::globals::local )
		return originals::o_getalphamodulation ( x86regout );

	const auto & fx_world_modulation = CONFIG_GET ( bool , fnv1a ( "fx_world_modulation" ) );

	if ( !fx_world_modulation )
		return originals::o_getalphamodulation ( x86regout );

	const auto mat = reinterpret_cast< sdk::valve::material_t * >( ecx );

	if ( !mat )
		return originals::o_getalphamodulation ( x86regout );

	const auto group = fnv1a_rt ( mat->get_texture_group_name ( ) );

	if ( group != fnv1a ( "StaticProp textures" ) )
		return originals::o_getalphamodulation ( x86regout );

	const auto & fx_world_modulation_color = CONFIG_GET ( c_color , fnv1a ( "fx_world_modulation_color" ) );

	return ( fx_world_modulation_color.a / 255.f > 0.96f ) ? 1.f : fx_world_modulation_color.a / 255.f;
}