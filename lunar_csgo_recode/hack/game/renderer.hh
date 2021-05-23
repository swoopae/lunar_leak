#pragma once

#ifndef LUNAR_RENDERER
#define LUNAR_RENDERER

#include "interfaces.hh"

#include "../../external/textures/textures.hh"
#include "../../external/fonts/cs.hh"

#include <d3d9.h>
#include <d3dx9core.h>

namespace hack::game::renderer {
	inline IDirect3DDevice9 * device;
	inline IDirect3DVertexBuffer9 * vertex_buffer;
	inline IDirect3DIndexBuffer9 * index_buffer;

	inline bool init = false;

	struct vertex_t {
		float m_x , m_y , m_z , m_rhw;
		std::uint32_t m_color;
	};

	struct texture_wrapper_t {
		IDirect3DTexture9 * m_tex;
		ID3DXSprite * m_sprite;
	};

	namespace fonts {
		inline ID3DXFont * menu_font;
		inline ID3DXFont * menu_title_font;
		inline ID3DXFont * menu_bold_font;
		inline ID3DXFont * esp_font;
		inline ID3DXFont * icon_font;
	}

	namespace texture_wrappers {
		inline texture_wrapper_t hue_bar_texture_wrapper;
		inline texture_wrapper_t transparency_bar_texture_wrapper;
	}

	static void reset ( ) {
		if ( fonts::menu_font )
			fonts::menu_font->Release ( );

		if ( fonts::menu_bold_font )
			fonts::menu_bold_font->Release ( );

		if ( fonts::menu_title_font )
			fonts::menu_title_font->Release ( );

		if ( fonts::esp_font )
			fonts::esp_font->Release ( );

		if ( fonts::icon_font )
			fonts::icon_font->Release ( );

		if ( texture_wrappers::hue_bar_texture_wrapper.m_sprite )
			texture_wrappers::hue_bar_texture_wrapper.m_sprite->Release ( );

		if ( texture_wrappers::hue_bar_texture_wrapper.m_tex )
			texture_wrappers::hue_bar_texture_wrapper.m_tex->Release ( );

		if ( texture_wrappers::transparency_bar_texture_wrapper.m_sprite )
			texture_wrappers::transparency_bar_texture_wrapper.m_sprite->Release ( );

		if ( texture_wrappers::transparency_bar_texture_wrapper.m_tex )
			texture_wrappers::transparency_bar_texture_wrapper.m_tex->Release ( );
	}

	static bool do_init ( IDirect3DDevice9 * device ) {
		if ( !device )
			return false;

		hack::game::renderer::device = device;

		D3DXCreateFontA ( hack::game::renderer::device , 15 , 0 , FW_LIGHT , 1 , false , ANSI_CHARSET , OUT_DEFAULT_PRECIS , CLEARTYPE_NATURAL_QUALITY , DEFAULT_PITCH , STR ( "Segoe UI Semilight" ) , &fonts::menu_font );
		if ( !fonts::menu_font )
			return false;

		D3DXCreateFontA ( hack::game::renderer::device , 17 , 0 , FW_NORMAL , 1 , false , ANSI_CHARSET , OUT_DEFAULT_PRECIS , CLEARTYPE_NATURAL_QUALITY , DEFAULT_PITCH , STR ( "Segoe UI" ) , &fonts::menu_bold_font );
		if ( !fonts::menu_bold_font )
			return false;

		D3DXCreateFontA ( hack::game::renderer::device , 36 , 0 , FW_NORMAL , 1 , false , ANSI_CHARSET , OUT_DEFAULT_PRECIS , CLEARTYPE_NATURAL_QUALITY , DEFAULT_PITCH , STR ( "Segoe UI" ) , &fonts::menu_title_font );
		if ( !fonts::menu_title_font )
			return false;

		D3DXCreateFontA ( hack::game::renderer::device , 12 , 0 , FW_NORMAL , 1 , false , ANSI_CHARSET , OUT_DEFAULT_PRECIS , CLEARTYPE_NATURAL_QUALITY , DEFAULT_PITCH , STR ( "Verdana" ) , &fonts::esp_font );
		if ( !fonts::esp_font )
			return false;

		D3DXCreateFontA ( hack::game::renderer::device , 23 , 0 , FW_LIGHT , 1 , false , ANSI_CHARSET , OUT_DEFAULT_PRECIS , CLEARTYPE_NATURAL_QUALITY , DEFAULT_PITCH , STR ( "Counter-Strike" ) , &fonts::icon_font );
		if ( !fonts::icon_font )
			return false;

		D3DXCreateTextureFromFileInMemory ( hack::game::renderer::device , textures::hue_bar_texture , sizeof ( textures::hue_bar_texture ) , &texture_wrappers::hue_bar_texture_wrapper.m_tex );
		D3DXCreateSprite ( hack::game::renderer::device , &texture_wrappers::hue_bar_texture_wrapper.m_sprite );
		if ( !texture_wrappers::hue_bar_texture_wrapper.m_sprite || !texture_wrappers::hue_bar_texture_wrapper.m_tex )
			return false;

		D3DXCreateTextureFromFileInMemory ( hack::game::renderer::device , textures::transparency_loader , sizeof ( textures::transparency_loader ) , &texture_wrappers::transparency_bar_texture_wrapper.m_tex );
		D3DXCreateSprite ( hack::game::renderer::device , &texture_wrappers::transparency_bar_texture_wrapper.m_sprite );
		if ( !texture_wrappers::transparency_bar_texture_wrapper.m_sprite || !texture_wrappers::transparency_bar_texture_wrapper.m_tex )
			return false;

		return true;
	}

