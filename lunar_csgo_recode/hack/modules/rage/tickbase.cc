#include "tickbase.hh"

#include "../../hooks/handler.hh"
#include "../../menu/framework.hh"

#include "../../../sdk/math/mathutils.hh"
#include "antiaim.hh"

/* don't ask.. ~swoopae */
void write_cmd ( void * buf , sdk::valve::ucmd_t * in , sdk::valve::ucmd_t * out ) {
	static auto fn = sig ( MODULE_CLIENT_PANORAMA , STR ( "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" ) );

	__asm
	{
		mov     ecx , buf
		mov     edx , in
		push	out
		call    fn
		add     esp , 4
	}
};

bool hack::modules::rage::tickbase::in_clmove ( ) {
	if ( !hack::game::interfaces::engine->is_connected ( ) || !hack::game::interfaces::engine->is_in_game ( ) )
		return false;

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return false;

	if ( ( hack::game::globals::cmd->m_tickcount <= shifted_cmd.m_tickcount + time_to_ticks ( 0.5f ) ) || shift_amount < 0 )
		return false;

	shift_amount--;

	if ( !shift_amount )
		can_we_dt_again = true;

	return true;
}

void hack::modules::rage::tickbase::in_createmove ( ) {
	const auto & r_tickbasemanip = CONFIG_GET ( bool , fnv1a ( "r_tickbasemanip" ) );
#if defined(_DEBUG) || defined(LUNAR_DEV) || defined(LUNAR_ALPHA)
	const auto & r_tickbaseshift = CONFIG_GET ( int , fnv1a ( "r_tickbaseshift" ) );
	const auto & r_overridetickbasedefault = CONFIG_GET ( bool , fnv1a ( "r_overridetickbasedefault" ) );
#endif

	static auto last_state = !r_tickbasemanip || !tickbase_key_state;

	if ( !hack::game::interfaces::engine->is_connected ( ) || !hack::game::interfaces::engine->is_in_game ( ) ) {
		can_we_dt_again = true;
		shifted_cmd.m_tickcount = -1;
		original_tickbase = -1;
		shift_amount = tickbase_shift = 0;
		return;
	}

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) ) {
		can_we_dt_again = true;
		shifted_cmd.m_tickcount = -1;
		original_tickbase = -1;
		shift_amount = tickbase_shift = 0;
		return;
	}

	/* no shifting on this sir */
	if ( hack::game::globals::local->active_weapon ( )->item_definition_index ( ) == sdk::e_item_definition_index::weapon_revolver ||
		 hack::game::globals::local->active_weapon ( )->weapon_kind ( ) == sdk::e_cs_weapon_type::weapontype_c4 ||
		 hack::game::globals::local->active_weapon ( )->weapon_kind ( ) == sdk::e_cs_weapon_type::weapontype_grenade ||
		 hack::game::globals::local->active_weapon ( )->weapon_kind ( ) == sdk::e_cs_weapon_type::weapontype_knife )
		return;

	/* hahahahahahahahahah wtf is this shit -swoopae from future */
	const static auto get_optimal_shift_value = [ & ] ( ) {
		if ( !hack::game::globals::local->active_weapon ( ) )
			return 15;

		switch ( hack::game::globals::local->active_weapon ( )->item_definition_index ( ) ) {
			case sdk::e_item_definition_index::weapon_deagle:
				return 13; break;
			case sdk::e_item_definition_index::weapon_awp:
			case sdk::e_item_definition_index::weapon_ssg08:
				return 9; break;
			case sdk::e_item_definition_index::weapon_usp_silencer:
			case sdk::e_item_definition_index::weapon_hkp2000:
			case sdk::e_item_definition_index::weapon_fiveseven:
			case sdk::e_item_definition_index::weapon_p250:
				return 10; break;
			case sdk::e_item_definition_index::weapon_cz75a:
				return 8; break;
			case sdk::e_item_definition_index::weapon_tec9:
				return 7; break;
			default:
				return 14; break;
		}

		return 14;
	};

	const auto & rage_fakeduck = CONFIG_GET ( bool , fnv1a ( "rage_fakeduck" ) );

	if ( rage_fakeduck && hack::modules::rage::antiaim::rage_fakeduck_keystate )
		return;

	if ( r_tickbasemanip && tickbase_key_state ) {
		/* recharge - always recharge 15 ticks in case we switch weapons or something */
		if ( last_state ) {
			can_we_dt_again = false;
			shift_amount = std::max ( 15 , r_tickbaseshift );
			hack::game::globals::send_packet = force_sendpacket = true;
		}

		/* shifting while choking is bad but w/e */
		if ( can_we_dt_again && shift_amount < 1 && hack::game::globals::local->can_shoot ( ) && hack::game::globals::cmd->m_buttons & sdk::valve::cmd_buttons::in_attack ) {
			can_we_dt_again = false;
			did_just_dt = true;
			std::memcpy ( &shifted_cmd , hack::game::globals::cmd , sizeof ( sdk::valve::ucmd_t ) );
			original_tickbase = hack::game::globals::local->tickbase ( );
			hack::game::globals::send_packet = force_sendpacket = true;
		#if defined(_DEBUG) || defined(LUNAR_DEV) || defined(LUNAR_ALPHA)
			if ( r_overridetickbasedefault )
				tickbase_shift = shift_amount = r_tickbaseshift;
			else tickbase_shift = shift_amount = get_optimal_shift_value ( );
		#else
			tickbase_shift = shift_amount = get_optimal_shift_value ( );
		#endif
		}
	}

	last_state = !r_tickbasemanip || !tickbase_key_state || !hack::game::globals::local->valid ( );
}

