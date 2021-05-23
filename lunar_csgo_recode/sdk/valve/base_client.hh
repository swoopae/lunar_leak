#pragma once

#include "../math/vector.hh"
#include "../math/matrix3x4.hh"

#include "../../shared/memory/memory.hh"
#include "../../shared/color/color.hh"

namespace sdk::valve {
	struct recv_table_t;
	struct recv_prop_t;
	enum solid_type;

	class c_client_networkable;
	class c_client_renderable;
	class c_client_entity;
	class c_client_thinkable;
	class c_client_unknown;
	class c_client_alpha_property;
	class c_entity;

	enum class_id {
		cai_basenpc ,
		cak47 ,
		cbaseanimating ,
		cbaseanimatingoverlay ,
		cbaseattributableitem ,
		cbasebutton ,
		cbasecombatcharacter ,
		cbasecombatweapon ,
		cbasecsgrenade ,
		cbasecsgrenadeprojectile ,
		cbasedoor ,
		cbaseentity ,
		cbaseflex ,
		cbasegrenade ,
		cbaseparticleentity ,
		cbaseplayer ,
		cbasepropdoor ,
		cbaseteamobjectiveresource ,
		cbasetempentity ,
		cbasetoggle ,
		cbasetrigger ,
		cbaseviewmodel ,
		cbasevphysicstrigger ,
		cbaseweaponworldmodel ,
		cbeam ,
		cbeamspotlight ,
		cbonefollower ,
		cbrc4target ,
		cbreachcharge ,
		cbreachchargeprojectile ,
		cbreakableprop ,
		cbreakablesurface ,
		cbumpmine ,
		cbumpmineprojectile ,
		cc4 ,
		ccascadelight ,
		cchicken ,
		ccolorcorrection ,
		ccolorcorrectionvolume ,
		ccsgamerulesproxy ,
		ccsplayer ,
		ccsplayerresource ,
		ccsragdoll ,
		ccsteam ,
		cdangerzone ,
		cdangerzonecontroller ,
		cdeagle ,
		cdecoygrenade ,
		cdecoyprojectile ,
		cdrone ,
		cdronegun ,
		cdynamiclight ,
		cdynamicprop ,
		ceconentity ,
		ceconwearable ,
		cembers ,
		centitydissolve ,
		centityflame ,
		centityfreezing ,
		centityparticletrail ,
		cenvambientlight ,
		cenvdetailcontroller ,
		cenvdofcontroller ,
		cenvgascanister ,
		cenvparticlescript ,
		cenvprojectedtexture ,
		cenvquadraticbeam ,
		cenvscreeneffect ,
		cenvscreenoverlay ,
		cenvtonemapcontroller ,
		cenvwind ,
		cfeplayerdecal ,
		cfirecrackerblast ,
		cfiresmoke ,
		cfiretrail ,
		cfish ,
		cfists ,
		cflashbang ,
		cfogcontroller ,
		cfootstepcontrol ,
		cfunc_dust ,
		cfunc_lod ,
		cfuncareaportalwindow ,
		cfuncbrush ,
		cfuncconveyor ,
		cfuncladder ,
		cfuncmonitor ,
		cfuncmovelinear ,
		cfuncoccluder ,
		cfuncreflectiveglass ,
		cfuncrotating ,
		cfuncsmokevolume ,
		cfunctracktrain ,
		cgamerulesproxy ,
		cgrassburn ,
		chandletest ,
		chegrenade ,
		chostage ,
		chostagecarriableprop ,
		cincendiarygrenade ,
		cinferno ,
		cinfoladderdismount ,
		cinfomapregion ,
		cinfooverlayaccessor ,
		citem_healthshot ,
		citemcash ,
		citemdogtags ,
		cknife ,
		cknifegg ,
		clightglow ,
		cmaterialmodifycontrol ,
		cmelee ,
		cmolotovgrenade ,
		cmolotovprojectile ,
		cmoviedisplay ,
		cparadropchopper ,
		cparticlefire ,
		cparticleperformancemonitor ,
		cparticlesystem ,
		cphysbox ,
		cphysboxmultiplayer ,
		cphysicsprop ,
		cphysicspropmultiplayer ,
		cphysmagnet ,
		cphyspropammobox ,
		cphysproplootcrate ,
		cphyspropradarjammer ,
		cphyspropweaponupgrade ,
		cplantedc4 ,
		cplasma ,
		cplayerping ,
		cplayerresource ,
		cpointcamera ,
		cpointcommentarynode ,
		cpointworldtext ,
		cposecontroller ,
		cpostprocesscontroller ,
		cprecipitation ,
		cprecipitationblocker ,
		cpredictedviewmodel ,
		cprop_hallucination ,
		cpropcounter ,
		cpropdoorrotating ,
		cpropjeep ,
		cpropvehicledriveable ,
		cragdollmanager ,
		cragdollprop ,
		cragdollpropattached ,
		cropekeyframe ,
		cscar17 ,
		csceneentity ,
		csensorgrenade ,
		csensorgrenadeprojectile ,
		cshadowcontrol ,
		cslideshowdisplay ,
		csmokegrenade ,
		csmokegrenadeprojectile ,
		csmokestack ,
		csnowball ,
		csnowballpile ,
		csnowballprojectile ,
		cspatialentity ,
		cspotlightend ,
		csprite ,
		cspriteoriented ,
		cspritetrail ,
		cstatueprop ,
		csteamjet ,
		csun ,
		csunlightshadowcontrol ,
		csurvivalspawnchopper ,
		ctablet ,
		cteam ,
		cteamplayroundbasedrulesproxy ,
		ctearmorricochet ,
		ctebasebeam ,
		ctebeamentpoint ,
		ctebeaments ,
		ctebeamfollow ,
		ctebeamlaser ,
		ctebeampoints ,
		ctebeamring ,
		ctebeamringpoint ,
		ctebeamspline ,
		ctebloodsprite ,
		ctebloodstream ,
		ctebreakmodel ,
		ctebspdecal ,
		ctebubbles ,
		ctebubbletrail ,
		cteclientprojectile ,
		ctedecal ,
		ctedust ,
		ctedynamiclight ,
		cteeffectdispatch ,
		cteenergysplash ,
		cteexplosion ,
		ctefirebullets ,
		ctefizz ,
		ctefootprintdecal ,
		ctefoundryhelpers ,
		ctegaussexplosion ,
		cteglowsprite ,
		cteimpact ,
		ctekillplayerattachments ,
		ctelargefunnel ,
		ctemetalsparks ,
		ctemuzzleflash ,
		cteparticlesystem ,
		ctephysicsprop ,
		cteplantbomb ,
		cteplayeranimevent ,
		cteplayerdecal ,
		cteprojecteddecal ,
		cteradioicon ,
		cteshattersurface ,
		cteshowline ,
		ctesla ,
		ctesmoke ,
		ctesparks ,
		ctesprite ,
		ctespritespray ,
		ctest_proxytoggle_networkable ,
		ctesttraceline ,
		cteworlddecal ,
		ctriggerplayermovement ,
		ctriggersoundoperator ,
		cvguiscreen ,
		cvotecontroller ,
		cwaterbullet ,
		cwaterlodcontrol ,
		cweaponaug ,
		cweaponawp ,
		cweaponbaseitem ,
		cweaponbizon ,
		cweaponcsbase ,
		cweaponcsbasegun ,
		cweaponcycler ,
		cweaponelite ,
		cweaponfamas ,
		cweaponfiveseven ,
		cweapong3sg1 ,
		cweapongalil ,
		cweapongalilar ,
		cweaponglock ,
		cweaponhkp2000 ,
		cweaponm249 ,
		cweaponm3 ,
		cweaponm4a1 ,
		cweaponmac10 ,
		cweaponmag7 ,
		cweaponmp5navy ,
		cweaponmp7 ,
		cweaponmp9 ,
		cweaponnegev ,
		cweaponnova ,
		cweaponp228 ,
		cweaponp250 ,
		cweaponp90 ,
		cweaponsawedoff ,
		cweaponscar20 ,
		cweaponscout ,
		cweaponsg550 ,
		cweaponsg552 ,
		cweaponsg556 ,
		cweaponshield ,
		cweaponssg08 ,
		cweapontaser ,
		cweapontec9 ,
		cweapontmp ,
		cweaponump45 ,
		cweaponusp ,
		cweaponxm1014 ,
		cweaponzonerepulsor ,
		cworld ,
		cworldvguitext ,
		dusttrail ,
		movieexplosion ,
		particlesmokegrenade ,
		rockettrail ,
		smoketrail ,
		sporeexplosion ,
		sporetrail ,
	};

