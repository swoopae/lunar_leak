#include "../framework.hh"

void hack::menu::controls::c_slider::init ( const c_vector2 & pos , const c_vector2 & size ) { 
	this->m_size.x = ( size.x - 44 );
}

void hack::menu::controls::c_slider::think ( const c_vector2 & pos ) {
	if ( m_hash && m_hash != this->m_value_hash )
		return;

	const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

	auto & value = CONFIG_GET ( int , this->m_value_hash );

	if ( input::key_down ( 1 ) && input::mouse_in_region ( pos.x , pos.y + text_size.y + 4 , this->m_size.x + 2 , 6 ) ) {
		const int value_unmapped = std::clamp ( static_cast< int >( input::mouse_pos.x - pos.x ) , 0 , static_cast< int >( this->m_size.x ) );
		const int value_mapped = static_cast< int >( value_unmapped / this->m_size.x * ( this->m_max - this->m_min ) + this->m_min );

		value = std::clamp ( value_mapped , this->m_min , this->m_max );
	}
}

void hack::menu::controls::c_slider::draw ( c_vector2 & pos ) {
	auto & value = CONFIG_GET ( int , this->m_value_hash );

	const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );
	const int dynamic_width = ( static_cast< float >( value ) - this->m_min ) / ( this->m_max - this->m_min ) * this->m_size.x;

	/* do clipping */
	// const auto old_viewport = hack::game::renderer::start_clipping ( pos , c_vector2 ( text_size.y , text_size.y ) );

	/* undo clipping */
	// hack::game::renderer::end_clipping ( old_viewport );

	hack::game::renderer::text ( pos , c_color ( 255 , 255 , 255 ) , std::string ( this->m_name + STR ( ": " ) ).data ( ) , hack::game::renderer::fonts::menu_font );
	hack::game::renderer::text ( c_vector2 ( pos.x + text_size.x + 6 , pos.y ) , c_color ( 255 , 255 , 255 ) , std::to_string ( value ) , hack::game::renderer::fonts::menu_font );

	pos.y += text_size.y + 4;

	hack::game::renderer::filled_rectangle ( pos , c_vector2 ( this->m_size.x , 6 ) , c_color ( 54 , 54 , 54 , 130 ) );
	hack::game::renderer::filled_rectangle ( pos , c_vector2 ( dynamic_width , 6 ) , c_color ( 70 , 70 , 70 , 255 ) );

	pos.y += 12;
}