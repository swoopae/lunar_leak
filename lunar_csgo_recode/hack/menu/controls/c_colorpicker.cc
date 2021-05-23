#include "../framework.hh"

void hack::menu::controls::c_colorpicker::think ( const c_vector2 & pos ) {
	/* if we aren't the top most element and there's some other element blocking input */
	if ( this->m_value_hash != m_hash && m_hash )
		return;

	const auto text_size = hack::game::renderer::text_position ( STR ( "pt" ) , hack::game::renderer::fonts::menu_font );
	const auto element_size = c_vector2 ( 30 , text_size.y );

	if ( input::key_released ( 1 ) && input::mouse_in_region ( pos.x , pos.y , element_size.x , element_size.y ) ) {
		m_hash = this->m_value_hash;
		this->m_animtime = 0.f;
	}

	/* if we aren't blocking input */
	if ( this->m_value_hash != m_hash )
		return;

	const auto popup_pos = c_vector2 ( pos.x + element_size.x + 10 , pos.y );
	const auto popup_size = c_vector2 ( 200 , 220 );

	if ( input::key_released ( 1 ) && !input::mouse_in_region ( popup_pos.x , popup_pos.y , popup_size.x , popup_size.y ) && this->m_animtime > 0.8f ) {
		m_hash = 0;
		this->m_animtime = 0.f;

		return;
	}

	/* in color rectangle */
	if ( input::key_down ( 1 ) && input::mouse_in_region ( popup_pos.x + 5 , popup_pos.y + 5 , popup_size.x - 10 , popup_size.x - 10 ) ) {
		/* get x "slider" value */
		const auto x_value_mapped = std::fabsf ( static_cast< float >( input::mouse_pos.x - ( popup_pos.x + 5 ) ) / ( popup_size.x - 10 ) );

		/* get y "slider" value */
		const auto y_value_mapped = std::fabsf ( static_cast< float >( input::mouse_pos.y - ( popup_pos.y + 5 ) ) / ( popup_size.x - 10 ) - 1.f );

		/* set cute little cursor value */
		this->m_colpos = c_vector2 ( input::mouse_pos.x - ( popup_pos.x + 5 ) - 2 , input::mouse_pos.y - ( popup_pos.x + 5 ) - 2 );

		auto & value = CONFIG_GET ( c_color , this->m_value_hash );

		/* lerp between the colors */
		c_color white ( 255 , 255 , 255 ) , black ( 0 , 0 , 0 ) , hue_col = c_color::from_hsv ( this->m_hue , 1.f , 0.5f );
		c_color white_diff = c_color ( white.r - hue_col.r , white.g - hue_col.g , white.b - hue_col.b );
		hue_col = c_color ( white.r - ( white_diff.r * x_value_mapped ) , white.g - ( white_diff.g * x_value_mapped ) , white.b - ( white_diff.b * x_value_mapped ) , m_transparency * 255 );
		hue_col = c_color ( black.r + ( hue_col.r * y_value_mapped ) , black.g + ( hue_col.g * y_value_mapped ) , black.b + ( hue_col.b * y_value_mapped ) , m_transparency * 255 );

		/* hope to god this is correct and set the color */
		value = hue_col;
	}

	/* hue bar */
	if ( input::key_down ( 1 ) && input::mouse_in_region ( popup_pos.x + 5 , popup_pos.y + 5 + popup_size.x - 7 , popup_size.x - 10 , 6 ) ) {
		/* get x "slider" value */
		const auto x_value_mapped = std::fabsf ( static_cast< float >( input::mouse_pos.x - ( popup_pos.x + 5 ) ) / ( popup_size.x - 10 ) );

		/* set new hue */
		this->m_hue = 360.f * x_value_mapped;
	}

	/* transparency bar */
	if ( input::key_down ( 1 ) && input::mouse_in_region ( popup_pos.x + 5 , popup_pos.y + 5 + popup_size.x + 3 , popup_size.x - 10 , 6 ) ) {
		/* get x "slider" value */
		const auto x_value_mapped = std::fabsf ( static_cast< float >( input::mouse_pos.x - ( popup_pos.x + 5 ) ) / ( popup_size.x - 10 ) - 1.f );

		/* set new transparency */
		this->m_transparency = x_value_mapped;
	}
}

void hack::menu::controls::c_colorpicker::draw ( const c_vector2 & pos ) {
	const auto text_size = hack::game::renderer::text_position ( STR ( "pt" ) , hack::game::renderer::fonts::menu_font );

	auto & value = CONFIG_GET ( c_color , this->m_value_hash );

	const auto element_size = c_vector2 ( 30 , text_size.y );

	hack::game::renderer::filled_rectangle ( pos , element_size , c_color ( 70 , 70 , 70 , 255 ) );
	hack::game::renderer::filled_rectangle ( pos + 1 , element_size - 2 , value );

	if ( this->m_value_hash != m_hash )
		return;

	const auto popup_pos = c_vector2 ( pos.x + element_size.x + 10 , pos.y );
	const auto popup_size = c_vector2 ( 200 , 220 );

	if ( this->m_animtime < 1.f )
		this->m_animtime += globals::frametime * 2.133f;

	this->m_animtime = std::clamp ( this->m_animtime , 0.f , 1.f );

	layer_queue.push_back ( utils::draw_call_t ( popup_pos , popup_size * utils::ease_out_quart ( this->m_animtime ) ) );

	layer_queue.push_back ( utils::draw_call_t ( popup_pos , popup_size , c_color ( 21 , 21 , 21 , 255 ) ) );
	layer_queue.push_back ( utils::draw_call_t ( popup_pos + 1 , popup_size - 2 , c_color ( 34 , 34 , 34 , 255 ) ) );
	layer_queue.push_back ( utils::draw_call_t ( popup_pos + 4 , c_vector2 ( popup_size.x - 8 , popup_size.x - 8 ) , c_color ( 47 , 47 , 47 , 255 ) ) );
	layer_queue.push_back ( utils::draw_call_t ( popup_pos + 5 , c_vector2 ( popup_size.x - 10 , popup_size.x - 10 ) , c_color ( 255 , 255 , 255 , 255 ) , c_color::from_hsv ( this->m_hue , 1.f , 0.5f ) , true ) );
	layer_queue.push_back ( utils::draw_call_t ( popup_pos + 5 , c_vector2 ( popup_size.x - 10 , popup_size.x - 10 ) , c_color ( 0 , 0 , 0 , 0 ) , c_color ( 0 , 0 , 0 , 255 ) , false ) );

	layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + 4 , popup_pos.y + 5 + popup_size.x - 8 ) , c_vector2 ( popup_size.x - 8 , 8 ) , c_color ( 47 , 47 , 47 , 255 ) ) );
	layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + 5 , popup_pos.y + 5 + popup_size.x - 7 ) , c_vector2 ( 1 , 1 ) , hack::game::renderer::texture_wrappers::hue_bar_texture_wrapper , 0.742f ) );

	layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + 4 , popup_pos.y + 5 + popup_size.x + 2 ) , c_vector2 ( popup_size.x - 8 , 8 ) , c_color ( 47 , 47 , 47 , 255 ) ) );
	layer_queue.push_back ( utils::draw_call_t ( c_vector2 ( popup_pos.x + 5 , popup_pos.y + 5 + popup_size.x + 3 ) , c_vector2 ( 1 , 1 ) , hack::game::renderer::texture_wrappers::transparency_bar_texture_wrapper , 0.742f ) );
}