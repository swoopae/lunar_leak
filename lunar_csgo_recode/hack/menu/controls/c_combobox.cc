#include "../framework.hh"

void hack::menu::controls::c_combobox::init ( const c_vector2 & pos , const c_vector2 & size ) {
	this->m_size.x = ( size.x - 44 );
}

void hack::menu::controls::c_combobox::think ( const c_vector2 & pos ) {
	// if ( m_hash && m_hash != this->m_value_hash ) {
		/* handle children of our element and return */
	c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

	for ( auto & child : this->m_children ) {
		child->think ( cursor );
		cursor.x -= 35;
	}
	// }

	auto & value = CONFIG_GET ( int , this->m_value_hash );

	value = std::clamp ( value , 0 , static_cast< int >( this->m_members.size ( ) ) );
	this->m_scroll_time = std::clamp ( this->m_scroll_time , 0.f , 0.3f );

	if ( m_hash && m_hash != this->m_value_hash )
		return;

	const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

	auto thinkpos = c_vector2 ( pos.x , pos.y + text_size.y + 4 );

	if ( input::key_released ( 1 ) && input::mouse_in_region ( pos.x , pos.y + text_size.y + 4 , this->m_size.x , text_size.y + 4 ) ) {
		m_hash = this->m_value_hash;
		this->m_animtime = 0.f;
	}

	if ( m_hash != this->m_value_hash )
		return;

	if ( this->m_animtime < 1.f )
		this->m_animtime += globals::frametime * 2.133f;

	if ( this->m_scroll_time > 0.1f )
		this->m_scroll_time -= globals::frametime;

	const auto popup_pos = c_vector2 ( thinkpos.x , thinkpos.y + text_size.y + 10 );
	const auto popup_size = c_vector2 ( this->m_size.x , ( ( text_size.y + 4 ) * std::min<int> ( this->m_members.size ( ) , 10 ) ) + 5 );

	if ( input::key_released ( 1 ) && !input::mouse_in_region ( popup_pos.x , popup_pos.y , popup_size.x , popup_size.y ) && this->m_animtime > 0.8f ) {
		m_hash = 0;
		this->m_animtime = 0.f;
	}

	/* welcome to "it's 9:29 AM , i've been up for too long". today we'll present you our latest hacky shitty fix */
	if ( input::mouse_in_region ( popup_pos.x , popup_pos.y , popup_size.x , popup_size.y ) && this->m_scroll_time < 0.1f && this->m_members.size ( ) > 10 ) {
		if ( scroll_factor > 0 ) {
			this->m_scroll++;
			this->m_scroll_time = 0.15f;
		}

		if ( scroll_factor < 0 ) {
			this->m_scroll--;
			this->m_scroll_time = 0.15f;
		}

		this->m_scroll = std::clamp ( this->m_scroll , 0 , std::abs ( static_cast< int >( this->m_members.size ( ) - 10 ) ) );
	}

	for ( int i = 0; i < std::min<int> ( this->m_members.size ( ) , 10 ); i++ )
		if ( input::key_released ( 1 ) && input::mouse_in_region ( popup_pos.x , popup_pos.y + 10 + ( text_size.y + 4 ) * i , popup_size.x , text_size.y ) )
			value = this->m_scroll + i;
}

void hack::menu::controls::c_combobox::draw ( c_vector2 & pos ) {
	auto & value = CONFIG_GET ( int , this->m_value_hash );

	const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

	hack::game::renderer::text ( pos , c_color ( 255 , 255 , 255 ) , this->m_name.data ( ) , hack::game::renderer::fonts::menu_font );

	/* handle children of our element */
	c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

	for ( auto & child : this->m_children ) {
		child->draw ( cursor );
		cursor.x -= 35;
	}

	pos.y += text_size.y + 4;

	hack::game::renderer::filled_rectangle ( pos , c_vector2 ( this->m_size.x , text_size.y + 4 ) , c_color ( 54 , 54 , 54 , 130 ) );
	hack::game::renderer::text ( c_vector2 ( pos.x + ( this->m_size.x / 2 ) , pos.y + 2 ) , c_color ( 255 , 255 , 255 ) , this->m_members.at ( value ) , hack::game::renderer::fonts::menu_font , true );

	pos.y += text_size.y + 10;

	this->m_animtime = std::clamp ( this->m_animtime , 0.f , 1.f );

	if ( m_hash == this->m_value_hash ) {
		const auto popup_pos = c_vector2 ( pos.x , pos.y );
		const auto popup_size = c_vector2 ( this->m_size.x , ( ( text_size.y + 4 ) * std::min<int> ( this->m_members.size ( ) , 10 ) ) + 5 );

		layer_queue.push_back ( utils::draw_call_t ( popup_pos , c_vector2 ( popup_size.x , popup_size.y * utils::ease_out_quart ( this->m_animtime ) ) ) );

		layer_queue.push_back ( utils::draw_call_t ( popup_pos , popup_size , c_color ( 47 , 47 , 47 , 255 ) ) );

		for ( int i = 0; i < std::min<int> ( this->m_members.size ( ) , 10 ); i++ )
			layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + ( popup_size.x / 2 ) , popup_pos.y + 5 + ( text_size.y + 4 ) * i ) , true , this->m_members.at ( this->m_scroll + i ) , hack::game::renderer::fonts::menu_font , value != this->m_scroll + i ? c_color ( 255 , 255 , 255 , 255 ) : config::accent_color ) );
	}
}