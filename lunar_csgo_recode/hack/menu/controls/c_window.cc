#include "../framework.hh"

void hack::menu::controls::c_window::init ( const c_vector2 & size , const c_vector2 & position ) { 
	this->m_position = position;
	this->m_size = size;
	this->m_on_top = false;
	this->m_is_dragging = false;

	if ( this->m_home_obj )
		this->m_home_obj->init ( this->m_position , this->m_size );
}

void hack::menu::controls::c_window::bind ( std::shared_ptr<c_home> form ) {
	this->m_home_obj.swap ( form );
}

void hack::menu::controls::c_window::think ( ) {
	/* for multiple window support */
	if ( input::key_pressed ( 1 ) && input::mouse_in_region ( this->m_position.x , this->m_position.y , this->m_size.x , this->m_size.y ) )
		this->m_on_top = true;

	/* don't think is this isn't the window on top */
	if ( !this->m_on_top )
		return;

	/* handle window dragging only if it was on top for more than this tick */
	if ( input::mouse_in_region ( this->m_position.x , this->m_position.y , this->m_size.x , 15 ) ) {
		this->m_is_dragging = true;
	} else if ( input::key_down ( 1 ) && this->m_is_dragging ) {
		this->m_position.x += input::mouse_pos.x - input::prev_mouse_pos.x;
		this->m_position.y += input::mouse_pos.y - input::prev_mouse_pos.y;
	} else if ( !input::key_down ( 1 ) && this->m_is_dragging ) {
		this->m_is_dragging = false;
	}

	/* think for selected form */
	this->m_home_obj->think ( this->m_position );

	/* make sure window doesn't go out of bounds */
	/* RETARD ALERT: GAME SPECIFIC CODE! */
	int width , height;
	hack::game::interfaces::engine->get_screen_size ( width , height );

	this->m_position.x = std::clamp ( this->m_position.x , 0.f , width - this->m_size.x );
	this->m_position.y = std::clamp ( this->m_position.y , 0.f , height - this->m_size.y );
}

void hack::menu::controls::c_window::draw ( ) {
	/* main window */
	/* outside border */
	hack::game::renderer::filled_rectangle ( this->m_position - 4 , this->m_size + 8 , c_color ( 0 , 0 , 0 , 30 ) );

	/* gradient border */
	hack::game::renderer::gradient_rectangle ( c_vector2 ( this->m_position.x - 3 , this->m_position.y - 3 ) , c_vector2 ( this->m_size.x + 6 , 1 ) , config::accent_color , config::secondary_color , false );
	hack::game::renderer::gradient_rectangle ( c_vector2 ( this->m_position.x - 3 , this->m_position.y - 3 + this->m_size.y + 5 ) , c_vector2 ( this->m_size.x + 6 , 1 ) , config::secondary_color , config::accent_color , false );

	hack::game::renderer::gradient_rectangle ( c_vector2 ( this->m_position.x - 3 + this->m_size.x + 5 , this->m_position.y - 2 ) , c_vector2 ( 1 , this->m_size.y + 4 ) , config::secondary_color , config::accent_color , true );
	hack::game::renderer::gradient_rectangle ( c_vector2 ( this->m_position.x - 3 , this->m_position.y - 2 ) , c_vector2 ( 1 , this->m_size.y + 4 ) , config::accent_color , config::secondary_color , true );

	/* actual menu frame */
	hack::game::renderer::filled_rectangle ( this->m_position - 1 , this->m_size + 2 , c_color ( 21 , 21 , 21 , 255 ) );
	hack::game::renderer::filled_rectangle ( this->m_position , this->m_size , c_color ( 34 , 34 , 34 , 255 ) );

	/* title text, other borders etc. */
	// hack::game::renderer::text ( c_vector2 ( this->m_position.x + ( this->m_size.x / 2 ) , this->m_position.y ) , c_color ( 255 , 255 , 255 , 255 ) , this->m_name.data ( ) , hack::game::renderer::fonts::menu_title_font , true );

	/* do clipping */
	this->m_old_viewport = hack::game::renderer::start_clipping ( this->m_position , this->m_size );

	/* draw for selected form */
	this->m_home_obj->draw ( this->m_position );

	/* handle animations */
	/* GHETTO CODE ! ! ! */
	if ( this->m_home_obj->m_time_in_anim < 0.0f ) {
		this->m_home_obj->m_queued_tab = 0;
	} else if ( this->m_home_obj->m_time_in_anim < 1.f ) {
		if ( this->m_home_obj->m_queued_tab != -1 ) {
			this->m_home_obj->m_selected_tab = this->m_home_obj->m_queued_tab;
			this->m_home_obj->m_queued_tab = -1;
		}

		hack::game::renderer::filled_rectangle ( this->m_position , this->m_size , c_color ( 43 , 43 , 43 , 255 * utils::ease_in_quart ( this->m_home_obj->m_time_in_anim ) ) );
	} else {
		const auto factor = std::fabsf ( 1.f - ( this->m_home_obj->m_time_in_anim - 1.f ) );
		const auto draw_pos = c_vector2 ( this->m_position.x + ( this->m_size.x / 2 ) , this->m_position.y + ( this->m_size.y / 2 ) );

		hack::game::renderer::filled_circle ( draw_pos , this->m_size.x * factor , 180 , c_color ( 43 , 43 , 43 , 255 ) );
	}

	if ( this->m_home_obj->m_time_in_anim > 0.0f )
		this->m_home_obj->m_time_in_anim -= globals::frametime;

	this->m_home_obj->m_time_in_anim = std::clamp ( this->m_home_obj->m_time_in_anim , 0.f , 2.f );

	/* do layer drawing */
	if ( !layer_queue.empty ( ) ) {
		for ( const auto & element : layer_queue ) {
			switch ( element.m_type ) {
				case utils::e_draw_types::DRAW_FILLED:
					hack::game::renderer::filled_rectangle ( element.m_pos , element.m_size , element.m_acc );
					break;
				case utils::e_draw_types::DRAW_RECT:
					hack::game::renderer::filled_rectangle ( c_vector2 ( element.m_pos.x , element.m_pos.y ) , c_vector2 ( element.m_size.x , element.m_size.y ) , element.m_acc );
					break;
				case utils::e_draw_types::DRAW_TEXTURE:
					hack::game::renderer::image ( element.m_pos , element.m_size , element.m_wrapper , element.m_scale );
					break;
				case utils::e_draw_types::DRAW_GRADIENT:
					hack::game::renderer::gradient_rectangle ( element.m_pos , element.m_size , element.m_acc , element.m_sec , !element.m_horizontal );
					break;
				case utils::e_draw_types::DRAW_TEXT:
					hack::game::renderer::text ( element.m_pos , element.m_acc , element.m_text , element.m_font , element.m_centered );
					break;
				case utils::e_draw_types::UTIL_SETCLIPPING:
					hack::game::renderer::start_clipping ( element.m_pos , element.m_size );
					break;
			}
		}
	}

	/* empty queue for other windows to use */
	layer_queue.clear ( );

	/* undo clipping */
	hack::game::renderer::end_clipping ( this->m_old_viewport );
}
