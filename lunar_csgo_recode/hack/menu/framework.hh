#pragma once

#ifndef SWOOPUI_DEF
#define SWOOPUI_DEF

#include "../game/interfaces.hh"
#include "../game/renderer.hh"

#include "config.hh"

namespace hack::menu {
	/* replace these values in case you want to theme the ui! */
	namespace config {
		inline c_color accent_color = c_color ( 255 , 168 , 250 ) , secondary_color = c_color ( 0 , 212 , 255 );
	}

	/* one global value at the moment lol */
	namespace globals {
		inline float frametime = 0.f , realtime = 0.f , framerate = 0.f;
	}

	namespace input {
		/* input */
		inline c_vector2 mouse_pos , prev_mouse_pos;
		inline bool key_states [ 256 ] , prev_key_states [ 256 ];

		static bool grab_input ( std::string window_name ) {
			if ( window_name.empty ( ) )
				return false;

			for ( auto i = 1; i < 256; i++ ) {
				prev_key_states [ i ] = key_states [ i ];
				key_states [ i ] = !!GetAsyncKeyState ( i ); /* lol */
			}

			POINT p;
			GetCursorPos ( &p );
			ScreenToClient ( FindWindowA ( nullptr , window_name.data ( ) ) , &p );

			prev_mouse_pos = mouse_pos;
			mouse_pos = c_vector2 ( p.x , p.y );

			return true;
		}

		static bool key_pressed ( const int & key ) { return key_states [ key ] && !prev_key_states [ key ]; }
		static bool key_released ( const int & key ) { return !key_states [ key ] && prev_key_states [ key ]; }
		static bool key_down ( const int & key ) { return key_states [ key ]; }
		static bool mouse_in_region ( const int & x , const int & y , const int & w , const int & h ) { return ( mouse_pos.x > x && mouse_pos.y > y && mouse_pos.x < w + x && mouse_pos.y < h + y ); }
	}

	namespace utils {
		static float ease_in_quart ( float value ) { return std::pow ( value , 4 ); }
		static float ease_out_quart ( float value ) { return std::fabsf ( 1 - std::pow ( 1 - value , 4 ) ); }

		enum class e_draw_types : char {
			DRAW_FILLED = 0 ,
			DRAW_RECT ,
			DRAW_GRADIENT ,
			DRAW_TEXT ,
			DRAW_TEXTURE ,
			UTIL_SETCLIPPING
		};

		struct draw_call_t {
			std::string m_text;
			ID3DXFont * m_font;
			c_vector2 m_pos , m_size;
			c_color m_acc , m_sec;
			e_draw_types m_type;
			bool m_horizontal , m_centered;
			float m_scale;
			hack::game::renderer::texture_wrapper_t m_wrapper;

			draw_call_t ( const int & x , const int & y , const int & w , const int & h , const c_color & col ) : m_pos ( x , y ) , m_size ( w , h ) , m_acc ( col ) , m_type ( e_draw_types::DRAW_RECT ) { }
			draw_call_t ( const c_vector2 & pos , const c_vector2 & size , const c_color & col ) : m_pos ( pos ) , m_size ( size ) , m_acc ( col ) , m_type ( e_draw_types::DRAW_FILLED ) { }
			draw_call_t ( const c_vector2 & pos , const c_vector2 & size , const c_color & col , const c_color & sec , bool horizontal ) : m_pos ( pos ) , m_size ( size ) , m_acc ( col ) , m_sec ( sec ) , m_type ( e_draw_types::DRAW_GRADIENT ) , m_horizontal ( horizontal ) { }
			draw_call_t ( const c_vector2 & pos , bool centered , const std::string & text , ID3DXFont * font , const c_color & col ) : m_pos ( pos ) , m_acc ( col ) , m_type ( e_draw_types::DRAW_TEXT ) , m_centered ( centered ) , m_text ( text ) , m_font ( font ) { }
			draw_call_t ( const c_vector2 & pos , const c_vector2 & size ) : m_pos ( pos ) , m_size ( size ) , m_type ( e_draw_types::UTIL_SETCLIPPING ) { }
			draw_call_t ( const c_vector2 & pos , const c_vector2 & size , const hack::game::renderer::texture_wrapper_t & wrapper , float scale = 1.0f ) : m_pos ( pos ) , m_size ( size ) , m_wrapper ( wrapper ) , m_scale ( scale ) , m_type ( e_draw_types::DRAW_TEXTURE ) { }
		};

