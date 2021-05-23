#pragma once

#ifndef LUNAR_CONFIG
#define LUNAR_CONFIG

#include "../../external/hasher/hasher.hh"
#include "../../shared/color/color.hh"

#include <unordered_map>
#include <string>
#include <memory>

/* use me to avoid namespace aids */
#define CONFIG_ADD(type,hash,val)												\
hack::variables::options->add<type> ( hash , val );

/* use me to avoid namespace aids */
#define CONFIG_GET(type,hash)													\
hack::variables::options->get<type> ( hash );

/* doesn't work */
#define CONFIG_STATIC_GET(type,hash)											\
( [ ] ( ) {																		\
const static auto val = hack::variables::options->m_options.find ( hash );		\
auto value = *( type* ) &val._Ptr->_Myval.second;								\
return &value;																	\
} ) ( )

/* macro that creates unique lambda that handles keybind shit - is unique only because of toggle option and i cannot be bothered to make a huge bool array for every keybind */
#define HANDLE_KEYBIND( x )														\
[ ] ( ) {																		\
const auto& meme = CONFIG_GET ( keybind_t , x );								\
if(!meme.m_key && meme.m_type) return false;									\
switch ( meme.m_type ) {														\
case 0:																			\
	return true;																\
	break;																		\
case 1: {																		\
	static bool toggle = false;													\
	if ( hack::menu::input::key_released ( meme.m_key ) )						\
		toggle = !toggle;														\
	return toggle;																\
	break;																		\
}																				\
case 2:																			\
	return hack::menu::input::key_down ( meme.m_key );							\
	break;																		\
}																				\
};

struct keybind_t {
	keybind_t ( int type ) { this->m_type = type; this->m_key = 0; }

	unsigned char m_key , m_type;
};

namespace hack::variables {
	enum option_type_t {
		option_bool = 0 ,
		option_int ,
		option_float ,
		option_string ,
		option_clr ,
		option_keybind
	};

	struct option_t {
	public:
		std::uint32_t m_hash;

		option_type_t m_type;

		union values_t {
			c_color m_clr;
			keybind_t m_kbind;
			int m_ioption;
			float m_foption;
			char m_stroption [ 256 ];

			values_t ( ) { }
		} values;

		option_t ( std::uint32_t hash , option_type_t type , values_t v ) {
			m_hash = hash;
			m_type = type;
			values = v;
		};
	};

	class c_options {
	public:
		std::unordered_map< std::uint32_t , option_t > m_options;
	public:
		~c_options ( ) { }

		template < typename t >
		void add ( std::uint32_t hash , t val ) {
			option_type_t type = option_bool;
			option_t::values_t values;

			std::memset ( &values , 0 , sizeof ( option_t::values_t ) );

			if ( std::is_same< const char * , t >::value )
				std::memcpy ( &values , ( void * ) *( std::uintptr_t * )( &val ) , sizeof ( values.m_stroption ) );
			else
				std::memcpy ( &values , &val , sizeof ( t ) );

			if ( std::is_same< bool , t >::value )
				type = option_bool;
			else if ( std::is_same< int , t >::value )
				type = option_int;
			else if ( std::is_same< float , t >::value )
				type = option_float;
			else if ( std::is_same< const char * , t >::value )
				type = option_string;
			else if ( std::is_same< c_color , t >::value )
				type = option_clr;
			else if ( std::is_same< keybind_t , t >::value )
				type = option_keybind;

			m_options.emplace ( hash , option_t ( hash , type , values ) );
		}
	public:
		template < typename t >
		t & get ( std::uint32_t hash ) {
			const auto val = m_options.find ( hash );

			if ( &val._Ptr->_Myval.second.values )
				return *( t * ) &val._Ptr->_Myval.second.values;

			return *( t * ) nullptr;
		}

		void save ( std::string file ) {
		}
		void load ( std::string file ) {
		}
	};

	inline std::unique_ptr< c_options > options = std::make_unique< c_options > ( );
}

#endif /* LUNAR_CONFIG */