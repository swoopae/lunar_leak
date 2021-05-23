#include "../framework.hh"

void hack::menu::controls::c_home::init ( const c_vector2 & pos , const c_vector2 & size ) {
	this->m_position = c_vector2 ( 0 , 0 );
	this->m_size = c_vector2 ( size.x , size.y );

	this->m_selected_tab = 0;
	this->m_queued_tab = -1;

	if ( !this->m_tabs.empty ( ) )
		for ( auto & groupbox : this->m_tabs )
			groupbox->init ( this->m_position , size );
}

void hack::menu::controls::c_home::bind ( std::shared_ptr<c_form> tab ) {
	this->m_tabs.push_back ( std::move ( tab ) );
}

void hack::menu::controls::c_home::think ( const c_vector2 & pos ) {
	if ( !this->m_tabs.empty ( ) && this->m_selected_tab && this->m_tabs.at ( this->m_selected_tab - 1 ) )
		this->m_tabs.at ( this->m_selected_tab - 1 )->think ( pos );
}

void hack::menu::controls::c_home::draw ( const c_vector2 & pos ) {
	if ( !this->m_selected_tab ) {
		const auto rect_pos = c_vector2 ( pos.x + ( this->m_size.x / 2 ) - 100 , pos.y + ( this->m_size.y / 2 ) - 75 ) , rect_size = c_vector2 ( 200 , 150 );

		hack::game::renderer::gradient_rectangle ( c_vector2 ( rect_pos.x - 3 , rect_pos.y - 3 ) , c_vector2 ( rect_size.x + 6 , 1 ) , config::accent_color , config::secondary_color , false );
		hack::game::renderer::gradient_rectangle ( c_vector2 ( rect_pos.x - 3 , rect_pos.y - 3 + rect_size.y + 5 ) , c_vector2 ( rect_size.x + 6 , 1 ) , config::secondary_color , config::accent_color , false );

		hack::game::renderer::gradient_rectangle ( c_vector2 ( rect_pos.x - 3 + rect_size.x + 5 , rect_pos.y - 2 ) , c_vector2 ( 1 , rect_size.y + 4 ) , config::secondary_color , config::accent_color , true );
		hack::game::renderer::gradient_rectangle ( c_vector2 ( rect_pos.x - 3 , rect_pos.y - 2 ) , c_vector2 ( 1 , rect_size.y + 4 ) , config::accent_color , config::secondary_color , true );

		hack::game::renderer::filled_rectangle ( rect_pos - 2 , rect_size + 4 , c_color ( 37 , 37 , 37 , 255 ) );

		auto text_position = c_vector2 ( pos.x + ( this->m_size.x / 2 ) , pos.y + ( this->m_size.y / 2 ) - 40 );

		hack::game::renderer::text ( text_position , c_color ( 235 , 235 , 235 , 255 ) , m_name , hack::game::renderer::fonts::menu_title_font , true );

		const auto title_size = hack::game::renderer::text_position ( m_name , hack::game::renderer::fonts::menu_title_font );

		text_position.x -= ( title_size.x / 2 );
		text_position.y += title_size.y + 15;

		auto scrolltab = 1;

		const auto developed_text_size = hack::game::renderer::text_position ( STR ( "pt" ) , hack::game::renderer::fonts::menu_font );

		hack::game::renderer::text ( c_vector2 ( pos.x + ( this->m_size.x / 2 ) , pos.y + this->m_size.y - developed_text_size.y - 2 ) , c_color::from_hsv ( std::fmodf ( hack::menu::globals::realtime * 100.f , 360.f ) , 1.f , 0.75f ) , STR ( "made with love by swoopae" ) , hack::game::renderer::fonts::menu_font , true );

		for ( const auto & tab : this->m_tabs ) {
			const auto text_size = hack::game::renderer::text_position ( tab->m_name , hack::game::renderer::fonts::menu_font ) + 1;

			bool hovered = false;

			if ( input::mouse_in_region ( text_position.x , text_position.y , text_size.x , text_size.y ) )
				hovered = true;

			if ( input::key_released ( 1 ) && hovered ) {
				this->m_queued_tab = scrolltab;
				this->m_time_in_anim = 2.f;
			}

			hack::game::renderer::text ( text_position , hovered ? c_color ( 240 , 240 , 240 , 255 ) : c_color ( 220 , 220 , 220 , 255 ) , tab->m_name , hack::game::renderer::fonts::menu_font , false );

			text_position.x += text_size.x;
			scrolltab++;
		}

		return;
	}

	if ( queued_tab != -1 ) {
		this->m_queued_tab = queued_tab;
		this->m_time_in_anim = 2.f;
		queued_tab = -1;
	}

	if ( !this->m_tabs.empty ( ) && this->m_tabs.at ( this->m_selected_tab - 1 ) )
		this->m_tabs.at ( this->m_selected_tab - 1 )->draw ( pos );
}