		const static std::string keys_list [ ] {
			"none", "m1", "m2", "break", "m3", "m4", "m5",
			"error", "back", "tab", "error", "error", "error", "enter", "error", "error", "shift",
			"ctrl", "alt", "pause", "caps", "error", "error", "error", "error", "error", "error",
			"error", "error", "error", "error", "error", "space", "pg up", "pg down", "end", "home", "left",
			"up", "right", "down", "error", "print", "error", "pscrn", "ins", "del", "error", "0", "1",
			"2", "3", "4", "5", "6", "7", "8", "9", "error", "error", "error", "error", "error", "error",
			"error", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
			"v", "w", "x", "y", "z", "lwin", "rwin", "error", "error", "error", "num0", "num1",
			"num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9", "*", "+", "_", "-", ".", "/", "f1", "f2", "f3",
			"f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21",
			"f22", "f23", "f24", "error", "error", "error", "error", "error", "error", "error", "error",
			"nlock", "slock", "error", "error", "error", "error", "error", "error", "error",
			"error", "error", "error", "error", "error", "error", "error", "lshift", "rshift", "lctrl",
			"rctrl", "lmenu", "rmenu", "error", "error", "error", "error", "error", "error", "error",
			"error", "error", "error", "error", "error", "stop", "play", "error", "error",
			"error", "error", "error", "error", ";", "+", ",", "-", ".", "/?", "~", "error", "error",
			"error", "error", "error", "error", "error", "error", "error", "error", "error",
			"error", "error", "error", "error", "error", "error", "error", "error", "error",
			"error", "error", "error", "error", "error", "error", "[{", "\\|", "}]", "'\"", "error",
			"error", "error", "error", "error", "error", "error", "error", "error", "error",
			"error", "error", "error", "error", "error", "error", "error", "error", "error",
			"error", "error", "error", "error", "error", "error", "error", "error", "error",
			"error", "error"
		};

		struct key_code_info {
			key_code_info ( int a , char b , char c ) : vk ( a ) , regular ( b ) , shift ( c ) { };

			int vk;

			char regular;
			char shift;
		};

		inline std::array<key_code_info , 22> special_characters = {
				key_code_info ( 48,  '0',  ')' ),
				key_code_info ( 49,  '1',  '!' ),
				key_code_info ( 50,  '2',  '@' ),
				key_code_info ( 51,  '3',  '#' ),
				key_code_info ( 52,  '4',  '$' ),
				key_code_info ( 53,  '5',  '%' ),
				key_code_info ( 54,  '6',  '^' ),
				key_code_info ( 55,  '7',  '&' ),
				key_code_info ( 56,  '8',  '*' ),
				key_code_info ( 57,  '9',  '(' ),
				key_code_info ( 32,  ' ',  ' ' ),
				key_code_info ( 192, '`',  '~' ),
				key_code_info ( 189, '-',  '_' ),
				key_code_info ( 187, '=',  '+' ),
				key_code_info ( 219, '[',  '(' ),
				key_code_info ( 220, '\\', '|' ),
				key_code_info ( 221, ']',  ')' ),
				key_code_info ( 186, ';',  ':' ),
				key_code_info ( 222, '\'', '"' ),
				key_code_info ( 188, ',',  '<' ),
				key_code_info ( 190, '.',  '>' ),
				key_code_info ( 191, '/',  '?' )
		};
	}

	namespace controls {
		/* hash of pop-up control blocking input */
		inline std::uint32_t m_hash = 0;

		/* scroll factor */
		inline int scroll_factor = 0;

		/* for drawing over other elements */
		inline std::vector<utils::draw_call_t> layer_queue;

		/* virtual clipboard for color (TODO) */
		inline c_color clipboard_color = c_color ( 25 , 255 , 25 );

		/* abstract class used by children of controls ( color pickers , keybinds , buttons etc. ) */
		class c_child {
		public:
			/* members */
			std::uint32_t m_value_hash;

			/* methods */
			virtual void think ( const c_vector2 & pos ) { }
			virtual void draw ( const c_vector2 & pos ) { }
		};

