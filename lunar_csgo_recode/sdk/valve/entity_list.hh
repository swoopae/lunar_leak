#pragma once

#include "../../shared/memory/memory.hh"

namespace sdk::valve {
	class c_entity_list {
	public:
		template < typename t >
		t get ( int index ) {
			using fn = t ( __thiscall * )( void * , int );
			return shared::memory::v_func< fn > ( this , 3 )( this , index );
		}

		template < typename t >
		t get_by_handle ( csgo_handle handle ) {
			using fn = t ( __thiscall * )( void * , csgo_handle );
			return shared::memory::v_func< fn > ( this , 4 )( this , handle );
		}

		int entity_num ( bool include_non_networkable = false ) {
			using fn = int ( __thiscall * )( void * , bool );
			return shared::memory::v_func< fn > ( this , 5 )( this , include_non_networkable );
		}

		int highest_entity_index ( ) {
			using fn = int ( __thiscall * )( void * );
			return shared::memory::v_func< fn > ( this , 6 )( this );
		}

		int max_entities ( ) {
			using fn = int ( __thiscall * )( void * );
			return shared::memory::v_func< fn > ( this , 8 )( this );
		}
	};
}