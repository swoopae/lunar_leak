#pragma once

/* all memory stuff is tossed in here */
#include <windows.h>
#include <Psapi.h>
#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "../../external/hasher/hasher.hh"

/* macros */
#define in_range(x,a,b) (x >= a && x <= b)

#define get_bits( x ) (in_range((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (in_range(x,'0','9') ? x - '0' : 0))
#define get_byte( x ) (get_bits(x[0]) << 4 | get_bits(x[1]))

#define combine( x, y ) x##y
#define combine_two( x, y ) combine( x, y )

/* we only really use these three */
#define pad( sz ) \
	private: \
	unsigned char combine_two( pad_, __COUNTER__ )[ sz ]; \
	public:

#define sig( mod,sig ) \
	shared::memory::get_signature ( GRAB_MODULE ( mod ) , sig )

#define ref_sig( mod,s ) \
	shared::memory::get_func_addr_relative ( sig ( GRAB_MODULE ( mod ) , s ) )

#define x86reg void* ecx , void* edx
#define x86regout ecx , edx

namespace shared::memory {
	/* reference: https://www.unknowncheats.me/forum/2695607-post7.html ~swoopae */
	static std::uintptr_t get_func_addr_relative ( std::uintptr_t address ) {
		const auto relative_addy = *reinterpret_cast< std::uintptr_t * >( std::uintptr_t ( address ) + 0x1 );

		// calling address
		return std::uintptr_t ( address ) + 0x5 + relative_addy;
	}

	static std::uintptr_t get_signature ( std::uintptr_t sig_module , const std::string & signature ) {
		auto range_start = sig_module;

		MODULEINFO module_info;

		GetModuleInformation ( GetCurrentProcess ( ) , ( HMODULE ) range_start , &module_info , sizeof ( MODULEINFO ) );

		int start = ( int ) module_info.lpBaseOfDll , len = module_info.SizeOfImage;

		uint8_t * scan_start , * scan_end;
		std::vector< std::pair< uint8_t , bool > > pattern {};
		std::stringstream stream { signature };
		std::string w;

		if ( !start || !len || signature.empty ( ) )
			return{};

		while ( stream >> w ) {
			if ( w [ 0 ] == '?' ) pattern.push_back ( { 0, true } );
			else if ( std::isxdigit ( w [ 0 ] ) && std::isxdigit ( w [ 1 ] ) ) pattern.push_back ( { ( uint8_t ) std::strtoul ( w.data ( ), 0, 16 ), false } );
		}

		scan_start = reinterpret_cast< uint8_t * >( start );
		scan_end = scan_start + len;

		auto result = std::search ( scan_start , scan_end , pattern.begin ( ) , pattern.end ( ) ,
									[ ] ( const uint8_t b , const std::pair< uint8_t , bool > & p ) {
			return b == p.first || p.second;
		} );

		if ( result == scan_end )
			return{};

		return ( uintptr_t ) result;
	}

	template < typename t >
	__forceinline t v_func ( void * thisptr , std::uintptr_t v_index ) {
		if ( !thisptr ) {
		#if defined(_DEBUG) || defined(LUNAR_DEV)
			std::cout << "[ERROR] - couldn't find vfunc with index" << v_index << "\n";
			throw std::exception ( std::string ( "couldn't find vfunc with index " + std::to_string ( v_index ) ).data ( ) );
		#endif
			return *( t * )nullptr;
		}

		return t ( ( *( std::uintptr_t ** ) thisptr ) [ v_index ] );
	}

	static std::int32_t rva_2_offset ( const std::uint32_t rva , PIMAGE_NT_HEADERS nt_headers , const bool in_memory = false ) {
		if ( !rva || !in_memory )
			return rva;

		auto sec = IMAGE_FIRST_SECTION ( nt_headers );
		for ( size_t i = 0; i < nt_headers->FileHeader.NumberOfSections; i++ ) {
			if ( rva >= sec->VirtualAddress && rva < sec->VirtualAddress + sec->Misc.VirtualSize )
				break;
			sec++;
		}

		return rva - sec->VirtualAddress + sec->PointerToRawData;
	}

	static std::uintptr_t get_proc_address ( const std::uintptr_t module , const std::uint32_t function , const bool in_memory = false ) {
		const auto dos_headers = reinterpret_cast< IMAGE_DOS_HEADER * >( module );
		if ( dos_headers->e_magic != IMAGE_DOS_SIGNATURE )
			return 0;

		const auto nt_headers = reinterpret_cast< IMAGE_NT_HEADERS * >( dos_headers->e_lfanew + module );
		if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
			return 0;

		const auto exports = reinterpret_cast< IMAGE_EXPORT_DIRECTORY * >(
			rva_2_offset ( nt_headers->OptionalHeader.DataDirectory [ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress ,
			nt_headers , in_memory ) + module );

		const auto names = reinterpret_cast< std::uint32_t * >( rva_2_offset ( exports->AddressOfNames , nt_headers , in_memory ) + module );

		auto ordinal_index = static_cast< std::uint32_t >( -1 );
		for ( std::uint32_t i = 0; i < exports->NumberOfFunctions; i++ ) {
			const auto function_name = reinterpret_cast< const char * >( rva_2_offset ( names [ i ] , nt_headers , in_memory ) + module );

			if ( fnv1a_rt ( function_name ) == function ) {
				ordinal_index = i;
				break;
			}
		}

		if ( ordinal_index > exports->NumberOfFunctions )
			return 0;

		const auto ordinals = reinterpret_cast< std::uint16_t * >( rva_2_offset ( exports->AddressOfNameOrdinals , nt_headers , in_memory ) + module );
		const auto addresses = reinterpret_cast< std::uint32_t * >( rva_2_offset ( exports->AddressOfFunctions , nt_headers , in_memory ) + module );
		return rva_2_offset ( addresses [ ordinals [ ordinal_index ] ] , nt_headers , in_memory ) + module;
	}
}