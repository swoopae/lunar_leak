#pragma once

#include "../math/matrix3x4.hh"

#include "../../shared/color/color.hh"
#include "../../shared/memory/memory.hh"

namespace sdk::valve {
	struct mdlrender_info_t {
		c_vector3 m_origin;
		c_vector3 m_angles;
		pad ( 4 );
		void * m_renderable;
		const void * m_model;
		c_matrix3x4 * m_model_to_world;
		c_matrix3x4 * m_lighting_offset;
		c_vector3 * m_lighting_origin;
		int m_flags;
		int m_entity_index;
		int m_skin;
		int m_body;
		int m_hitbox_set;
		std::uint16_t m_instance;
	};

	class c_renderview {
	public:
		void set_blend ( float blend ) {
			using setblend_fn = void ( __thiscall * )( void * , float );
			shared::memory::v_func< setblend_fn > ( this , 4 )( this , ( float ) blend / 255.0f );
		}

		void color_modulate ( c_color clr ) {
			using colormodulate_fn = void ( __thiscall * )( void * , float * );
			float clrf [ ] { ( float ) clr.r / 255.0f, ( float ) clr.g / 255.0f, ( float ) clr.b / 255.0f };
			shared::memory::v_func< colormodulate_fn > ( this , 6 )( this , clrf );
		}
	};
}