	static void set_states ( ) {
		D3DVIEWPORT9 scrn;
		hack::game::renderer::device->GetViewport ( &scrn );

		hack::game::renderer::device->SetVertexShader ( nullptr );
		hack::game::renderer::device->SetPixelShader ( nullptr );
		hack::game::renderer::device->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
		hack::game::renderer::device->SetRenderState ( D3DRS_LIGHTING , false );
		hack::game::renderer::device->SetRenderState ( D3DRS_FOGENABLE , false );
		hack::game::renderer::device->SetRenderState ( D3DRS_CULLMODE , D3DCULL_NONE );
		hack::game::renderer::device->SetRenderState ( D3DRS_FILLMODE , D3DFILL_SOLID );

		hack::game::renderer::device->SetRenderState ( D3DRS_ZENABLE , D3DZB_FALSE );
		hack::game::renderer::device->SetRenderState ( D3DRS_SCISSORTESTENABLE , true );
		hack::game::renderer::device->SetRenderState ( D3DRS_ZWRITEENABLE , false );
		hack::game::renderer::device->SetRenderState ( D3DRS_STENCILENABLE , false );

		hack::game::renderer::device->SetRenderState ( D3DRS_MULTISAMPLEANTIALIAS , false );
		hack::game::renderer::device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE , false );