		/* keybind child control */
		class c_keybind : public c_child {
		private:
			float m_animtime;
			bool m_popup;
		public:
			c_keybind ( std::uint32_t hash , keybind_t init_val ) {
				this->m_value_hash = hash;
				this->m_animtime = 0.f;
				this->m_popup = false;

				CONFIG_ADD ( keybind_t , hash , init_val );
			}

			void think ( const c_vector2 & pos ) override;
			void draw ( const c_vector2 & pos ) override;
		};

		/* color picker child control */
		class c_colorpicker : public c_child {
		private:
			float m_animtime , m_hue = 0.f , m_transparency = 1.f;
			c_vector2 m_colpos , m_huepos , m_transpos;
		public:
			c_colorpicker ( std::uint32_t hash , c_color init_val ) {
				this->m_value_hash = hash;
				this->m_animtime = 0.f;

				CONFIG_ADD ( c_color , hash , init_val );
			}

			void think ( const c_vector2 & pos ) override;
			void draw ( const c_vector2 & pos ) override;
		};

		/* abstract class used by controls */
		class c_control {
		public:
			/* members */
			std::string m_name;
			std::uint32_t m_value_hash;
			std::float_t m_last_click_animtime , m_last_hover_animtime;
			c_vector2 m_position , m_size;
			bool m_hovered , m_clicked;

			/* list of children the control contains */
			std::vector< std::shared_ptr <c_child> > m_children;

			/* methods */
			virtual void init ( const c_vector2 & pos , const c_vector2 & size ) { }
			virtual void think ( const c_vector2 & pos ) { }
			virtual void draw ( c_vector2 & pos ) { }
			void bind ( std::shared_ptr<c_child> child ) { this->m_children.push_back ( std::move ( child ) ); }
		};

		/* label control */
		class c_label : public c_control {
		public:
			/* constructor */
			c_label ( std::string_view name ) {
				this->m_name = name;
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override {
				this->m_size.x = ( size.x - 44 );
			}

			void think ( const c_vector2 & pos ) override {
				if ( m_hash && m_hash != this->m_value_hash ) {
					/* handle children of our element and return */
					c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

					for ( auto & child : this->m_children ) {
						child->think ( cursor );
						cursor.x -= 35;
					}
					return;
				}

				/* handle children of our element */
				c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

				for ( auto & child : this->m_children ) {
					child->think ( cursor );
					cursor.x -= 35;
				}
			}

			void draw ( c_vector2 & pos ) override {
				const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

				hack::game::renderer::text ( pos , c_color ( 255 , 255 , 255 ) , this->m_name , hack::game::renderer::fonts::menu_font );

				pos.y += text_size.y + 7;
			}
		};

		/* label reference control */
		class c_labelref : public c_control {
		private:
			std::string * m_ptr;
		public:
			/* constructor */
			c_labelref ( std::string * ptr ) {
				this->m_ptr = ptr;
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override {
				this->m_size.x = ( size.x - 44 );
			}

			void think ( const c_vector2 & pos ) override {
				if ( m_hash && m_hash != this->m_value_hash ) {
					/* handle children of our element and return */
					c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

					for ( auto & child : this->m_children ) {
						child->think ( cursor );
						cursor.x -= 35;
					}
					return;
				}

				/* handle children of our element */
				c_vector2 cursor ( pos.x + this->m_size.x - 33 , pos.y );

				for ( auto & child : this->m_children ) {
					child->think ( cursor );
					cursor.x -= 35;
				}
			}

			void draw ( c_vector2 & pos ) override {
				const auto text_size = hack::game::renderer::text_position ( this->m_ptr->data ( ) , hack::game::renderer::fonts::menu_font );

				hack::game::renderer::text ( pos , c_color ( 255 , 255 , 255 ) , this->m_ptr->data ( ) , hack::game::renderer::fonts::menu_font );

				pos.y += text_size.y + 7;
			}
		};

		/* checkbox control */
		class c_checkbox : public c_control {
		public:
			/* constructor */
			c_checkbox ( std::string_view name , std::uint32_t hash , bool init_val ) {
				this->m_name = name;
				this->m_value_hash = hash;
				this->m_hovered = false;
				this->m_clicked = false;
				this->m_last_click_animtime = 0.f;

				CONFIG_ADD ( bool , hash , init_val );
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override;
			void think ( const c_vector2 & pos ) override;
			void draw ( c_vector2 & pos ) override;
		};

