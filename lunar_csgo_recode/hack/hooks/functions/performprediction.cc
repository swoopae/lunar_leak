#include "../handler.hh"

void __fastcall hack::hooks::performprediction_hk ( x86reg , int slot , sdk::c_player * local , bool received_new_world_update , int incoming_acknowledged , int outgoing_command ) {
// 	std::cout << "CPrediction::PerformPrediction - running on command " << outgoing_command << '\n';
	originals::o_performprediction ( x86regout , slot , local , received_new_world_update , incoming_acknowledged , outgoing_command );
}