#include "weapon.hh"

#include "../hack/game/interfaces.hh"

sdk::c_weaponinfo * sdk::c_weapon::data ( ) {
	if ( !this )
		return nullptr;

	static auto dataidx = 460;
	return shared::memory::v_func< sdk::c_weaponinfo * ( __thiscall * )( void * ) > ( this , dataidx )( this );
}

/* xref: "Inaccuracy =\t%f\tSpread =\t%f\tSpreadDistance =\t%f\tPlayer Velocity =\t%f\n" */
float sdk::c_weapon::inaccuracy ( ) {
	static auto getinaccuracy_idx = 482;
	return shared::memory::v_func< float ( __thiscall * )( void * ) > ( this , getinaccuracy_idx )( this );
}

float sdk::c_weapon::spread ( ) {
	static auto getspread_idx = 452;
	return shared::memory::v_func< float ( __thiscall * )( void * ) > ( this , getspread_idx )( this );
}

void sdk::c_weapon::update_accuracy ( ) {
	static auto updateaccuracy_idx = 483;
	shared::memory::v_func< void ( __thiscall * )( void * ) > ( this , updateaccuracy_idx )( this );
}
