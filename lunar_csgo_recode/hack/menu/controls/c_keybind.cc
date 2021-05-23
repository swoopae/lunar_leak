#include "../framework.hh"

void hack::menu::controls::c_keybind::think ( const c_vector2 & pos ) {
	/* if we aren't the top most element and there's some other element blocking input */
	if ( this->m_value_hash != m_hash && m_hash )
		return;

	auto & value = CONFIG_GET ( keybind_t , this->m_value_hash );

	const auto text_size = hack::game::renderer::text_position ( STR ( "pt" ) , hack::game::renderer::fonts::menu_font );
	const auto element_size = c_vector2 ( 30 , text_size.y );

	if ( input::key_released ( 1 ) && input::mouse_in_region ( pos.x , pos.y , element_size.x , element_size.y ) ) {
		m_hash = this->m_value_hash;
		this->m_popup = false;
	} else if ( input::key_released ( 2 ) && input::mouse_in_region ( pos.x , pos.y , element_size.x , element_size.y ) ) {
		m_hash = this->m_value_hash;
		this->m_animtime = 0.f;
		this->m_popup = true;
	}

	/* if we aren't blocking input */
	if ( this->m_value_hash != m_hash )
		return;

	if ( this->m_popup ) {
		const auto popup_pos = c_vector2 ( pos.x + element_size.x + 10 , pos.y );
		const auto popup_size = c_vector2 ( 50 , 50 );

		if ( input::key_released ( 1 ) && !input::mouse_in_region ( popup_pos.x , popup_pos.y , popup_size.x , popup_size.y ) && this->m_animtime > 0.8f ) {
			m_hash = 0;
			this->m_animtime = 0.f;

			return;
		}

		if ( input::key_released ( 1 ) && input::mouse_in_region ( popup_pos.x , popup_pos.y , popup_size.x , popup_size.y ) && this->m_animtime > 0.8f ) {
			if ( input::mouse_in_region ( popup_pos.x , popup_pos.y , popup_size.x , 17 ) )
				value.m_type = 0;
			if ( input::mouse_in_region ( popup_pos.x , popup_pos.y + 17 , popup_size.x , 17 ) )
				value.m_type = 1;
			if ( input::mouse_in_region ( popup_pos.x , popup_pos.y + 34 , popup_size.x , 17 ) )
				value.m_type = 2;
		}
	} else {
		for ( auto i = 1; i < 256; i++ ) {
			if ( input::key_pressed ( i ) && fnv1a_rt ( utils::keys_list [ i ].data ( ) ) != fnv1a ( "error" ) ) {
				value.m_key = i;
				m_hash = 0;
			}
		}
	}
}

void hack::menu::controls::c_keybind::draw ( const c_vector2 & pos ) {
	const auto text_size = hack::game::renderer::text_position ( STR ( "pt" ) , hack::game::renderer::fonts::menu_font );

	auto & value = CONFIG_GET ( keybind_t , this->m_value_hash );

	const auto element_size = c_vector2 ( 30 , text_size.y );

	hack::game::renderer::filled_rectangle ( pos , element_size , c_color ( 70 , 70 , 70 , 255 ) );

	hack::game::renderer::text ( c_vector2 ( pos.x + element_size.x / 2 , pos.y ) , c_color ( 255 , 255 , 255 , 255 ) , ( this->m_value_hash == m_hash && !this->m_popup ) ? STR ( ". . ." ) : utils::keys_list [ value.m_key ] , hack::game::renderer::fonts::menu_font , true );

	if ( this->m_value_hash != m_hash )
		return;

	const auto popup_pos = c_vector2 ( pos.x + element_size.x + 10 , pos.y );
	const auto popup_size = c_vector2 ( 50 , 50 );

	if ( this->m_animtime < 1.f && this->m_popup )
		this->m_animtime += globals::frametime * 2.133f;

	this->m_animtime = std::clamp ( this->m_animtime , 0.f , 1.f );

	if ( !this->m_popup )
		return;

	layer_queue.push_back ( utils::draw_call_t ( popup_pos , c_vector2 ( popup_size.x , popup_size.y * utils::ease_out_quart ( this->m_animtime ) ) ) );

	layer_queue.push_back ( utils::draw_call_t ( popup_pos , popup_size , c_color ( 47 , 47 , 47 , 255 ) ) );

	layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + ( popup_size.x / 2 ) , popup_pos.y + 2 ) , true , STR ( "ignore" ) , hack::game::renderer::fonts::menu_font , value.m_type ? c_color ( 255 , 255 , 255 , 255 ) : config::accent_color ) );
	layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + ( popup_size.x / 2 ) , popup_pos.y + 16 ) , true , STR ( "toggle" ) , hack::game::renderer::fonts::menu_font , value.m_type != 1 ? c_color ( 255 , 255 , 255 , 255 ) : config::accent_color ) );
	layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + ( popup_size.x / 2 ) , popup_pos.y + 30 ) , true , STR ( "hold" ) , hack::game::renderer::fonts::menu_font , value.m_type != 2 ? c_color ( 255 , 255 , 255 , 255 ) : config::accent_color ) );
}