	struct recv_prop_t {
		const char * m_name;
		std::uint32_t m_recv_type;
		std::uint32_t m_flags;
		std::uint32_t m_str_buf_sz;
		std::uint32_t m_inside_array;
		void * m_extra_data;
		void * m_array_prop;
		void * m_array_len_proxy;
		void * m_proxy_fn;
		void * m_table_proxy_fn;
		recv_table_t * m_table;
		std::uint32_t m_offset;
		std::uint32_t m_element_stride;
		std::uint32_t m_element_sz;
		const char * m_parent_name;
	};

	struct recv_table_t {
		recv_prop_t * m_props;
		std::uint32_t m_prop_count;
		void * m_decoder;
		const char * m_net_table_name;
		bool m_in_main_list;
		bool m_initialized;
	};

	struct client_class_t {
		void * m_create_func;
		void * m_create_event_func;
		const char * m_network_name;
		recv_table_t * m_recv_table;
		client_class_t * m_next;
		std::uint32_t m_class_id;
	};

	struct ray_t;

	class base_trace;
	class c_trace;

	class i_handle_entity {
	public:
		virtual ~i_handle_entity ( ) = default;
		virtual void set_ref_handle ( const std::uint32_t & handle ) = 0;
		virtual const std::uint32_t & get_handle ( ) const = 0;
	};