		/* slider control */
		class c_slider : public c_control {
			int m_min , m_max;
		public:
			/* constructor */
			c_slider ( std::string_view name , std::uint32_t hash , int init_val , int min = 0 , int max = 100 ) {
				this->m_name = name;
				this->m_value_hash = hash;
				this->m_hovered = false;
				this->m_clicked = false;
				this->m_last_click_animtime = 0.f;
				this->m_min = min;
				this->m_max = max;

				CONFIG_ADD ( int , hash , init_val );
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override;
			void think ( const c_vector2 & pos ) override;
			void draw ( c_vector2 & pos ) override;
		};

		/* textedit control - unimplemented in config system therefore takes pointer to string */
		class c_textedit : public c_control {
		private:
			std::string * m_ptr;
			bool m_is_password;
		public:
			/* constructor */
			c_textedit ( std::string_view name , std::string * pointer , bool is_password = false ) {
				this->m_name = name;
				this->m_hovered = false;
				this->m_clicked = false;
				this->m_last_click_animtime = 0.f;
				this->m_ptr = pointer;
				this->m_is_password = is_password;
				this->m_value_hash = fnv1a_rt ( this->m_name.data ( ) );
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override {
				this->m_size.x = ( size.x - 44 );
			}

			void think ( const c_vector2 & pos ) override {
				if ( m_hash && m_hash != this->m_value_hash )
					return;

				const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

				if ( input::key_released ( 1 ) && input::mouse_in_region ( pos.x , pos.y + text_size.y + 4 , this->m_size.x + 2 , text_size.y + 2 ) ) {
					m_hash = this->m_value_hash;
				}

				if ( m_hash != this->m_value_hash )
					return;

				if ( input::key_released ( 1 ) && !input::mouse_in_region ( pos.x , pos.y + text_size.y + 4 , this->m_size.x + 2 , text_size.y + 2 ) ) {
					m_hash = 0;
				}

				/* for erasing */
				if ( input::key_released ( 8 ) && this->m_ptr->length ( ) )
					this->m_ptr->erase ( this->m_ptr->length ( ) - 1 );

				/* stolen from my good friend mr zgui */
				for ( int i = 32; i <= 222; i++ ) {
					if ( ( i > 32 && i < 48 ) || ( i > 57 && i < 65 ) || ( i > 90 && i < 186 ) )
						continue;

					if ( i > 57 && i <= 90 ) {
						if ( input::key_released ( i ) )
							*this->m_ptr += ( input::key_down ( VK_SHIFT ) ? static_cast< char >( i ) : static_cast< char >( i + 32 ) );
					} else {
						if ( input::key_released ( i ) ) {
							for ( int j = 0; j < sizeof ( utils::special_characters ); j++ ) {
								if ( utils::special_characters [ j ].vk == i ) {
									*this->m_ptr += input::key_down ( VK_SHIFT ) ? utils::special_characters [ j ].shift : utils::special_characters [ j ].regular;

									// if ( i == 0x38 && !input::key_down ( VK_SHIFT ) && this->m_ptr->length ( ) )
									// 	this->m_ptr->erase ( this->m_ptr->length ( ) - 1 );
								}
							}
						}
					}
				}
			}

			void draw ( c_vector2 & pos ) override {
				auto & value = CONFIG_GET ( int , this->m_value_hash );

				const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

				hack::game::renderer::text ( pos , c_color ( 255 , 255 , 255 ) , this->m_name.data ( ) , hack::game::renderer::fonts::menu_font );

				pos.y += text_size.y + 4;

				hack::game::renderer::filled_rectangle ( pos , c_vector2 ( this->m_size.x , text_size.y + 2 ) , c_color ( 54 , 54 , 54 , 130 ) );

				/* do clipping */
				const auto old_viewport = hack::game::renderer::start_clipping ( pos , c_vector2 ( this->m_size.x , text_size.y + 2 ) );

				const auto & hide_pwd = CONFIG_GET ( bool , fnv1a ( "login_showpass" ) );

				std::string ss;
				if ( this->m_is_password && !hide_pwd )
					for ( auto i = 0; i < this->m_ptr->size ( ); i++ )
						ss += STR ( "*" );
				else ss = this->m_ptr->data ( );

				if ( std::fmodf ( globals::realtime , 0.5f ) > 0.25f && m_hash == this->m_value_hash )
					ss += STR ( "_" );

				/* draw text */
				hack::game::renderer::text ( pos + 1 , c_color ( 255 , 255 , 255 ) , ss.data ( ) , hack::game::renderer::fonts::menu_font , false );

				/* undo clipping */
				hack::game::renderer::end_clipping ( old_viewport );

				pos.y += text_size.y + 8;
			}
		};

