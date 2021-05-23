#include "engine_prediction.hh"

#include "../rage/tickbase.hh"

c_vector3 old_origin {};

void hack::modules::prediction::engine_prediction::start_prediction ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	in_custom_prediction = true;

	if ( !engine_prediction::random_prediction_seed )
		engine_prediction::random_prediction_seed = *reinterpret_cast< int ** >( sig ( MODULE_CLIENT_PANORAMA , STR ( "A3 ? ? ? ? 66 0F 6E 86" ) ) + 1 );

	if ( !engine_prediction::set_prediction_seed )
		engine_prediction::set_prediction_seed = *reinterpret_cast< int ** >( sig ( MODULE_CLIENT_PANORAMA , STR ( "89 35 ? ? ? ? F3 0F 10 48" ) ) + 2 );

	old_origin = hack::game::globals::local->origin ( );

	// static auto unknown_fn = reinterpret_cast< void ( __thiscall * )( int , int ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 8B 55 08 3B CA" ) ) );
	// unknown_fn ( reinterpret_cast< std::uintptr_t >( hack::game::globals::local + 0x3234 ) , reinterpret_cast< std::uintptr_t >( hack::game::globals::cmd ) );

	foffset ( sdk::valve::ucmd_t * , hack::game::globals::local , 0x3314 ) = hack::game::globals::cmd;
	foffset ( sdk::valve::ucmd_t , hack::game::globals::local , 0x326C ) = *hack::game::globals::cmd;

	*engine_prediction::random_prediction_seed = hack::game::globals::cmd ? hack::game::globals::cmd->m_randseed : -1;
	*engine_prediction::set_prediction_seed = reinterpret_cast< int >( hack::game::globals::local );

	engine_prediction::old_curtime = hack::game::interfaces::global_vars->m_curtime;
	engine_prediction::old_frametime = hack::game::interfaces::global_vars->m_frametime;

	hack::game::interfaces::global_vars->m_curtime = hack::game::globals::local->tickbase ( ) * hack::game::interfaces::global_vars->m_ipt;
	hack::game::interfaces::global_vars->m_frametime = hack::game::interfaces::prediction->m_engine_paused ? 0 : hack::game::interfaces::global_vars->m_ipt;

	const auto old_in_prediction = hack::game::interfaces::prediction->m_in_prediction;
	const auto old_is_first_time_prediction = hack::game::interfaces::prediction->m_is_first_time_predicted;

	hack::game::interfaces::prediction->m_in_prediction = false;
	hack::game::interfaces::prediction->m_is_first_time_predicted = true;

	hack::game::interfaces::move_helper->set_host ( hack::game::globals::local );
	hack::game::interfaces::game_movement->start_track_prediction_errors ( hack::game::globals::local );

	/* do weapon selection here LATER */
	// CBasePlayer::SelectItem = 55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06 , vfunc index 329

	// if ( auto i = hack::game::globals::cmd->m_impulse )
	//	*reinterpret_cast< std::uintptr_t * >( reinterpret_cast< std::uintptr_t >( hack::game::globals::local ) + 0x31FC ) = i;

	/* CBasePlayer::UpdateButtonState - INLINED in RunCommand */
	/* v16 = *( _DWORD* ) ( ucmd_pointer + 48 );
	v17 = v16 ^ entity [ 3198 ];
	entity [ 3195 ] = entity [ 3198 ];
	entity [ 3198 ] = v16;
	entity [ 3196 ] = v16 & v17;
	entity [ 3197 ] = v17 & ~v16; */

	/* check if player is standing on moving ground */
	hack::game::interfaces::prediction->check_moving_ground ( hack::game::globals::local , hack::game::interfaces::global_vars->m_frametime );
	 
	// // void CPrediction::RunPreThink( C_BasePlayer *player )
	// // argument is thinkMethod but is defaulted to 0
	// if ( hack::game::globals::local->physics_run_think ( 0 ) )
	// 	hack::game::globals::local->pre_think ( );
	// 
	// auto thinktick = hack::game::globals::local->next_think_tick ( );
	// if ( thinktick != -1 && thinktick > 0 && thinktick <= hack::game::globals::local->tickbase ( ) ) {
	// 	/* inline player->SetNextThink( TICK_NEVER_THINK ); */
	// 	thinktick = -1;
	// 
	// 	/* player->CheckHasThinkFunction ( false ) */
	// 	static auto setnextthink_fn = reinterpret_cast< char ( __thiscall * )( void * , bool ) >( sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) ) );
	// 	setnextthink_fn ( hack::game::globals::local , false );
	// 
	// 	hack::game::globals::local->think ( );
	// }

	sdk::valve::movedata_t data;

	std::memset ( &data , 0 , sizeof ( sdk::valve::movedata_t ) );

	hack::game::interfaces::prediction->setup_move ( hack::game::globals::local , hack::game::globals::cmd , hack::game::interfaces::move_helper , &data );

	hack::game::interfaces::game_movement->process_movement ( hack::game::globals::local , &data );

	hack::game::interfaces::prediction->finish_move ( hack::game::globals::local , hack::game::globals::cmd , &data );

	hack::game::interfaces::move_helper->process_impacts ( );

	hack::game::globals::local->force_velocity ( data.m_vecvelocity_ );
	hack::game::globals::local->set_absolute_origin ( hack::game::globals::local->origin ( ) );

	// hack::game::globals::local->post_think_rebuilt ( );

	/* update weapon accuracy - take a peep at rifk to see what to backup and what to restore - idx: inacc + 1 */

	/* should maybe add 1 to tickbase cuz tHaTs wHaT vAlVe dOeS =) */

	hack::game::interfaces::prediction->m_in_prediction = old_in_prediction;
	hack::game::interfaces::prediction->m_is_first_time_predicted = old_is_first_time_prediction;
}