	class c_collideable {
	protected:
		~c_collideable ( ) = default;
	public:
		virtual i_handle_entity * get_entity_handle ( ) = 0;
		virtual c_vector3 & obb_mins ( ) const = 0;
		virtual c_vector3 & obb_maxs ( ) const = 0;
		virtual void world_space_trigger_bounds ( c_vector3 * mins , c_vector3 * maxs ) const = 0;
		virtual bool test_collision ( const ray_t & ray , unsigned int mask , base_trace & trace ) = 0;
		virtual bool test_hitboxes ( const ray_t & ray , unsigned int mask , base_trace & trace ) = 0;
		virtual int get_collision_model_index ( ) = 0;
		virtual const void * get_collision_model ( ) = 0;
		virtual c_vector3 & get_collision_origin ( ) const = 0;
		virtual c_vector3 & get_collision_angles ( ) const = 0;
		virtual const c_matrix3x4 & collision_to_world_transform ( ) const = 0;
		virtual solid_type get_solid ( ) const = 0;
		virtual int get_solid_flags ( ) const = 0;
		virtual c_client_unknown * get_client_unknown ( ) = 0;
		virtual int get_collision_group ( ) const = 0;
		virtual void world_space_surrounding_bounds ( c_vector3 * mins , c_vector3 * maxs ) = 0;
		virtual bool should_touch_trigger ( int flags ) const = 0;
		virtual const c_matrix3x4 * get_root_parent_to_world_transform ( ) const = 0;
	};

	class c_client_unknown : public i_handle_entity {
	public:
		virtual c_collideable * get_collideable ( ) = 0;
		virtual c_client_networkable * get_client_networkable ( ) = 0;
		virtual c_client_renderable * get_client_renderable ( ) = 0;
		virtual c_client_entity * get_client_entity ( ) = 0;
		virtual c_entity * get_base_entity ( ) = 0;
		virtual c_client_thinkable * get_client_thinkable ( ) = 0;
		virtual c_client_alpha_property * get_client_alpha_property ( ) = 0;
	};

	class c_client_thinkable {
	public:
		virtual ~c_client_thinkable ( ) = default;
	};

