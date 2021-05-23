#include "esp.hh"

#include "../../menu/config.hh"

#include "../../game/renderer.hh"

#include "../../../sdk/math/mathutils.hh"
#include "../../../sdk/weapon.hh"
#include "../misc/others.hh"

bool grab_esp_info ( sdk::c_player * player , hack::modules::visuals::esp::esp_info_t & info ) {
	if ( !player || !player->alive ( ) || player->lifestate ( ) )
		return false;

	const auto & trans = player->coordonate_frame ( );

	const c_vector3 & min = player->collision ( ).mins ( );
	const c_vector3 & max = player->collision ( ).maxs ( );

	c_vector3 point_list [ ] = {
		c_vector3 ( min.x, min.y, min.z ), c_vector3 ( min.x, max.y, min.z ),
		c_vector3 ( max.x, max.y, min.z ), c_vector3 ( max.x, min.y, min.z ),
		c_vector3 ( max.x, max.y, max.z ), c_vector3 ( min.x, max.y, max.z ),
		c_vector3 ( min.x, min.y, max.z ), c_vector3 ( max.x, min.y, max.z )
	};

	c_vector3 transformed [ 8 ];

	for ( auto i = 0; i < 8; i++ )
		sdk::math::transform_vector ( point_list [ i ] , trans , transformed [ i ] );

	c_vector3 screen_pos [ 8 ] { c_vector3 ( 0.f, 0.f ) };

	for ( auto i = 0; i < 8; i++ )
		hack::game::renderer::world_to_screen ( transformed [ i ] , screen_pos [ i ] );

	float left = FLT_MAX , top = FLT_MIN , right = FLT_MIN , bottom = FLT_MAX;

	for ( int i {}; i < 8; i++ ) {
		left = std::min<float> ( left , screen_pos [ i ].x );
		top = std::max<float> ( top , screen_pos [ i ].y );
		right = std::max<float> ( right , screen_pos [ i ].x );
		bottom = std::min<float> ( bottom , screen_pos [ i ].y );
	}

	if ( right - left > 4000.f || top - bottom > 4000.f )
		return false;

	info.m_pos.x = static_cast< int >( left );
	info.m_pos.y = static_cast< int >( bottom );
	info.m_size.x = static_cast< int >( right - left );
	info.m_size.y = static_cast< int >( top - bottom );
	info.m_player = player;
	info.m_dormant = player->dormant ( );

	return true;
}

void do_esp_box ( const hack::modules::visuals::esp::esp_info_t & info ) {
	const auto & accent_color = CONFIG_GET ( c_color , fnv1a ( "e_acc_col" ) );
	const auto & secondary_color = CONFIG_GET ( c_color , fnv1a ( "e_sec_col" ) );
	const auto & border_color = CONFIG_GET ( c_color , fnv1a ( "e_border_col" ) );

	hack::game::renderer::rectangle ( c_vector2 ( info.m_pos.x - 1 , info.m_pos.y - 1 ) , c_vector2 ( info.m_size.x + 3 , info.m_size.y + 3 ) , border_color );
	hack::game::renderer::rectangle ( c_vector2 ( info.m_pos.x + 1 , info.m_pos.y + 1 ) , c_vector2 ( info.m_size.x - 1 , info.m_size.y - 1 ) , border_color );

	if ( !info.m_dormant ) {
		hack::game::renderer::gradient_rectangle ( info.m_pos , c_vector2 ( info.m_size.x , 1 ) , accent_color , secondary_color , false );
		hack::game::renderer::gradient_rectangle ( c_vector2 ( info.m_pos.x + info.m_size.x , info.m_pos.y ) , c_vector2 ( 1 , info.m_size.y ) , secondary_color , accent_color , true );
		hack::game::renderer::gradient_rectangle ( c_vector2 ( info.m_pos.x , info.m_pos.y + info.m_size.y ) , c_vector2 ( info.m_size.x + 1 , 1 ) , secondary_color , accent_color , false );
		hack::game::renderer::gradient_rectangle ( info.m_pos , c_vector2 ( 1 , info.m_size.y ) , accent_color , secondary_color , true );
	}
}

void do_esp_name ( const hack::modules::visuals::esp::esp_info_t & info ) {
	constexpr auto clean_name = [ ] ( std::string name , bool is_bot ) {
		std::string tmp = STR ( "BOT " );

		if ( is_bot ) tmp += name;
		else tmp = name;

		if ( tmp.length ( ) > 30 ) {
			tmp.erase ( 30 , ( tmp.length ( ) - 30 ) );
			tmp.append ( STR ( ".." ) );
		}

		return tmp;
	};

	sdk::valve::player_info_t player_info;

	hack::game::interfaces::engine->get_player_info ( info.m_player->index ( ) , &player_info );

	const auto name = clean_name ( player_info.m_name , player_info.m_fake_player );
	const auto text_size = hack::game::renderer::text_position ( name , hack::game::renderer::fonts::esp_font );

	hack::game::renderer::text ( c_vector2 ( info.m_pos.x + ( info.m_size.x / 2 ) + 1 , info.m_pos.y - text_size.y - 1 ) , c_color ( 0 , 0 , 0 , 190 ) , name , hack::game::renderer::fonts::esp_font , true );
	hack::game::renderer::text ( c_vector2 ( info.m_pos.x + ( info.m_size.x / 2 ) , info.m_pos.y - text_size.y - 2 ) , c_color ( 255 , 255 , 255 , info.m_dormant ? 160 : 255 ) , name , hack::game::renderer::fonts::esp_font , true );
}