void hack::modules::prediction::engine_prediction::end_prediction ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	hack::game::interfaces::game_movement->finish_track_prediction_errors ( hack::game::globals::local );
	hack::game::interfaces::move_helper->set_host ( 0 );

	/* directly from Valve:
	 * "fixes a crash: when loading highlights twice or after previously loaded map, there was a dirty player pointer in game movement" */
	hack::game::interfaces::game_movement->reset ( );

	/* player->m_pCurrentCommand = NULL;
	 * possibly gets set by called functions? */
	 // *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( hack::game::globals::local ) + 0x3338 ) = 0;

	*engine_prediction::random_prediction_seed = -1;
	*engine_prediction::set_prediction_seed = 0;

	hack::game::interfaces::global_vars->m_curtime = engine_prediction::old_curtime;
	hack::game::interfaces::global_vars->m_frametime = engine_prediction::old_frametime;

	hack::game::globals::local->set_absolute_origin ( old_origin );
	hack::game::globals::local->origin ( ) = old_origin;

	in_custom_prediction = false;
}

/* TODO: */
void hack::modules::prediction::engine_prediction::repredict ( ) {
}

/* unused and old and very likely incorrect... */
void hack::modules::prediction::engine_prediction::update_correct_globals ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	correct_tickbase = ( !last_command || last_command->m_hasbeenpredicted ) ? hack::game::globals::local->tickbase ( ) : correct_tickbase++;
	correct_curtime = correct_tickbase * hack::game::interfaces::global_vars->m_ipt;
	last_command = hack::game::globals::cmd;
}

void hack::modules::prediction::engine_prediction::store_netvars ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	/* this one is for you lime, i'll stop the namespace abuse just for you :) ~swoopae */
	auto & data_ref = data.at ( hack::game::globals::local->tickbase ( ) % 150 );

	/* store tickbase for checking if we should apply our record or not */
	data_ref.m_tickbase = hack::game::globals::local->tickbase ( );

	/* store our netvars */
	data_ref.m_fallvelocity = hack::game::globals::local->local_exclusive ( ).fall_velocity ( );
}

void hack::modules::prediction::engine_prediction::apply_netvars ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::interfaces::engine->is_connected ( ) )
		return;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return;

	auto & data_ref = data.at ( hack::game::globals::local->tickbase ( ) % 150 );

	/* TODO: reduntant check... am i retarded or some shit? ~swoopae */
	if ( hack::game::globals::local->tickbase ( ) != data_ref.m_tickbase )
		return;

	/* get delta for everything - this is aids */
	const auto delta_fallvelocity = hack::game::globals::local->local_exclusive ( ).fall_velocity ( ) - data_ref.m_fallvelocity;

	/* absolute aids */
	if ( std::fabsf ( delta_fallvelocity ) < 0.03125f )
		hack::game::globals::local->local_exclusive ( ).fall_velocity ( ) = data_ref.m_fallvelocity;
}

void hack::modules::prediction::engine_prediction::run_in_prediction ( std::function<void ( )> fn ) {
	start_prediction ( );
	// update_correct_globals ( );

	fn ( );

	end_prediction ( );
}