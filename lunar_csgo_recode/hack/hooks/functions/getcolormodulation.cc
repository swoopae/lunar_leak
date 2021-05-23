#include "../handler.hh"

#include "../../menu/config.hh"

void __fastcall hack::hooks::getcolormodulation_hk ( x86reg , float * r , float * g , float * b ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_connected ( ) || !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::globals::local )
		return originals::o_getcolormodulation ( x86regout , r , g , b );

	originals::o_getcolormodulation ( x86regout , r , g , b );

	const auto material = reinterpret_cast< sdk::valve::material_t * >( ecx );

	if ( !material )
		return;

	const auto group = fnv1a_rt ( material->get_texture_group_name ( ) );

	if ( group != fnv1a ( "World textures" ) && group != fnv1a ( "StaticProp textures" ) && group != fnv1a ( "SkyBox textures" ) )
		return;

	const auto & fx_world_modulation = CONFIG_GET ( bool , fnv1a ( "fx_world_modulation" ) );

	if ( !fx_world_modulation )
		return;

	const auto & fx_world_modulation_color = CONFIG_GET ( c_color , fnv1a ( "fx_world_modulation_color" ) );

	const auto is_prop = group == fnv1a ( "StaticProp textures" );

	*r *= is_prop ? 0.5f * ( fx_world_modulation_color.r / 255.f ) : 0.23f * ( fx_world_modulation_color.r / 255.f );
	*g *= is_prop ? 0.5f * ( fx_world_modulation_color.g / 255.f ) : 0.23f * ( fx_world_modulation_color.g / 255.f );
	*b *= is_prop ? 0.5f * ( fx_world_modulation_color.b / 255.f ) : 0.23f * ( fx_world_modulation_color.b / 255.f );
}
