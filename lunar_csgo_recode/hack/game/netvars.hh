#pragma once

#include "../../sdk/valve/base_client.hh"
#include "../../external/xor/xor.hh"
#include "../../external/hasher/hasher.hh"

#include <fstream>
#include <unordered_map>

#define netvar_offset( t, func, name, off ) \
t& func( ) { \
	static const auto offset = hack::game::netvars::offsets[ fnv1a ( name ) ]; \
	return *( t* ) ( std::uintptr_t( this ) + offset + off ); \
}

#define netvar_fn( t, func, name ) \
t& func( ) { \
	static const auto offset = hack::game::netvars::offsets[ fnv1a ( name ) ]; \
	return *( t* ) ( std::uintptr_t( this ) + offset ); \
}

#define offset( t, func, offset ) \
t& func( ) { \
	return *( t* ) ( std::uintptr_t( this ) + offset ); \
}

#define poffset( t, func, offset ) \
t func( ) { \
	return ( t ) ( std::uintptr_t( this ) + offset ); \
}

#define foffset( type, ptr, offset ) \
( *( type* ) ( ( std::uintptr_t ) (ptr) + ( offset ) ) )

namespace hack::game::netvars {
#if defined(_DEBUG) || defined(LUNAR_DEV)
	inline std::ofstream ofs ( "netvars.dump" );
#endif
	inline std::unordered_map< std::uint32_t , std::uintptr_t > offsets;

	static void dump_rec ( sdk::valve::recv_table_t * table ) {
		for ( auto i = 0; i < table->m_prop_count; ++i ) {
			const auto prop = &table->m_props [ i ];

			if ( !prop
				 || std::isdigit ( prop->m_name [ 0 ] )
				 || !std::strcmp ( prop->m_name , STR ( "baseclass" ) ) )
				continue;

			if ( prop->m_recv_type == 6
				 && prop->m_table
				 && prop->m_table->m_net_table_name [ 0 ] == 'D' )
				dump_rec ( prop->m_table );

		#if defined(_DEBUG) || defined(LUNAR_DEV)
			ofs << table->m_net_table_name + std::string ( "->" ) + prop->m_name << "=" << prop->m_offset << " - hash: " << fnv1a_rt ( std::string ( table->m_net_table_name + std::string ( "->" ) + prop->m_name ).data ( ) ) << '\n';
		#endif

			offsets [ fnv1a_rt ( std::string ( table->m_net_table_name + std::string ( "->" ) + prop->m_name ).data ( ) ) ] = prop->m_offset;
		}
	}

	bool grab_netvars ( );
}