		/* button control - unimplemented in config system... obviously */
		class c_button : public c_control {
		private:
			void ( *m_callback )( );
		public:
			/* constructor */
			c_button ( std::string_view name , void ( *argument ) ( ) , bool is_password = false ) {
				this->m_name = name;
				this->m_hovered = false;
				this->m_clicked = false;
				this->m_last_click_animtime = 0.f;
				this->m_callback = argument;
				this->m_value_hash = fnv1a_rt ( this->m_name.data ( ) );
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override {
				this->m_size.x = ( size.x - 44 );
			}

			void think ( const c_vector2 & pos ) override {
				if ( m_hash && m_hash != this->m_value_hash )
					return;

				const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

				if ( input::key_released ( 1 ) && input::mouse_in_region ( pos.x , pos.y /* + text_size.y + 4 */ , this->m_size.x + 2 , text_size.y + 2 ) ) {
					this->m_callback ( );
				}
			}

			void draw ( c_vector2 & pos ) override {
				auto & value = CONFIG_GET ( int , this->m_value_hash );

				const auto text_size = hack::game::renderer::text_position ( this->m_name , hack::game::renderer::fonts::menu_font );

				// hack::game::renderer::text ( pos , c_color ( 255 , 255 , 255 ) , this->m_name.data ( ) , hack::game::renderer::fonts::menu_font );
				// 
				// pos.y += text_size.y + 4;

				hack::game::renderer::filled_rectangle ( pos , c_vector2 ( this->m_size.x , text_size.y + 2 ) , c_color ( 54 , 54 , 54 , 130 ) );

				/* do clipping */
				const auto old_viewport = hack::game::renderer::start_clipping ( pos , c_vector2 ( this->m_size.x , text_size.y + 2 ) );

				/* draw text */
				hack::game::renderer::text ( c_vector2 ( pos.x + 1 + ( m_size.x / 2 ) , pos.y + 1 ) , c_color ( 255 , 255 , 255 ) , this->m_name.data ( ) , hack::game::renderer::fonts::menu_font , true );

				/* undo clipping */
				hack::game::renderer::end_clipping ( old_viewport );

				pos.y += text_size.y + 8;
			}
		};

		/* combobox control */
		class c_combobox : public c_control {
			float m_animtime = 0.f , m_scroll_time = 0.f;
			int m_scroll = 0;
			std::vector<std::string> m_members;
		public:
			/* constructor */
			c_combobox ( std::string_view name , std::uint32_t hash , std::vector<std::string> members ) {
				this->m_name = name;
				this->m_value_hash = hash;
				this->m_hovered = false;
				this->m_clicked = false;
				this->m_last_click_animtime = 0.f;
				this->m_members = members;

				CONFIG_ADD ( int , hash , 0 );
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override;
			void think ( const c_vector2 & pos ) override;
			void draw ( c_vector2 & pos ) override;
		};

		/* multi combobox control */
		class c_multicombobox : public c_control {
			float m_animtime = 0.f , m_scroll_time = 0.f;
			int m_scroll = 0;
			std::vector<std::string> m_members;
			/* i have a reason for this okay. it makes scripting for the hack much easier for users. */
			std::vector<std::uint32_t> m_hashes;
		public:
			/* constructor */
			c_multicombobox ( std::string_view name , std::vector < std::uint32_t > hash , std::vector < std::string > members ) {
				this->m_name = name;
				this->m_hovered = false;
				this->m_clicked = false;
				this->m_last_click_animtime = 0.f;
				this->m_members = members;
				this->m_hashes = hash;
				this->m_value_hash = hash.at ( 0 );

				for ( const auto & hashed : this->m_hashes )
					CONFIG_ADD ( bool , hashed , false );
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) override;
			void think ( const c_vector2 & pos ) override;
			void draw ( c_vector2 & pos ) override;
		};

