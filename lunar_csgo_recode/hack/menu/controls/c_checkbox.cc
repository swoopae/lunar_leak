#include "../framework.hh"

void hack::menu::controls::c_checkbox::init ( const c_vector2 & pos , const c_vector2 & size ) {
	this->m_size.x = ( size.x - 44 );
}

void hack::menu::controls::c_checkbox::think ( const c_vector2 & pos ) {
	if ( m_hash && m_hash != this->m_value_hash ) {
		/* handle children of our element and return */
		c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

		for ( auto & child : this->m_children ) {
			child->think ( cursor );
			cursor.x -= 35;
		}
		return;
	}

	const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

	auto & value = CONFIG_GET ( bool , this->m_value_hash );

	if ( input::key_released ( 1 ) && input::mouse_in_region ( pos.x , pos.y , text_size.y + 10 + text_size.x , text_size.y ) ) {
		value = !value;
		this->m_last_click_animtime = 1.f;
	}

	/* handle children of our element */
	c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

	for ( auto & child : this->m_children ) {
		child->think ( cursor );
		cursor.x -= 35;
	}
}

void hack::menu::controls::c_checkbox::draw ( c_vector2 & pos ) {
	const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

	auto & value = CONFIG_GET ( bool , this->m_value_hash );

	hack::game::renderer::filled_rectangle ( pos , c_vector2 ( text_size.y , text_size.y ) , c_color ( 54 , 54 , 54 , 130 ) );

	/* do clipping */
	const auto old_viewport = hack::game::renderer::start_clipping ( pos , c_vector2 ( text_size.y , text_size.y ) );

	/* draw our animation */
	if ( this->m_last_click_animtime > 0.1f ) {
		if ( value ) {
			const auto factor = std::fabsf ( this->m_last_click_animtime - 1.f );

			hack::game::renderer::filled_circle ( c_vector2 ( pos.x + ( text_size.y / 2 ) , pos.y + ( text_size.y / 2 ) ) , text_size.y * utils::ease_out_quart ( factor ) , 45 , c_color ( 70 , 70 , 70 , 255 ) );
		} else {
			hack::game::renderer::filled_circle ( c_vector2 ( pos.x + ( text_size.y / 2 ) , pos.y + ( text_size.y / 2 ) ) , text_size.y * utils::ease_in_quart ( this->m_last_click_animtime ) , 45 , c_color ( 70 , 70 , 70 , 255 ) );
		}

		this->m_last_click_animtime -= globals::frametime;
	} else if ( value ) {
		hack::game::renderer::filled_rectangle ( pos , c_vector2 ( text_size.y , text_size.y ) , c_color ( 70 , 70 , 70 , 255 ) );
	}

	/* undo clipping */
	hack::game::renderer::end_clipping ( old_viewport );

	hack::game::renderer::text ( c_vector2 ( pos.x + text_size.y + 10 , pos.y ) , c_color ( 255 , 255 , 255 ) , this->m_name.data ( ) , hack::game::renderer::fonts::menu_font );

	/* handle children of our element */
	c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

	for ( auto & child : this->m_children ) {
		child->draw ( cursor );
		cursor.x -= 35;
	}

	pos.y += text_size.y + 7;
}