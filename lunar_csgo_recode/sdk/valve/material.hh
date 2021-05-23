#pragma once

#include "../math/matrix3x4.hh"
#include "../math/vector.hh"

#include "../../shared/color/color.hh"
#include "../../shared/memory/memory.hh"

#define	material_var_debug ( 1 << 0 )
#define	material_var_no_debug_override ( 1 << 1 )
#define	material_var_no_draw ( 1 << 2 )
#define	material_var_use_in_fillrate_mode ( 1 << 3 )
#define	material_var_vertexcolor ( 1 << 4 )
#define	material_var_vertexalpha ( 1 << 5 )
#define	material_var_selfillum ( 1 << 6 )
#define	material_var_additive ( 1 << 7 )
#define	material_var_alphatest ( 1 << 8 )
#define	material_var_znearer ( 1 << 10 )
#define	material_var_model ( 1 << 11 )
#define	material_var_flat ( 1 << 12 )
#define	material_var_nocull ( 1 << 13 )
#define	material_var_nofog ( 1 << 14 )
#define	material_var_ignorez ( 1 << 15 )
#define	material_var_decal ( 1 << 16 )
#define	material_var_envmapsphere ( 1 << 17 )
#define	material_var_envmapcameraspace ( 1 << 19 )
#define	material_var_basealphaenvmapmask ( 1 << 20 )
#define	material_var_translucent ( 1 << 21 )
#define	material_var_normalmapalphaenvmapmask ( 1 << 22 )
#define	material_var_opaquetexture ( 1 << 24 )
#define	material_var_suppress_decals ( 1 << 26 )
#define	material_var_halflambert ( 1 << 27 )
#define	material_var_wireframe ( 1 << 28 )
#define	material_var_allowalphatocoverage ( 1 << 29 )
#define	material_var_alpha_modified_by_proxy ( 1 << 30 )
#define	material_var_vertexfog ( 1 << 31 )

namespace sdk::valve {
	class material_t {
	public:
		const char * get_name ( void ) {
			using getname_fn = const char * ( __thiscall * )( void * );
			return shared::memory::v_func< getname_fn > ( this , 0 )( this );
		}

		const char * get_texture_group_name ( void ) {
			using gettexturegroupname_fn = const char * ( __thiscall * )( void * );
			return shared::memory::v_func< gettexturegroupname_fn > ( this , 1 )( this );
		}

		void increment_reference_count ( void ) {
			using increferencecount_fn = void ( __thiscall * )( void * );
			shared::memory::v_func< increferencecount_fn > ( this , 12 )( this );
		}

		void * find_var ( const char * var , bool * found , bool complain = true ) {
			using findvar_fn = void * ( __thiscall * )( void * , const char * , bool * , bool );
			return shared::memory::v_func< findvar_fn > ( this , 11 )( this , var , found , complain );
		}

		void color_modulate ( c_color clr ) {
			using colormodulate_fn = void ( __thiscall * )( void * , float , float , float );
			shared::memory::v_func< colormodulate_fn > ( this , 28 )( this , ( float ) clr.r / 255.0f , ( float ) clr.g / 255.0f , ( float ) clr.b / 255.0f );
		}

		void set_material_var_flag ( std::uint32_t flag , bool state ) {
			using setmaterialvarflag_fn = void ( __thiscall * )( void * , std::uint32_t , bool );
			shared::memory::v_func< setmaterialvarflag_fn > ( this , 29 )( this , flag , state );
		}
	};

	class c_model_render {
	public:
		void force_mat_override ( material_t * mat ) {
			using original_fn = void ( __thiscall * )( c_model_render * , material_t * , int , int );
			shared::memory::v_func< original_fn > ( this , 1 )( this , mat , 0 , 0 );
		}
	};

	class c_materialsystem {
	public:
		material_t * create_material ( const char * name , void * kv ) {
			using creatematerial_fn = material_t * ( __thiscall * )( void * , const char * , void * );
			return shared::memory::v_func< creatematerial_fn > ( this , 83 )( this , name , kv );
		}

		material_t * find_material ( char const * material_name , const char * group_name = nullptr , bool complain = true , const char * complain_prefix = 0 ) {
			using original_fn = material_t * ( __thiscall * )( c_materialsystem * , char const * , const char * , bool , const char * );
			return shared::memory::v_func< original_fn > ( this , 84 )( this , material_name , group_name , complain , complain_prefix );
		}
		int	get_materials_count ( ) {
			using original_fn = int ( __thiscall * )( c_materialsystem * );
			return shared::memory::v_func< original_fn > ( this , 90 )( this );
		}
	};

	struct model_render_info_t {
		c_vector3 origin;
		c_vector3 angles;
		pad ( 4 );
		void * renderable;
		const void * model;
		const c_matrix3x4 * model_to_world;
		const c_matrix3x4 * lighting_offset;
		const c_vector3 * lighting_origin;
		int flags;
		int entity_index;
		int skin;
		int body;
		int hitboxset;
		unsigned short instance;

		model_render_info_t ( ) {
			model_to_world = 0;
			lighting_offset = 0;
			lighting_origin = 0;
		}
	};
}