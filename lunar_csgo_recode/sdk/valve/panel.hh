#pragma once

#include "../../shared/memory/memory.hh"

namespace sdk::valve {
	class c_panel {
	public:
		const char * get_name ( unsigned int vgui_panel ) {
			using get_name_fn = const char * ( __thiscall * )( void * , unsigned int );
			return shared::memory::v_func< get_name_fn > ( this , 36 )( this , vgui_panel );
		}
	};
}