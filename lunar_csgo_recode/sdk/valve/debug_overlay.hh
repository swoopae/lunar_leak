#pragma once

#include "../math/vector.hh"

#include "../../shared/memory/memory.hh"
#include "../../shared/color/color.hh"

namespace sdk::valve {
	class c_debug_overlay {
	public:
		void add_line_overlay ( c_vector3 & origin , c_vector3 & dest , c_color colour , float duration ) {
			using fn = void ( __thiscall * )( void * , c_vector3 & , c_vector3 & , int , int , int , bool , float );
			shared::memory::v_func< fn > ( this , 5 )( this , origin , dest , colour.r , colour.g , colour.b , false , duration );
		}

		int screen_position ( const c_vector3 & point , c_vector3 & screen ) {
			using fn = int ( __thiscall * )( void * , const c_vector3 & , c_vector3 & );
			return shared::memory::v_func< fn > ( this , 13 )( this , point , screen );
		}
	};
}