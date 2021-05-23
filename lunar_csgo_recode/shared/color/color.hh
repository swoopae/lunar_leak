#pragma once
#include <d3dx9.h>

class c_color {
public:
	int r , g , b , a;

	constexpr c_color ( ) : r ( 255 ) , g ( 255 ) , b ( 255 ) , a ( 255 ) { }
	constexpr c_color ( int r_ , int g_ , int b_ , int a_ = 255 ) : r ( r_ ) , g ( g_ ) , b ( b_ ) , a ( a_ ) { }

	static float hue_to_rgb ( float v1 , float v2 , float v_h ) {
		if ( v_h < 0 )
			v_h += 1;

		if ( v_h > 1 )
			v_h -= 1;

		if ( ( 6 * v_h ) < 1 )
			return ( v1 + ( v2 - v1 ) * 6 * v_h );

		if ( ( 2 * v_h ) < 1 )
			return v2;

		if ( ( 3 * v_h ) < 2 )
			return ( v1 + ( v2 - v1 ) * ( ( 2.0f / 3 ) - v_h ) * 6 );

		return v1;
	}

	static std::uint32_t to_d3d ( const c_color & col ) {
		return D3DCOLOR_RGBA ( col.r , col.g , col.b , col.a );
	}

	static c_color from_hsv ( float hue , float sat , float l ) {
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;

		if ( sat == 0 ) {
			r = g = b = ( unsigned char ) ( l * 255 );
		} else {
			float v1 , v2;

			float huexd = ( float ) hue / 360;
			while ( huexd > 1.1f )
				huexd = ( float ) hue / 360;

			v2 = ( l < 0.5 ) ? ( l * ( 1 + sat ) ) : ( ( l + sat ) - ( l * sat ) );
			v1 = 2 * l - v2;

			r = ( unsigned char ) ( 255 * hue_to_rgb ( v1 , v2 , huexd + ( 1.0f / 3 ) ) );
			g = ( unsigned char ) ( 255 * hue_to_rgb ( v1 , v2 , huexd ) );
			b = ( unsigned char ) ( 255 * hue_to_rgb ( v1 , v2 , huexd - ( 1.0f / 3 ) ) );
		}

		return c_color ( r , g , b , 255.f );
	}
};