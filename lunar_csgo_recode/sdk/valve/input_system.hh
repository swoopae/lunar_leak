#pragma once

#include "../../shared/memory/memory.hh"

namespace sdk::valve {
	class c_input_system {
	public:
		void * enable_input ( bool enable ) {
			return shared::memory::v_func< void * ( __thiscall * )( void * , bool ) > ( this , 11 )( this , enable );
		}
	};
}