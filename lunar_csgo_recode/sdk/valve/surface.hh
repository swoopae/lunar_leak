#pragma once

#include "../math/vector.hh"

#include "../../shared/color/color.hh"
#include "../../shared/memory/memory.hh"

namespace sdk::valve {
	namespace vgui {
		typedef unsigned long hfont;
		typedef unsigned int vpanel;
	};

	enum font_feature {
		font_feature_antialiased_fonts = 1 , font_feature_dropshadow_fonts = 2 , font_feature_outline_fonts = 6
	};

	enum font_draw_type {
		font_draw_default = 0 , font_draw_nonadditive , font_draw_additive , font_draw_type_count = 2
	};

	enum font_flags {
		fontflag_none ,
		fontflag_italic = 0x001 ,
		fontflag_underline = 0x002 ,
		fontflag_strikeout = 0x004 ,
		fontflag_symbol = 0x008 ,
		fontflag_antialias = 0x010 ,
		fontflag_gaussianblur = 0x020 ,
		fontflag_rotary = 0x040 ,
		fontflag_dropshadow = 0x080 ,
		fontflag_additive = 0x100 ,
		fontflag_outline = 0x200 ,
		fontflag_custom = 0x400 ,
		fontflag_bitmap = 0x800
	};

	struct vert_t {
		vert_t ( ) = default;

		vert_t ( const c_vector2 & pos , const c_vector2 & coord = c_vector2 ( 0 , 0 ) )
			: m_pos ( pos ) , m_coord ( coord ) { };

		void init ( const c_vector2 & pos , const c_vector2 & coord = c_vector2 ( 0 , 0 ) ) {
			m_pos = pos;
			m_coord = coord;
		};

		c_vector2 m_pos;
		c_vector2 m_coord;
	};

	class c_surface {
	public:
		void draw_set_color ( c_color col ) {
			using fn = void ( __thiscall * )( void * , int , int , int , int );
			shared::memory::v_func<fn> ( this , 15 )( this , col.r , col.g , col.b , col.a );
		}

		void draw_filled_rect ( int x , int y , int w , int h ) {
			using fn = void ( __thiscall * )( void * , int , int , int , int );
			shared::memory::v_func<fn> ( this , 16 )( this , x , y , x + w , y + h );
		}

		void draw_outlined_rect ( int x , int y , int w , int h ) {
			using fn = void ( __thiscall * )( void * , int , int , int , int );
			shared::memory::v_func<fn> ( this , 18 )( this , x , y , x + w , y + h );
		}

		void draw_line ( int x , int y , int x1 , int y1 ) {
			using fn = void ( __thiscall * )( void * , int , int , int , int );
			shared::memory::v_func<fn> ( this , 19 )( this , x , y , x1 , y1 );
		}

		void draw_set_texture ( int id ) {
			using fn = void ( __thiscall * )( void * , int );
			shared::memory::v_func<fn> ( this , 38 )( this , id );
		}

		bool is_texture_id_valid ( int id ) {
			using fn = bool ( __thiscall * )( void * , int );
			return shared::memory::v_func<fn> ( this , 42 )( this , id );
		}

		int create_new_texture_id ( bool procedural = false ) {
			using fn = int ( __thiscall * )( void * , bool );
			return shared::memory::v_func<fn> ( this , 43 )( this , procedural );
		}

		void unlock_cursor ( ) {
			using fn = void ( __thiscall * )( void * );
			shared::memory::v_func<fn> ( this , 66 )( this );
		}

		void lock_cursor ( ) {
			using fn = void ( __thiscall * )( void * );
			shared::memory::v_func<fn> ( this , 67 )( this );
		}

		vgui::hfont create_font ( ) {
			using fn = vgui::hfont ( __thiscall * )( void * );
			return shared::memory::v_func<fn> ( this , 71 )( this );
		}

		bool set_font_glyph ( vgui::hfont font , const char * name , int tall , int weight , int blur , int scanlines , int flags , int range_minimum = 0 , int range_maximum = 0 ) {
			using fn = bool ( __thiscall * )( void * , vgui::hfont , const char * , int , int , int , int , int , int , int );
			return shared::memory::v_func<fn> ( this , 72 )( this , font , name , tall , weight , blur , scanlines , flags , range_minimum , range_maximum );
		}

		void get_text_size ( vgui::hfont font , const wchar_t * text , int & wide , int & tall ) {
			using fn = void ( __thiscall * )( void * , vgui::hfont , const wchar_t * , int & , int & );
			shared::memory::v_func<fn> ( this , 79 )( this , font , text , wide , tall );
		}

		void play_sound ( const char * sound ) {
			using fn = void ( __thiscall * )( void * , const char * );
			shared::memory::v_func<fn> ( this , 82 )( this , sound );
		}

		void draw_textured_polygon ( int count , vert_t * verts , bool clip = true ) {
			using fn = void ( __thiscall * )( void * , int , vert_t * , bool );
			shared::memory::v_func<fn> ( this , 106 )( this , count , verts , clip );
		}

		void draw_fade_rectangle ( int x , int y , int w , int h , std::uint32_t alpha0 , std::uint32_t alpha1 , bool horizontal ) {
			using fn = void ( __thiscall * )( void * , int , int , int , int , std::uint32_t , std::uint32_t , bool );
			shared::memory::v_func< fn > ( this , 123 )( this , x , y , x + w , y + h , alpha0 , alpha1 , horizontal );
		}

		void get_clip_rectangle ( int & x , int & y , int & x1 , int & y1 ) {
			using fn = void ( __thiscall * )( void * , int & , int & , int & , int & );
			shared::memory::v_func<fn> ( this , 146 )( this , x , y , x1 , y1 );
		}

		void set_clip_rectangle ( int x , int y , int x1 , int y1 ) {
			using fn = void ( __thiscall * )( void * , int , int , int , int );
			shared::memory::v_func<fn> ( this , 147 )( this , x , y , x1 , y1 );
		}

		void draw_circle ( int x , int y , float radius , int r , int g , int b , int a ) {
			using fn = void ( __thiscall * )( void * , int , int , float , int , int , int , int );
			shared::memory::v_func<fn> ( this , 163 )( this , x , y , radius , r , g , b , a );
		}

		void draw_text ( vgui::hfont font , int x , int y , int r , int g , int b , int a , const char * txt ) {
			using fn = void ( __thiscall * )( void * , vgui::hfont , int , int , int , int , int , int , const char * , const char * );
			shared::memory::v_func<fn> ( this , 164 )( this , font , x , y , r , g , b , a , txt , nullptr );
		}
	};
}