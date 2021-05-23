#include "entity.hh"

#include "../hack/game/modules.hh"

bool sdk::c_entity::dormant ( ) {
	const static auto dormant_offset = *( std::uintptr_t * )( sig ( MODULE_CLIENT_PANORAMA , STR ( "83 79 ? FF 74 07 8A 81 ? ? ? ? C3 32 C0 C3 6A 00" ) ) + 8 ) + 8;
	return *reinterpret_cast< bool * >( std::uintptr_t ( this ) + dormant_offset );
}