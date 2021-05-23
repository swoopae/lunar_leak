#pragma once

#include "../math/vector.hh"

#include "../../shared/memory/memory.hh"

namespace sdk::valve {
	enum {
		TE_BEAMPOINTS = 0 ,
		TE_SPRITE = 1 ,
		TE_BEAMDISK = 2 ,
		TE_BEAMCYLINDER = 3 ,
		TE_BEAMFOLLOW = 4 ,
		TE_BEAMRING = 5 ,
		TE_bmsPLINE = 6 ,
		TE_BEAMRINGPOINT = 7 ,
		TE_BEAMLASER = 8 ,
		TE_BEAMTESLA = 9
	};

	enum {
		FBEAM_STARTENTITY = 0x00000001 ,
		FBEAM_ENDENTITY = 0x00000002 ,
		FBEAM_FADEIN = 0x00000004 ,
		FBEAM_FADEOUT = 0x00000008 ,
		FBEAM_SINENOISE = 0x00000010 ,
		FBEAM_SOLID = 0x00000020 ,
		FBEAM_SHADEIN = 0x00000040 ,
		FBEAM_SHADEOUT = 0x00000080 ,
		FBEAM_ONLYNOISEONCE = 0x00000100 ,		// Only calculate our noise once
		FBEAM_NOTILE = 0x00000200 ,
		FBEAM_USE_HITBOXES = 0x00000400 ,		// Attachment indices represent hitbox indices instead when this is set.
		FBEAM_STARTVISIBLE = 0x00000800 ,		// Has this client actually seen this beam's start entity yet?
		FBEAM_ENDVISIBLE = 0x00001000 ,		// Has this client actually seen this beam's end entity yet?
		FBEAM_ISACTIVE = 0x00002000 ,
		FBEAM_FOREVER = 0x00004000 ,
		FBEAM_HALOBEAM = 0x00008000 ,		// When drawing a beam with a halo, don't ignore the segments and endwidth
		FBEAM_REVERSED = 0x00010000 ,
		NUM_BEAM_FLAGS = 17
	};

	struct beam_trail_t {
		beam_trail_t * next;
		float die;
		c_vector3 org;
		c_vector3 vel;
	};

	class c_beam {
	public:
		pad ( 224 ) /* 0 */
			float m_flred; /* 224 */
		float m_flgreen; /* 228 */
		float m_flblue; /* 232 */
		pad ( 8 ) /* 236 */
			float m_flframerate; /* 244 */
		int m_nstartframe; /* 248 */
	};

	struct beam_info_t {
		int	m_ntype; /* 0 */
		void * m_pstartent; /* 4 */
		int	m_nstartattachment; /* 8 */
		void * m_pendent; /* 12 */
		int	m_nendattachment; /* 16 */
		c_vector3 m_vecstart; /* 20 */
		c_vector3 m_vecend; /* 32 */
		int	m_nmodelindex; /* 44 */
		const char * m_pszmodelname; /* 48 */
		int	m_nhaloindex; /* 52 */
		const char * m_pszhaloname; /* 56 */
		float m_flhaloscale; /* 60 */
		float m_fllife; /* 64 */
		float m_flwidth; /* 68 */
		float m_flendwidth; /* 72 */
		float m_flfadelength; /* 76 */
		float m_flamplitude; /* 80 */
		float m_flbrightness; /* 84 */
		float m_flspeed; /* 88 */
		int	m_nstartframe; /* 92 */
		float m_flframerate; /* 96 */
		float m_flred; /* 100 */
		float m_flgreen; /* 104 */
		float m_flblue; /* 108 */
		bool m_brenderable; /* 112 */
		int m_nsegments; /* 113 */
		int	m_nflags; /* 117 */
		c_vector3 m_veccenter; /* 121 */
		float m_flstartradius; /* 133 */
		float m_flendradius; /* 137 */

		beam_info_t ( ) {
			m_ntype = TE_BEAMPOINTS;
			m_nsegments = -1;
			m_pszmodelname = 0;
			m_pszhaloname = 0;
			m_nmodelindex = -1;
			m_nhaloindex = -1;
			m_brenderable = true;
			m_nflags = 0;
		}
	};

	class c_view_render_beams {
	public:
		void draw_beam ( sdk::valve::c_beam * beam ) {
			/* sig: client_panorama , 55 8B EC 83 E4 F8 83 EC 28 89 4C 24 0C */
			/* xref: "CViewRenderBeams::DrawBeam:  Unknown beam type %i\n" */
			using fn = void ( __thiscall * )( void * , c_beam * );
			shared::memory::v_func< fn > ( this , 4 )( this , beam );
		}

		sdk::valve::c_beam * create_beam ( sdk::valve::beam_info_t & beam ) {
			/* sig: 55 8B EC 56 8B 75 08 0F 57 C9 57 8B F9 F3 0F 10 46 ? 0F 2E C1 9F F6 C4 44 7B 17 */
			/* no xref */
			using fn = c_beam * ( __thiscall * )( void * , beam_info_t & );
			return shared::memory::v_func< fn > ( this , 12 )( this , beam );
		}
	};
}