		/* groupbox control */
		class c_groupbox {
		public:
			/* members */
			std::string m_name , m_tooltip;
			c_vector2 m_position , m_size;
			bool m_hovered , m_clicked;

			/* list of members the groupbox contains */
			std::vector< std::shared_ptr <c_control> > m_members;

			/* constructor */
			c_groupbox ( std::string_view name , std::string_view tip ) {
				this->m_name = name;
				this->m_tooltip = tip;
				this->m_hovered = false;
				this->m_clicked = false;
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size );
			void bind ( std::shared_ptr < c_control > control );
			void think ( const c_vector2 & pos ) { }
			void draw ( const c_vector2 & pos );
		};

		inline char queued_tab = -1;

		class c_subtab {
		public:
			/* members */
			std::string m_name;
			std::vector< std::shared_ptr <c_groupbox> > m_groupboxes;

			/* constructor */
			c_subtab ( std::string_view name ) {
				this->m_name = name;
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size ) {
				for ( auto & groupbox : this->m_groupboxes )
					groupbox->init ( pos , size );
			}

			void bind ( std::shared_ptr <c_groupbox> groupbox ) {
				this->m_groupboxes.push_back ( std::move ( groupbox ) );
			}

			void think ( const c_vector2 & pos ) {
				for ( auto & groupbox : this->m_groupboxes )
					groupbox->think ( pos );
			}

			void draw ( const c_vector2 & pos ) {
				for ( auto & groupbox : this->m_groupboxes )
					groupbox->draw ( pos );
			}
		};

		class c_form {
		public:
			/* members */
			std::string m_name , m_drawn_name;
			c_vector2 m_position , m_size;
			char m_selected_tab;
			std::vector<std::shared_ptr <c_subtab>> m_subtabs;

			/* constructor */
			c_form ( std::string_view name , std::string_view drawn_name ) {
				this->m_name = name;
				this->m_drawn_name = drawn_name;
				this->m_selected_tab = 0;
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size );
			void bind ( std::shared_ptr < c_subtab > subtab );
			void think ( const c_vector2 & pos );
			void draw ( const c_vector2 & pos );
		};

		class c_home {
		public:
			/* members */
			std::string m_name;
			c_vector2 m_position , m_size;
			char m_selected_tab , m_queued_tab;
			float m_time_in_anim;

			std::vector<std::shared_ptr<c_form>> m_tabs;

			/* constructors */
			c_home ( ) {
				this->m_name = " ";
				this->m_position = this->m_size = c_vector2 ( );
				this->m_selected_tab = 0;
				this->m_queued_tab = -1;
				this->m_time_in_anim = 0.f;
			}

			c_home ( std::string_view name ) {
				this->m_name = name;
			}

			/* methods */
			void init ( const c_vector2 & pos , const c_vector2 & size );
			void bind ( std::shared_ptr<c_form> tab );
			virtual void think ( const c_vector2 & pos );
			virtual void draw ( const c_vector2 & pos );
		};

		class c_custom_home : public c_home {
		public:
			c_custom_home ( std::string_view name = " " ) {
				this->m_name = name;
			}

			void think ( const c_vector2 & pos ) override {
			}

			void draw ( const c_vector2 & pos ) override {
			}
		};

		class c_window {
		public:
			/* members */
			std::string_view m_name;
			c_vector2 m_position , m_size;
			std::shared_ptr < c_home > m_home_obj;
			bool m_on_top , m_is_dragging , m_on_key;
			D3DVIEWPORT9 m_old_viewport;

			/* constructor */
			c_window ( std::string_view name , bool on_key ) {
				this->m_name = name;
				this->m_on_key = on_key;
			}

			/* methods */
			void init ( const c_vector2 & size , const c_vector2 & position = c_vector2 ( 100 , 100 ) );
			void bind ( std::shared_ptr<c_home> form );
			void think ( );
			void draw ( );
		};
	}

	/* the functions you need to touch */
	namespace impl {
		inline std::vector<std::shared_ptr<hack::menu::controls::c_window>> window_list;
		inline bool menu_opened = false;

		void init ( );
		void run_menu ( );
	}
}

#endif /* SWOOPUI_DEF */