void hack::modules::rage::tickbase::in_endscene ( ) {
	const static auto keybind_func = HANDLE_KEYBIND ( fnv1a ( "r_tickbasemanip_key" ) );

	tickbase_key_state = keybind_func ( );
}

/* this code is funny -swoopae from the future */
bool hack::modules::rage::tickbase::in_writeucmddelta ( void * ecx , void * edx , int slot , void * buf , int from , int to , bool newcmd ) {
	if ( !hack::game::interfaces::engine->is_connected ( ) || !hack::game::interfaces::engine->is_in_game ( ) )
		return hack::hooks::originals::o_writeucmddelta ( x86regout , slot , buf , from , to , newcmd );

	if ( !hack::game::globals::local || !hack::game::globals::cmd || !hack::game::globals::local->alive ( ) )
		return hack::hooks::originals::o_writeucmddelta ( x86regout , slot , buf , from , to , newcmd );

	const auto & r_tickbasemanip = CONFIG_GET ( bool , fnv1a ( "r_tickbasemanip" ) );

	if ( !r_tickbasemanip || !tickbase_key_state || !tickbase_shift )
		return hack::hooks::originals::o_writeucmddelta ( x86regout , slot , buf , from , to , newcmd );

	if ( from != -1 )
		return true;

	auto backup_cmds = reinterpret_cast< int * >( std::uintptr_t ( buf ) - 48 ) , new_cmds = reinterpret_cast< int * >( std::uintptr_t ( buf ) - 44 );

	auto new_commands = *new_cmds , next_cmd_number = hack::game::interfaces::clientstate->last_outgoing_command + hack::game::interfaces::clientstate->choked_commands + 1;
	auto total_new_cmds = std::clamp ( tickbase_shift , 0 , 62 );
	tickbase_shift -= total_new_cmds;

	*new_cmds = total_new_cmds;
	*backup_cmds = 0;

	for ( to = next_cmd_number - new_commands + 1; to <= next_cmd_number; to++ ) {
		if ( !hack::hooks::originals::o_writeucmddelta ( x86regout , slot , buf , from , to , true ) )
			return false;

		from = to;
	}

	auto last_real_cmd = hack::game::interfaces::input->get_usercmd ( slot , from );
	auto from_cmd = sdk::valve::ucmd_t {};

	if ( last_real_cmd )
		from_cmd = *last_real_cmd;

	auto to_cmd = from_cmd;
	to_cmd.m_cmdnum = next_cmd_number++;

	const auto & r_tickbase_teleport = CONFIG_GET ( bool , fnv1a ( "r_tickbase_teleport" ) );
	if ( !r_tickbase_teleport )
		to_cmd.m_tickcount += 200;

	for ( auto i = new_commands; i <= total_new_cmds; i++ ) {
		write_cmd ( buf , &to_cmd , &from_cmd );
		from_cmd = to_cmd;
		to_cmd.m_cmdnum++;
		to_cmd.m_tickcount++;
	}

	return true;
}
