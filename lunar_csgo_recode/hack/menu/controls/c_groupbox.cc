#include "../framework.hh"

void hack::menu::controls::c_groupbox::init ( const c_vector2 & pos , const c_vector2 & size ) {
	auto context_cursor = pos;

	context_cursor.x += 6;
	context_cursor.y += 50;

	this->m_position = context_cursor;
	this->m_size = c_vector2 ( size.x - 12 , size.y - context_cursor.y - 20 );

	for ( auto & ctrl : this->m_members )
		ctrl->init ( pos , size );
}

void hack::menu::controls::c_groupbox::bind ( std::shared_ptr<c_control> control ) {
	this->m_members.push_back ( std::move ( control ) );
}

void hack::menu::controls::c_groupbox::draw ( const c_vector2 & pos ) {
	auto context_cursor = pos + this->m_position;

	const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_bold_font );
	const auto tool_size = hack::game::renderer::text_position ( this->m_tooltip , hack::game::renderer::fonts::menu_font );

	hack::game::renderer::text ( c_vector2 ( context_cursor.x + 15 , context_cursor.y ) , config::accent_color , this->m_name , hack::game::renderer::fonts::menu_bold_font );
	context_cursor.y += text_size.y + 2;

	hack::game::renderer::text ( c_vector2 ( context_cursor.x + 15 , context_cursor.y ) , config::accent_color , this->m_tooltip , hack::game::renderer::fonts::menu_font );
	context_cursor.y += tool_size.y;

	context_cursor.y += 8;

	const auto acc = c_color ( config::accent_color.r , config::accent_color.g , config::accent_color.b , 30 );
	const auto sec = c_color ( config::secondary_color.r , config::secondary_color.g , config::secondary_color.b , 30 );

	hack::game::renderer::gradient_rectangle ( context_cursor , c_vector2 ( this->m_size.x , 4 ) , config::accent_color , config::secondary_color , false );

	context_cursor.y += 4;

	hack::game::renderer::filled_rectangle ( context_cursor , c_vector2 ( this->m_size.x , this->m_size.y - 35 ) , c_color ( 42 , 42 , 42 , 255 ) );

	context_cursor.y -= 4;

	hack::game::renderer::gradient_rectangle ( c_vector2 ( context_cursor.x , context_cursor.y - 2 ) , c_vector2 ( this->m_size.x , 8 ) , acc , sec , false );
	hack::game::renderer::gradient_rectangle ( c_vector2 ( context_cursor.x , context_cursor.y - 1 ) , c_vector2 ( this->m_size.x , 6 ) , acc , sec , false );

	context_cursor.y += 4;

	context_cursor.x += 16;
	context_cursor.y += 12;

	/* do clipping */
	const auto old_viewport = hack::game::renderer::start_clipping ( context_cursor , c_vector2 ( this->m_size.x , this->m_size.y - 35 ) );

	/* draw and think all of our children -- much easier to do here since we will manipulate the cursor vector */
	for ( auto & control : this->m_members ) {
		control->think ( context_cursor );
		control->draw ( context_cursor );
	}

	/* undo clipping */
	hack::game::renderer::end_clipping ( old_viewport );
}
