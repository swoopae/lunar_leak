#include "../handler.hh"

#include "../../modules/animations/animations.hh"

void __fastcall hack::hooks::interpolateserverentities_hk ( x86reg ) {
	originals::o_interpolateserverentities ( x86regout );

	if ( hack::game::globals::local && hack::game::globals::local->valid ( ) && hack::game::globals::cmd ) {
		/* set proper info */
		hack::game::globals::local->set_absolute_angles ( hack::modules::animations::last_sent_absyaw );

		/* correct render bone matrix */
		/* CREDZzZz: @alpha */
		const auto angle_matrix = [ ] ( const c_vector3 & angle , const c_vector3 & position , c_matrix3x4 & out ) {
			const static auto fn = reinterpret_cast< void ( __fastcall * )( const c_vector3 & , c_matrix3x4 & ) >( shared::memory::get_func_addr_relative ( sig ( MODULE_CLIENT_PANORAMA , STR ( "E8 ? ? ? ? 83 FF 03" ) ) ) );
			fn ( angle , out );

			/* set matrix origin */
			out [ 0 ][ 3 ] = position.x;
			out [ 1 ][ 3 ] = position.y;
			out [ 2 ][ 3 ] = position.z;
		};

		/* variables */
		c_matrix3x4 new_matrix {};
		std::uint32_t bone_computed [ 8 ] = { 0 };

		angle_matrix ( hack::game::globals::local->absolute_angles ( ) , hack::game::globals::local->absolute_origin ( ) , new_matrix );

		const auto backup_matrix = hack::game::globals::local->bone_accesor ( ).m_bones;

		hack::modules::animations::allow_setup_bones = true;

		/* thx alpha 4 codenz */
		hack::game::globals::local->studio_build_matrices ( hack::game::globals::local->studio_hdr ( ) , new_matrix , hack::modules::animations::local_bones_pos , hack::modules::animations::local_bones_quat , 0x7ff00 , hack::modules::animations::local_real_bones , bone_computed );

		/* invalidate bone cache */
		hack::game::globals::local->invalidate_bone_cache ( );

		hack::game::globals::local->bone_accesor ( ).m_readable = hack::game::globals::local->bone_accesor ( ).m_writeable = 0xFFFFFFFF;
		hack::game::globals::local->bone_accesor ( ).m_bones = hack::modules::animations::local_real_bones;

		/* call attachment helper directly */
		hack::game::globals::local->attachment_helper ( );

		hack::game::globals::local->bone_accesor ( ).m_bones = backup_matrix;

		hack::modules::animations::allow_setup_bones = false;
	}
}