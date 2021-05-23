#include "netvars.hh"
#include "interfaces.hh"
#include "globals.hh"

bool hack::game::netvars::grab_netvars ( ) {
#if defined(_DEBUG) || defined(LUNAR_DEV)
	std::cout << "[info] - dumping all netvars recursively...\n";
	ofs << "fartcheat name - csgo netvar dump - compile date: " << __DATE__ << '\n';
#endif
	for ( auto pclass = hack::game::interfaces::client->get_all_classes ( ); pclass; pclass = pclass->m_next ) {
		// if ( pclass->m_class_id == sdk::valve::class_id::cprecipitation )
		// 	hack::game::globals::precipitation_client_class = pclass;

		const auto table = pclass->m_recv_table;

		if ( !table )
			continue;

		dump_rec ( table );
	}

#if defined(_DEBUG) || defined(LUNAR_DEV)
	std::cout << "[info] - dumped all netvars\n";
#endif

	return true;
}