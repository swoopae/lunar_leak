#pragma once

#include "../math/matrix3x4.hh"

#include "../entity.hh"

#include "../../shared/memory/memory.hh"

#define	hitgroup_generic 0
#define	hitgroup_head 1
#define	hitgroup_chest 2
#define	hitgroup_stomach 3
#define hitgroup_leftarm 4
#define hitgroup_rightarm 5
#define hitgroup_leftleg 6
#define hitgroup_rightleg 7
#define hitgroup_gear 10

namespace sdk::valve {
	enum contents {
		contents_empty = 0x0 ,
		contents_solid = 0x1 ,
		contents_window = 0x2 ,
		contents_aux = 0x4 ,
		contents_grate = 0x8 ,
		contents_slime = 0x10 ,
		contents_water = 0x20 ,
		contents_blocklos = 0x40 ,
		contents_opaque = 0x80 ,
		contents_testfogvolume = 0x100 ,
		contents_unused = 0x200 ,
		contents_blocklight = 0x400 ,
		contents_team1 = 0x800 ,
		contents_team2 = 0x1000 ,
		contents_ignore_nodraw_opaque = 0x2000 ,
		contents_moveable = 0x4000 ,
		contents_areaportal = 0x8000 ,
		contents_playerclip = 0x10000 ,
		contents_monsterclip = 0x20000 ,
		contents_current0 = 0x40000 ,
		contents_current90 = 0x80000 ,
		contents_current180 = 0x100000 ,
		contents_current270 = 0x200000 ,
		contents_current_up = 0x400000 ,
		contents_current_down = 0x800000 ,
		contents_origin = 0x1000000 ,
		contents_monster = 0x2000000 ,
		contents_debris = 0x4000000 ,
		contents_detail = 0x8000000 ,
		contents_translucent = 0x10000000 ,
		contents_ladder = 0x20000000 ,
		contents_hitbox = 0x40000000 ,

		last_visible_contents = contents_opaque ,
		all_visible_contents = last_visible_contents | last_visible_contents - 1
	};

	enum surf {
		surf_light = 0x1 ,
		surf_sky2d = 0x2 ,
		surf_sky = 0x4 ,
		surf_warp = 0x8 ,
		surf_trans = 0x10 ,
		surf_noportal = 0x20 ,
		surf_trigger = 0x40 ,
		surf_nodraw = 0x80 ,
		surf_hint = 0x100 ,
		surf_skip = 0x200 ,
		surf_nolight = 0x400 ,
		surf_bumplight = 0x800 ,
		surf_noshadows = 0x1000 ,
		surf_nodecals = 0x2000 ,
		surf_nopaint = surf_nodecals ,
		surf_nochop = 0x4000 ,
		surf_hitbox = 0x8000
	};