		hack::game::renderer::device->SetRenderState ( D3DRS_ALPHABLENDENABLE , true );
		hack::game::renderer::device->SetRenderState ( D3DRS_ALPHATESTENABLE , false );
		hack::game::renderer::device->SetRenderState ( D3DRS_SEPARATEALPHABLENDENABLE , true );
		hack::game::renderer::device->SetRenderState ( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
		hack::game::renderer::device->SetRenderState ( D3DRS_SRCBLENDALPHA , D3DBLEND_INVDESTALPHA );
		hack::game::renderer::device->SetRenderState ( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
		hack::game::renderer::device->SetRenderState ( D3DRS_DESTBLENDALPHA , D3DBLEND_ONE );

		hack::game::renderer::device->SetRenderState ( D3DRS_SRGBWRITEENABLE , false );
		hack::game::renderer::device->SetRenderState ( D3DRS_COLORWRITEENABLE , D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
	}

	static void line ( const c_vector2 & start , const c_vector2 & end , const c_color & col ) {
		vertex_t vtx [ 2 ] = { { start.x, start.y, 0.0f, 1.0f, c_color::to_d3d ( col ) }, { end.x, end.y, 0.0f, 1.0f, c_color::to_d3d ( col ) } };

		hack::game::renderer::device->SetTexture ( 0 , nullptr );
		hack::game::renderer::device->DrawPrimitiveUP ( D3DPT_LINELIST , 1 , &vtx , sizeof ( vertex_t ) );
	}

	static void image ( const c_vector2 & pos , const c_vector2 & size , const texture_wrapper_t & texture , float scale = 1.0f ) {
		texture.m_sprite->Begin ( D3DXSPRITE_ALPHABLEND );

		D3DXVECTOR2 center = D3DXVECTOR2 ( size.x / 2 , size.y / 2 );
		D3DXVECTOR2 trans = D3DXVECTOR2 ( pos.x - size.x / 4 , pos.y - size.y / 4 );
		D3DXMATRIX mat;
		D3DXVECTOR2 scaling ( scale , scale );
		D3DXMatrixTransformation2D ( &mat , nullptr , 0.0f , &scaling , &center , 0.0f , &trans );

		texture.m_sprite->SetTransform ( &mat );

		texture.m_sprite->Draw ( texture.m_tex , nullptr , nullptr , nullptr , 0xFFFFFFFF );
		texture.m_sprite->End ( );
	}

	static void gradient_rectangle ( const c_vector2 & pos , const c_vector2 & size , const c_color & col , const c_color & oth , const bool vertical ) {
		const auto one = c_color::to_d3d ( col ) , two = c_color::to_d3d ( oth );
		std::uint32_t c1 , c2 , c3 , c4;

		if ( !vertical ) {
			c1 = one;
			c2 = two;
			c3 = one;
			c4 = two;
		} else {
			c1 = one;
			c2 = one;
			c3 = two;
			c4 = two;
		}

		vertex_t vert [ 4 ] = {
			{ pos.x, pos.y, 0.0f, 1.0f, c1 },
			{ pos.x + size.x, pos.y, 0.0f, 1.0f, c2 },
			{ pos.x, pos.y + size.y, 0.0f, 1.0f, c3 },
			{ pos.x + size.x, pos.y + size.y, 0.0f, 1.0f, c4 }
		};

		hack::game::renderer::device->SetTexture ( 0 , nullptr );
		hack::game::renderer::device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE , false );
		hack::game::renderer::device->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP , 2 , &vert , sizeof ( vertex_t ) );
		hack::game::renderer::device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE , true );
	}

	static void filled_rectangle ( const c_vector2 & pos , const c_vector2 & size , const c_color & col ) {
		vertex_t vtx [ 4 ] = { { pos.x, pos.y + size.y , 0.0f, 1.0f, c_color::to_d3d ( col ) }, { pos.x, pos.y, 0.0f, 1.0f, c_color::to_d3d ( col ) }, { pos.x + size.x, pos.y + size.y, 0.0f, 1.0f, c_color::to_d3d ( col ) }, { pos.x + size.x, pos.y, 0.0f, 1.0f, c_color::to_d3d ( col ) } };

		hack::game::renderer::device->SetTexture ( 0 , nullptr );
		hack::game::renderer::device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE , false );
		hack::game::renderer::device->DrawPrimitiveUP ( D3DPT_TRIANGLESTRIP , 2 , vtx , sizeof ( vertex_t ) );
		hack::game::renderer::device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE , true );
	}

	static void filled_circle ( const c_vector2 & pos , int radius , int segments , const c_color & col ) {
		vertex_t * vtx = new vertex_t [ segments + 1 ];

		for ( int i = 0; i <= segments; i++ )
			vtx [ i ] = { pos.x + radius * std::cosf ( D3DX_PI * ( i / ( segments / 2.0f ) ) ), pos.y + radius * std::sinf ( D3DX_PI * ( i / ( segments / 2.0f ) ) ), 0.0f, 1.0f, c_color::to_d3d ( col ) };

		hack::game::renderer::device->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
		hack::game::renderer::device->DrawPrimitiveUP ( D3DPT_TRIANGLEFAN , segments , vtx , sizeof ( vertex_t ) );

		delete [ ] vtx;
	}

	static void rectangle ( const c_vector2 & pos , const c_vector2 & size , const c_color & col ) {
		filled_rectangle ( c_vector2 ( pos.x , pos.y ) , c_vector2 ( size.x , 1 ) , col );
		filled_rectangle ( c_vector2 ( pos.x , pos.y ) , c_vector2 ( 1 , size.y ) , col );
		filled_rectangle ( c_vector2 ( pos.x + size.x - 1 , pos.y ) , c_vector2 ( 1 , size.y ) , col );
		filled_rectangle ( c_vector2 ( pos.x , pos.y + size.y - 1 ) , c_vector2 ( size.x , 1 ) , col );
	}

	static void dispatch_vertex_list ( vertex_t* vertex_list , int size , D3DPRIMITIVETYPE primitive ) {
		hack::game::renderer::device->DrawPrimitiveUP ( primitive , size , vertex_list , sizeof ( vertex_t ) );
	}

	static c_vector2 text_position ( const std::string & txt , ID3DXFont * font ) {
		RECT rect = RECT ( );
		font->DrawTextA ( nullptr , txt.data ( ) , txt.length ( ) , &rect , DT_CALCRECT , D3DCOLOR_RGBA ( 0 , 0 , 0 , 0 ) );

		c_vector2 ret ( rect.right - rect.left , rect.bottom - rect.top );
		return ret;
	}

	static void text ( const c_vector2 & pos , const c_color & col , const std::string & txt , ID3DXFont * font , bool centered = false ) {
		RECT rect = RECT ( );

		SetRect ( &rect , pos.x , pos.y , pos.x , pos.y );
		font->DrawTextA ( nullptr , txt.data ( ) , -1 , &rect , centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP , c_color::to_d3d ( col ) );
	}

	static D3DVIEWPORT9 get_viewport ( ) {
		D3DVIEWPORT9 vp;
		hack::game::renderer::device->GetViewport ( &vp );
		return vp;
	}

	static void set_viewport ( D3DVIEWPORT9 vp ) {
		hack::game::renderer::device->SetViewport ( &vp );
	}

	/* easy to understand wrappers */
	static D3DVIEWPORT9 start_clipping ( const c_vector2 & pos , const c_vector2 & size ) {
		const auto old_vp = get_viewport ( ) , new_vp = D3DVIEWPORT9 { unsigned long ( pos.x ) , unsigned long ( pos.y ) , unsigned long ( size.x ) , unsigned long ( size.y ) };
		set_viewport ( new_vp );
		return old_vp;
	}

	static void end_clipping ( D3DVIEWPORT9 vp ) {
		set_viewport ( vp );
	}

	/* source specific */
	__forceinline static bool screen_transform ( c_vector3 & screen , c_vector3 & origin ) {
		static auto view_matrix = *reinterpret_cast< std::uintptr_t * >( shared::memory::get_signature ( GRAB_MODULE ( MODULE_CLIENT_PANORAMA ) , STR ( "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) ) + 3 ) + 176;

		if ( !view_matrix )
			return false;

		const auto & world_matrix = *reinterpret_cast< v_matrix * >( view_matrix );

		screen.x = world_matrix.m [ 0 ][ 0 ] * origin.x + world_matrix.m [ 0 ][ 1 ] * origin.y + world_matrix.m [ 0 ][ 2 ] * origin.z + world_matrix.m [ 0 ][ 3 ];
		screen.y = world_matrix.m [ 1 ][ 0 ] * origin.x + world_matrix.m [ 1 ][ 1 ] * origin.y + world_matrix.m [ 1 ][ 2 ] * origin.z + world_matrix.m [ 1 ][ 3 ];
		screen.z = 0;

		const auto w = world_matrix.m [ 3 ][ 0 ] * origin.x + world_matrix.m [ 3 ][ 1 ] * origin.y + world_matrix.m [ 3 ][ 2 ] * origin.z + world_matrix.m [ 3 ][ 3 ];

		if ( w < 0.001f ) {
			screen.x *= 100000;
			screen.y *= 100000;

			return false;
		}

		screen.x *= 1.f / w;
		screen.y *= 1.f / w;

		return true;
	}

	static bool world_to_screen ( c_vector3 & origin , c_vector3 & screen ) {
		if ( !screen_transform ( screen , origin ) )
			return false;

		int width , height;
		hack::game::interfaces::engine->get_screen_size ( width , height );

		screen.x = ( width * 0.5f ) + ( screen.x * width ) * 0.5f;
		screen.y = ( height * 0.5f ) - ( screen.y * height ) * 0.5f;

		return true;
	}
}

#endif /* LUNAR_RENDERER */