#include "../handler.hh"

#include "../../game/renderer.hh"
#include "../../menu/framework.hh"

#include "../../modules/misc/others.hh"
#include "../../modules/visuals/esp.hh"
#include "../../modules/visuals/misc.hh"
#include "../../modules/rage/antiaim.hh"
#include "../../modules/rage/tickbase.hh"

#include <intrin.h>
#include <ctime>
#include "../../modules/misc/movement.hh"

long __fastcall hack::hooks::endscene_hk ( x86reg , IDirect3DDevice9 * device ) {
	static std::uintptr_t gameoverlay_address = 0; // , normal_address = 0;

	const auto ret_add = std::uintptr_t ( _ReturnAddress ( ) );

	if ( !gameoverlay_address /* || !normal_address */ ) {
		MEMORY_BASIC_INFORMATION info;
		VirtualQuery ( _ReturnAddress ( ) , &info , sizeof ( MEMORY_BASIC_INFORMATION ) );

		char mod [ MAX_PATH ];
		GetModuleFileNameA ( ( HMODULE ) info.AllocationBase , mod , MAX_PATH );

		if ( strstr ( mod , STR ( "gameoverlay" ) ) )
			gameoverlay_address = ret_add;
		// else normal_address = ret_add;
	}

	/* if streamproof */
	if ( gameoverlay_address != ret_add )
		return originals::o_endscene ( x86regout , device );
	/* else use normal address */

	IDirect3DStateBlock9 * pixel_state = nullptr;
	IDirect3DVertexDeclaration9 * vertex_declaration = nullptr;
	IDirect3DVertexShader9 * vertex_shader = nullptr;

	device->CreateStateBlock ( D3DSBT_PIXELSTATE , &pixel_state );
	device->GetVertexDeclaration ( &vertex_declaration );
	device->GetVertexShader ( &vertex_shader );

	if ( !hack::game::renderer::init )
		if ( hack::game::renderer::do_init ( device ) )
			hack::game::renderer::init = true;

	hack::game::renderer::set_states ( );

	/* util func */
	const auto clock_to_seconds = [ & ] ( std::clock_t ticks ) { return ( ticks / static_cast< float >( CLOCKS_PER_SEC ) ); };

	/* calculate frametime */
	static std::clock_t current_time = 0 , old_time = 0;
	old_time = current_time;
	current_time = std::clock ( );

	hack::menu::globals::frametime = clock_to_seconds ( current_time - old_time );
	hack::menu::globals::realtime += hack::menu::globals::frametime;

	/* lol ghetto garbage */
	static int frames_drawn = 0;
	static float last_fps_time = 0.f;
	frames_drawn++;

	if ( hack::menu::globals::realtime > last_fps_time + 1.f ) {
		last_fps_time = hack::menu::globals::realtime;
		hack::menu::globals::framerate = frames_drawn;
		frames_drawn = 0;
	}

	/* do draw hacks */
	hack::modules::visuals::esp::in_endscene ( );
	hack::modules::visuals::misc::in_endscene ( );

	/* draw our shit here */
	hack::menu::impl::run_menu ( );

	/* grab input shit here */
	hack::modules::misc::others::in_endscene ( );
	hack::modules::rage::antiaim::in_endscene ( );
	hack::modules::misc::movement::in_endscene ( );
	hack::modules::rage::tickbase::in_endscene ( );

	// hack::game::renderer::text ( c_vector2 ( 5 , 125 ) , c_color ( 255 , 255 , 255 ) , "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ" , hack::game::renderer::fonts::icon_font );

	/* block/allow input if we're in the menu */
	hack::game::interfaces::input_system->enable_input ( !hack::menu::impl::menu_opened );

	pixel_state->Apply ( );
	pixel_state->Release ( );
	device->SetVertexDeclaration ( vertex_declaration );
	device->SetVertexShader ( vertex_shader );

	return originals::o_endscene ( x86regout , device );
}