	enum mask {
		mask_all = 0xFFFFFFFF ,
		mask_solid = ( contents_solid | contents_moveable | contents_window | contents_monster | contents_grate ) ,
		mask_playersolid = ( contents_solid | contents_moveable | contents_playerclip | contents_window | contents_monster | contents_grate ) ,
		mask_npcsolid = ( contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate ) ,
		mask_npcfluid = ( contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster ) ,
		mask_water = ( contents_water | contents_moveable | contents_slime ) ,
		mask_opaque = ( contents_water | contents_moveable | contents_opaque ) ,
		mask_opaque_npc = ( mask_opaque | contents_monster ) ,
		mask_blocklos = ( contents_solid | contents_moveable | contents_slime ) ,
		mask_blocklos_npc = ( mask_blocklos | contents_monster ) ,
		mask_visible = ( mask_opaque | contents_ignore_nodraw_opaque ) ,
		mask_visible_npc = ( mask_opaque_npc | contents_ignore_nodraw_opaque ) ,
		mask_shot = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_hitbox ) ,
		mask_shot_brushonly = ( contents_solid | contents_moveable | contents_window | contents_debris ) ,
		mask_shot_hull = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate ) ,
		mask_shot_player = ( mask_shot_hull | contents_hitbox ) ,
		mask_shot_portal = ( contents_solid | contents_moveable | contents_window | contents_monster ) ,
		mask_solid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_grate ) ,
		mask_playersolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_playerclip | contents_grate ) ,
		mask_npcsolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_monsterclip | contents_grate ) ,
		mask_npcworldstatic = ( contents_solid | contents_window | contents_monsterclip | contents_grate ) ,
		mask_npcworldstatic_fluid = ( contents_solid | contents_window | contents_monsterclip ) ,
		mask_splitareaportal = ( contents_water | contents_slime ) ,
		mask_current = ( contents_current0 | contents_current90 | contents_current180 | contents_current270 | contents_current_up | contents_current_down ) ,
		mask_deadsolid = ( contents_solid | contents_playerclip | contents_window | contents_grate )
	};

	class __declspec( align( 16 ) )c_vector_aligned : public c_vector3 {
	public:
		c_vector_aligned & operator=( const c_vector3 & oth ) {
			init ( oth.x , oth.y , oth.z );
			return *this;
		}

		float w;
	};

	struct ray_t {
		c_vector_aligned m_start; // starting point, centered within the extents
		c_vector_aligned m_delta; // direction + length of the ray
		c_vector_aligned m_startoffset; // add this to m_start to get the actual ray start
		c_vector_aligned m_extents; // describes an axis aligned box extruded along a ray

		const c_matrix3x4 * m_pworldaxistransform;

		bool m_isray; // are the extents zero?
		bool m_isswept; // is delta != 0?

		ray_t ( c_vector3 _start , c_vector3 _end ) {
			init ( _start , _end );
		}

		ray_t ( c_vector3 _start , c_vector3 _end , c_vector3 _mins , c_vector3 _maxs ) {
			init ( _start , _end , _mins , _maxs );
		}

		void init ( const c_vector3 & vecstart , const c_vector3 & vecend ) {
			m_delta = vecend - vecstart;

			m_isswept = ( m_delta.length_sqr ( ) != 0 );

			m_extents.x = m_extents.y = m_extents.z = 0.0f;
			m_isray = true;

			m_startoffset.x = m_startoffset.y = m_startoffset.z = 0.0f;

			m_start = vecstart;
		}

		void init ( c_vector3 & vecstart , c_vector3 & vecend , c_vector3 min , c_vector3 max ) {
			m_delta = vecend - vecstart;

			m_isswept = ( m_delta.length_sqr ( ) != 0 );

			m_extents.x = ( max.x - min.x );
			m_extents.y = ( max.y - min.y );
			m_extents.z = ( max.z - min.z );
			m_isray = false;

			m_startoffset.x = m_startoffset.y = m_startoffset.z = 0.0f;

			m_start = vecstart + ( ( max + min ) * 0.5f );
		}
	};

	struct csurface_t {
		const char * name;
		short surface_props;
		unsigned short flags;
	};

	struct cplane_t {
		c_vector3 normal;
		float dist;
		uint8_t type;
		uint8_t signbits;
		uint8_t pad [ 2 ];
	};

	class c_base_trace {
	public:
		c_vector3 startpos {};
		c_vector3 endpos {};
		cplane_t plane {};
		float flfraction {};
		int	contents {};
		unsigned short disp_flags {};
		bool allsolid {};
		bool startsolid {};

		c_base_trace ( ) { }
	};

	class c_trace : public c_base_trace {
	public:
		bool didhitworld ( ) const {
			return m_pent && m_pent->index ( ) == 0;
		}

		bool didhitnonworldentity ( ) const {
			return m_pent != NULL && !didhitworld ( );
		}

		bool didhit ( ) const {
			return flfraction == 1.f;
		}

		float fractionleftsolid {};
		csurface_t surface {};
		int hitgroup {};
		short physicsbone {};
		unsigned short world_surface_index {};
		sdk::c_entity * m_pent {};
		int hitbox {};
	};

	enum tracetype_t {
		trace_everything = 0 ,
		trace_world_only ,
		trace_entities_only ,
		trace_everything_filter_props
	};

	class c_trace_filter_parent {
	public:
		virtual bool shouldhitentity ( void * pentity , int contentsmask ) = 0;
		virtual tracetype_t gettracetype ( ) const = 0;
	};

	class c_trace_filter : public c_trace_filter_parent {
	public:
		explicit c_trace_filter ( void * entity , tracetype_t tracetype = trace_everything ) {
			pskip1 = entity;
		}

		bool shouldhitentity ( void * pentityhandle , int contentsmask ) {
			return ( pentityhandle != pskip1 );
		}

		tracetype_t gettracetype ( ) const {
			return trace_everything;
		}

		void * pskip1;
		char * cc_ignore = ( char * ) "";
	};

	class c_trace_filter_one_entity : public c_trace_filter_parent {
	public:

		explicit c_trace_filter_one_entity ( void * entity , tracetype_t tracetype = trace_everything ) {
			pentity = entity;
		}

		bool shouldhitentity ( void * pentityhandle , int contentsmask ) {
			return ( pentityhandle == pentity );
		}

		tracetype_t gettracetype ( ) const {
			return trace_everything;
		}

		void * pentity;
	};

	class c_trace_filter_one_entity_2 : public c_trace_filter_parent {
	public:
		explicit c_trace_filter_one_entity_2 ( void * entity , tracetype_t tracetype = trace_everything ) {
			pentity = entity;
		}

		bool shouldhitentity ( void * pentityhandle , int contentsmask ) {
			return ( pentityhandle == pentity );
		}

		tracetype_t gettracetype ( ) const {
			return trace_entities_only;
		}

		void * pentity;
	};

	class c_trace_filter_skip_two_entities : public c_trace_filter_parent {
	public:
		c_trace_filter_skip_two_entities ( void * ppassent1 , void * ppassent2 ) {
			passentity1 = ppassent1;
			passentity2 = ppassent2;
		}

		virtual bool shouldhitentity ( void * pentityhandle , int contentsmask ) {
			return !( pentityhandle == passentity1 || pentityhandle == passentity2 );
		}

		virtual tracetype_t gettracetype ( ) const {
			return trace_everything;
		}

		void * passentity1;
		void * passentity2;
	};

	class c_trace_entity : public c_trace_filter_parent {
	public:

		explicit c_trace_entity ( void * entity , tracetype_t tracetype = trace_everything ) {
			pskip1 = entity;
		}

		bool shouldhitentity ( void * pentityhandle , int contentsmask ) {
			return !( pentityhandle == pskip1 );
		}

		tracetype_t gettracetype ( ) const {
			return trace_entities_only;
		}

		void * pskip1;
	};

	class c_trace_entity_world : public c_trace_filter_parent {
	public:
		explicit c_trace_entity_world ( ) { }

		bool shouldhitentity ( void * pentityhandle , int contentsmask ) {
			return false;
		}

		tracetype_t gettracetype ( ) const {
			return trace_world_only;
		}
	};

	class c_trace_world_only : public c_trace_filter_parent {
	public:
		explicit c_trace_world_only ( void * entity , tracetype_t tracetype = trace_everything ) {
			pskip1 = entity;
		}

		bool shouldhitentity ( void * pentityhandle , int contentsmask ) {
			return false;
		}

		tracetype_t gettracetype ( ) const {
			return trace_everything;
		}

		void * pskip1;
	};

	class c_engine_trace {
	public:
		/* TODO: sig 55 8B EC 83 E4 F8 83 EC 24 53 56 8B 75 0C engine.dll rebuild and optimize */
		virtual int		get_point_contents ( const c_vector3 & vecabsposition , int contentsmask = mask_all , void ** ppentity = nullptr ) = 0;
		virtual int		get_point_contents_worldonly ( const c_vector3 & vecabsposition , int contentsmask = mask_all ) = 0;
		virtual int		get_point_contents_collideable ( void * pcollide , const c_vector3 & vecabsposition ) = 0;
		virtual void	clip_ray_to_entity ( const ray_t & ray , unsigned int fmask , sdk::c_entity * pent , c_trace * ptrace ) = 0;
		virtual void	clip_ray_to_collideable ( const ray_t & ray , unsigned int fmask , void * pcollide , c_trace * ptrace ) = 0;
		virtual void	trace_ray ( const ray_t & ray , unsigned int fmask , c_trace_filter_parent * ptracefilter , c_trace * ptrace ) = 0;

		void clip_ray_to_player ( c_vector3 src , c_vector3 end , const std::uint32_t & mask ,
								  sdk::c_entity * player , c_trace_filter * filter , c_trace * t ) {
			if ( filter && !filter->shouldhitentity ( player , mask ) )
				return;

			c_trace t_new {};
			ray_t r ( src , end );
			r.init ( src , end );

			clip_ray_to_entity ( r , mask , player , &t_new );

			if ( t_new.flfraction < t->flfraction )
				*t = t_new;
		}
	};
}