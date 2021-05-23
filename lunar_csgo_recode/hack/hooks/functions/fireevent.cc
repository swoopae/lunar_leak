#include "../handler.hh"

#include "../../game/globals.hh"

struct event_t {
public:
	pad ( 4 );
	float m_delay;
	pad ( 48 );
	event_t * m_next;
};

bool __fastcall hack::hooks::fireevent_hk ( x86reg ) {
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) || !hack::game::globals::local )
		return originals::o_fireevent ( x86regout );

	auto ei = *reinterpret_cast< event_t ** > ( reinterpret_cast< std::uintptr_t >( hack::game::interfaces::clientstate ) + 0x4E64 );

	if ( !ei )
		return originals::o_fireevent ( x86regout );

	event_t * next = nullptr;

	do {
		next = ei->m_next;
		ei->m_delay = 0.f;
		ei = next;
	} while ( next );

	return originals::o_fireevent ( x86regout );
}