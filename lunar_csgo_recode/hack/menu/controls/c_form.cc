#include "../framework.hh"

void hack::menu::controls::c_form::init ( const c_vector2 & pos , const c_vector2 & size ) {
	/* position is an offset from the actual parent position - i know i shouldn't hardcode it but i'm behind schedule on the cheat :( */
	this->m_position = c_vector2 ( 6 , 6 );
	this->m_size = c_vector2 ( size.x - 12 , 50 );
	this->m_selected_tab = 0;

	/* init for all groupboxes */
	if ( !this->m_subtabs.empty ( ) )
		for ( auto & subtab : this->m_subtabs )
			subtab->init ( pos , size );
}

void hack::menu::controls::c_form::bind ( std::shared_ptr<c_subtab> subtab ) {
	this->m_subtabs.push_back ( std::move ( subtab ) );
}

void hack::menu::controls::c_form::think ( const c_vector2 & pos ) {
	if ( !this->m_subtabs.empty ( ) && this->m_subtabs.at ( m_selected_tab ) )
		this->m_subtabs.at ( m_selected_tab )->think ( pos );
}

void hack::menu::controls::c_form::draw ( const c_vector2 & pos ) {
	auto context_cursor = pos + m_position;

	hack::game::renderer::gradient_rectangle ( context_cursor , c_vector2 ( this->m_size.x , 4 ) , config::accent_color , config::secondary_color , false );

	context_cursor.y += 4;

	hack::game::renderer::filled_rectangle ( context_cursor , c_vector2 ( this->m_size.x , 35 ) , c_color ( 42 , 42 , 42 , 255 ) );

	context_cursor.y -= 4;

	const auto acc = c_color ( config::accent_color.r , config::accent_color.g , config::accent_color.b , 30 );
	const auto sec = c_color ( config::secondary_color.r , config::secondary_color.g , config::secondary_color.b , 30 );
	hack::game::renderer::gradient_rectangle ( c_vector2 ( context_cursor.x , context_cursor.y - 2 ) , c_vector2 ( this->m_size.x , 8 ) , acc , sec , false );
	hack::game::renderer::gradient_rectangle ( c_vector2 ( context_cursor.x , context_cursor.y - 1 ) , c_vector2 ( this->m_size.x , 6 ) , acc , sec , false );

	context_cursor.y += 10;

	auto subtab_list_pos = 6;

	auto text_size = hack::game::renderer::text_position ( STR ( "<" ) , hack::game::renderer::fonts::menu_bold_font );

	hack::game::renderer::text ( c_vector2 ( context_cursor.x + subtab_list_pos , context_cursor.y ) , c_color ( 220 , 220 , 220 , 255 ) , STR ( "<" ) , hack::game::renderer::fonts::menu_bold_font );

	if ( input::key_released ( 1 ) && input::mouse_in_region ( context_cursor.x + subtab_list_pos , context_cursor.y , text_size.x , text_size.y ) )
		queued_tab = 0;

	subtab_list_pos += text_size.x + 10;

	auto idx = 0;

	for ( const auto & tab : this->m_subtabs ) {
		text_size = hack::game::renderer::text_position ( tab->m_name , hack::game::renderer::fonts::menu_bold_font );

		hack::game::renderer::text ( c_vector2 ( context_cursor.x + subtab_list_pos , context_cursor.y ) , c_color ( 220 , 220 , 220 , 255 ) , tab->m_name , hack::game::renderer::fonts::menu_bold_font );

		if ( input::key_released ( 1 ) && input::mouse_in_region ( context_cursor.x + subtab_list_pos , context_cursor.y , text_size.x , text_size.y ) )
			this->m_selected_tab = idx;

		subtab_list_pos += text_size.x + 15;

		idx++;
	}

	context_cursor.y += 33;

	if ( !this->m_subtabs.empty ( ) && this->m_subtabs.at ( m_selected_tab ) )
		this->m_subtabs.at ( m_selected_tab )->draw ( pos );
}