#include "misc.hh"

#include "../../game/renderer.hh"

#include "../rage/tickbase.hh"
#include "../rage/antiaim.hh"
#include "../misc/movement.hh"
#include "../animations/animations.hh"

#include "../../menu/config.hh"
#include "../../menu/framework.hh"

#include <mutex>

void indicators ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::globals::local || !hack::game::globals::local->alive ( ) )
		return;

	/* grab screen bounds */
	int screen_width , screen_height;
	hack::game::interfaces::engine->get_screen_size ( screen_width , screen_height );

	/* set initial drawing position */
	screen_width /= 2;
	screen_height /= 2;

	screen_height += 25;

	/* start drawing indicators */
	const auto & r_tickbasemanip = CONFIG_GET ( bool , fnv1a ( "r_tickbasemanip" ) );
	const auto & rage_fakeduck = CONFIG_GET ( bool , fnv1a ( "rage_fakeduck" ) );
	const auto & aa_desync = CONFIG_GET ( bool , fnv1a ( "aa_desync" ) );
	const auto & m_selfpeek = CONFIG_GET ( bool , fnv1a ( "m_selfpeek" ) );

	/* initialize our indicator colors */
	const auto off_indicator_color = c_color ( 255 , 35 , 35 ) , on_indicator_color = c_color ( 35 , 255 , 35 ) , mid_indicator_color = c_color ( 255 , 255 , 35 ) , outline_color = c_color ( 0 , 0 , 0 , 240 );

	if ( r_tickbasemanip ) {
		hack::game::renderer::text ( c_vector2 ( screen_width - 1 , screen_height - 1 ) , outline_color , STR ( "TICKBASE" ) , hack::game::renderer::fonts::menu_font , true );
		
		if ( hack::modules::rage::tickbase::can_we_dt_again && hack::modules::rage::tickbase::tickbase_key_state ) {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , on_indicator_color , STR ( "TICKBASE" ) , hack::game::renderer::fonts::menu_font , true );
		} else if ( hack::modules::rage::tickbase::shift_amount > 0 ) {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , mid_indicator_color , STR ( "TICKBASE" ) , hack::game::renderer::fonts::menu_font , true );
		} else {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , off_indicator_color , STR ( "TICKBASE" ) , hack::game::renderer::fonts::menu_font , true );
		}

		screen_height += 16;
	}

	if ( rage_fakeduck ) {
		hack::game::renderer::text ( c_vector2 ( screen_width - 1 , screen_height - 1 ) , outline_color , STR ( "FAKEDUCK" ) , hack::game::renderer::fonts::menu_font , true );
		
		if ( hack::modules::rage::antiaim::rage_fakeduck_keystate ) {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , on_indicator_color , STR ( "FAKEDUCK" ) , hack::game::renderer::fonts::menu_font , true );
		}  else {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , off_indicator_color , STR ( "FAKEDUCK" ) , hack::game::renderer::fonts::menu_font , true );
		}

		screen_height += 16;
	}

	if ( aa_desync ) {
		hack::game::renderer::text ( c_vector2 ( screen_width - 1 , screen_height - 1 ) , outline_color , STR ( "INVERT" ) , hack::game::renderer::fonts::menu_font , true );
		
		if ( hack::modules::rage::antiaim::aa_desync_invert_keystate ) {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , on_indicator_color , STR ( "INVERT" ) , hack::game::renderer::fonts::menu_font , true );
		}  else {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , off_indicator_color , STR ( "INVERT" ) , hack::game::renderer::fonts::menu_font , true );
		}

		screen_height += 16;
	}

	if ( m_selfpeek ) {
		hack::game::renderer::text ( c_vector2 ( screen_width - 1 , screen_height - 1 ) , outline_color , STR ( "SELFPEEK" ) , hack::game::renderer::fonts::menu_font , true );
		
		if ( hack::modules::misc::movement::m_selfpeek_keybind_state ) {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , on_indicator_color , STR ( "SELFPEEK" ) , hack::game::renderer::fonts::menu_font , true );
		}  else {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , off_indicator_color , STR ( "SELFPEEK" ) , hack::game::renderer::fonts::menu_font , true );
		}

		screen_height += 16;
	}

	/* always on CHOKE indicator */ {
		hack::game::renderer::text ( c_vector2 ( screen_width - 1 , screen_height - 1 ) , outline_color , STR ( "CHOKE" ) , hack::game::renderer::fonts::menu_font , true );
		
		if ( hack::game::interfaces::clientstate->choked_commands ) {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , on_indicator_color , STR ( "CHOKE" ) , hack::game::renderer::fonts::menu_font , true );
		} else {
			hack::game::renderer::text ( c_vector2 ( screen_width , screen_height ) , off_indicator_color , STR ( "CHOKE" ) , hack::game::renderer::fonts::menu_font , true );
		}

		screen_height += 16;
	}
}

void draw_local_info ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::globals::local || !hack::game::globals::local->alive ( ) )
		return;

	auto mata = c_color ( 255 , 255 , 255 );
	int maaaaa = 15;
	hack::game::renderer::text ( c_vector2 ( 45 , maaaaa ) , mata , STR ( "local info" ) , hack::game::renderer::fonts::menu_font , false );
	maaaaa += 15;

	for ( const auto & value : hack::game::globals::local->pose_parameter ( ) ) {
		hack::game::renderer::text ( c_vector2 ( 45 ,  maaaaa ) , mata , std::to_string ( value ) , hack::game::renderer::fonts::menu_font , false );
		maaaaa += 15;
	}

	maaaaa = 15;

	for ( int i = 0; i < hack::game::globals::local->num_anim_overlays(); i++ ) {
		hack::game::renderer::text ( c_vector2 ( 600 , maaaaa ) , mata , std::to_string ( hack::game::globals::local->anim_overlays ( ) [ i ].m_cycle ) , hack::game::renderer::fonts::menu_font , false );
		maaaaa += 15;
		hack::game::renderer::text ( c_vector2 ( 600 , maaaaa ) , mata , std::to_string ( hack::game::globals::local->anim_overlays ( ) [ i ].m_playback_rate ) , hack::game::renderer::fonts::menu_font , false );
		maaaaa += 15;
	}
}

void custom_rain ( ) { /* nope */ }

void hack::modules::visuals::misc::in_endscene ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::globals::local )
		return;

	const auto & fx_indicators = CONFIG_GET ( bool , fnv1a ( "fx_indicators" ) );

	if ( fx_indicators )
		indicators ( );

	draw_local_info ( );
	// custom_rain ( );
}