	class c_client_renderable {
	protected:
		~c_client_renderable ( ) = default;
	public:
		virtual c_client_unknown * get_client_unknown ( ) = 0;
		virtual c_vector3 const & get_render_origin ( ) = 0;
		virtual c_vector3 const & get_render_angles ( ) = 0;
		virtual bool should_draw ( ) = 0;
		virtual int get_render_flags ( ) = 0;
		virtual void unused ( ) const = 0;
		virtual std::uint32_t get_shadow_handle ( ) const = 0;
		virtual std::uint32_t & render_handle ( ) = 0;
		virtual const void * get_model ( ) const = 0;
		virtual int draw_model ( int flags , uint8_t alpha ) = 0;
		virtual int get_body ( ) = 0;
		virtual void get_color_modulation ( float * color ) = 0;
		virtual bool lod_test ( ) = 0;
		virtual bool setup_bones ( c_matrix3x4 * bones , int max , int mask , float time ) = 0;
		virtual void setup_weights ( const c_matrix3x4 * bones , int count , float * weights , float * delayed_weights ) = 0;
		virtual void do_animation_events ( ) = 0;
		virtual void * get_pvs_notify_interface ( ) = 0;
		virtual void get_render_bounds ( c_vector3 & mins , c_vector3 & maxs ) = 0;
		virtual void get_render_bounds_worldspace ( c_vector3 & mins , c_vector3 & maxs ) = 0;
		virtual void get_shadow_render_bounds ( c_vector3 & mins , c_vector3 & maxs , int type ) = 0;
		virtual bool should_receive_projected_textures ( int flags ) = 0;
		virtual bool get_shadow_cast_distance ( float * dist , int type ) const = 0;
		virtual bool get_shadow_cast_direction ( c_vector3 * direction , int type ) const = 0;
		virtual bool is_shadow_dirty ( ) = 0;
		virtual void mark_shadow_dirty ( bool dirty ) = 0;
		virtual c_client_renderable * get_shadow_parent ( ) = 0;
		virtual c_client_renderable * first_shadow_child ( ) = 0;
		virtual c_client_renderable * next_shadow_peer ( ) = 0;
		virtual int shadow_cast_type ( ) = 0;
		virtual void create_model_instance ( ) = 0;
		virtual std::uint32_t get_model_instance ( ) = 0;
		virtual const c_matrix3x4 & renderable_to_world_transform ( ) = 0;
		virtual int lookup_attachment ( const char * name ) = 0;
		virtual bool get_attachment ( int index , c_vector3 & origin , c_vector3 & angles ) = 0;
		virtual bool get_attachment ( int index , c_matrix3x4 & matrix ) = 0;
		virtual float * get_render_clip_plane ( ) = 0;
		virtual int get_skin ( ) = 0;
		virtual void on_threaded_draw_setup ( ) = 0;
		virtual bool uses_flex_delayed_weights ( ) = 0;
		virtual void record_tool_message ( ) = 0;
		virtual bool should_draw_for_split_screen_user ( int slot ) = 0;
		virtual uint8_t override_alpha_modulation ( uint8_t alpha ) = 0;
		virtual uint8_t override_shadow_alpha_modulation ( uint8_t alpha ) = 0;
	};

	class c_client_networkable {
	protected:
		~c_client_networkable ( ) = default;
	public:
		virtual c_client_unknown * get_client_unknown ( ) = 0;
		virtual void release ( ) = 0;
		virtual client_class_t * get_client_class ( ) = 0;
		virtual void notify_should_transmit ( int state ) = 0;
		virtual void on_pre_data_changed ( int type ) = 0;
		virtual void on_data_changed ( int type ) = 0;
		virtual void pre_data_update ( int type ) = 0;
		virtual void post_data_update ( int type ) = 0;
		virtual void on_data_unchanged_in_pvs ( ) = 0;
		virtual bool is_dormant_virt ( ) = 0;
		virtual int	index ( ) const = 0;
		virtual void receive_message ( int class_id , void * msg ) = 0;
		virtual void * get_data_table ( ) = 0;
		virtual void set_destroyed_on_recreate_entities ( ) = 0;

		std::uint32_t get_class_id ( ) {
			return get_client_class ( )->m_class_id;
		}

		bool is_player ( ) {
			return get_class_id ( ) == class_id::ccsplayer;
		}
	};

	class c_client_entity : public c_client_unknown , public c_client_renderable , public c_client_networkable , public c_client_thinkable {
	public:
		virtual ~c_client_entity ( ) { };
	};

	class c_client {
	public:
		client_class_t * get_all_classes ( ) {
			using get_all_classes_fn = client_class_t * ( __thiscall * )( void * );
			return shared::memory::v_func< get_all_classes_fn > ( this , 8 )( this );
		}
	};
}