void do_esp_weapon ( const hack::modules::visuals::esp::esp_info_t & info ) {
	auto name = hack::modules::misc::others::weapon_name.at ( info.m_player->index ( ) );

	/* draw as icon */
	if ( name.second ) {
		const auto text_size = hack::game::renderer::text_position ( name.first , hack::game::renderer::fonts::icon_font );

		hack::game::renderer::text ( c_vector2 ( info.m_pos.x + ( info.m_size.x / 2 ) + 1 , info.m_pos.y + info.m_size.y + 7 ) , c_color ( 0 , 0 , 0 , 190 ) , name.first , hack::game::renderer::fonts::icon_font , true );
		hack::game::renderer::text ( c_vector2 ( info.m_pos.x + ( info.m_size.x / 2 ) , info.m_pos.y + info.m_size.y + 6 ) , c_color ( 255 , 255 , 255 , info.m_dormant ? 160 : 255 ) , name.first , hack::game::renderer::fonts::icon_font , true );

		return;
	}

	const auto text_size = hack::game::renderer::text_position ( name.first , hack::game::renderer::fonts::esp_font );

	hack::game::renderer::text ( c_vector2 ( info.m_pos.x + ( info.m_size.x / 2 ) + 1 , info.m_pos.y + info.m_size.y + 7 ) , c_color ( 0 , 0 , 0 , 190 ) , name.first , hack::game::renderer::fonts::esp_font , true );
	hack::game::renderer::text ( c_vector2 ( info.m_pos.x + ( info.m_size.x / 2 ) , info.m_pos.y + info.m_size.y + 6 ) , c_color ( 255 , 255 , 255 , info.m_dormant ? 160 : 255 ) , name.first , hack::game::renderer::fonts::esp_font , true );
}

void do_esp_health ( const hack::modules::visuals::esp::esp_info_t & info ) {
	const auto & accent = CONFIG_GET ( c_color , fnv1a ( "e_hpbar_acc" ) );
	const auto & secondary = CONFIG_GET ( c_color , fnv1a ( "e_hpbar_sec" ) );
	const auto & border = CONFIG_GET ( c_color , fnv1a ( "e_border_col" ) );
	const auto & other_border = c_color ( border.r , border.g , border.b , border.a / 2 );

	const float health_value = std::min ( info.m_player->health ( ) , 100 ) / 100.f;

	hack::game::renderer::rectangle ( c_vector2 ( info.m_pos.x - 1 , info.m_pos.y + info.m_size.y + 3 ) , c_vector2 ( info.m_size.x + 3 , 5 ) , border );
	hack::game::renderer::gradient_rectangle ( c_vector2 ( info.m_pos.x , info.m_pos.y + info.m_size.y + 4 ) , c_vector2 ( ( info.m_size.x + 1 ) * health_value , 3 ) , info.m_dormant ? other_border : secondary , info.m_dormant ? other_border : accent , false );
}

void hack::modules::visuals::esp::in_endscene ( ) {
	/* sanity checks */
	if ( !hack::game::interfaces::engine->is_in_game ( ) || !hack::game::globals::local )
		return;

	/* variables */
	const auto & esp_enabled = CONFIG_GET ( bool , fnv1a ( "e_enabled" ) );

	if ( !esp_enabled )
		return;

	/* run entity esp */


	const auto & esp_box = CONFIG_GET ( bool , fnv1a ( "e_box" ) );
	const auto & esp_name = CONFIG_GET ( bool , fnv1a ( "e_name" ) );
	const auto & esp_hp = CONFIG_GET ( bool , fnv1a ( "e_hpbar" ) );
	const auto & esp_weap = CONFIG_GET ( bool , fnv1a ( "e_weap" ) );

	/* run player esp */
	sdk::foreach_player ( [ & ] ( sdk::c_player * player ) {
		if ( hack::game::globals::local == player || !player->enemy ( ) )
			return;

		esp_info_t box_info;

		if ( !grab_esp_info ( player , box_info ) )
			return;

		if ( esp_box )
			do_esp_box ( box_info );

		if ( esp_name )
			do_esp_name ( box_info );

		if ( esp_hp )
			do_esp_health ( box_info );

		if ( esp_weap )
			do_esp_weapon ( box_info );
	} , true );
}