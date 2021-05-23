#pragma once

#include "entity.hh"

#include <map>

namespace sdk {
	struct strike_weapon_definition_t {
		pad ( 8 );
		int weapon_id;
	};

	enum class e_cs_weapon_type : int {
		weapontype_knife = 0 ,
		weapontype_pistol ,
		weapontype_submachinegun ,
		weapontype_rifle ,
		weapontype_shotgun ,
		weapontype_sniper_rifle ,
		weapontype_machinegun ,
		weapontype_c4 ,
		weapontype_placeholder ,
		weapontype_grenade ,
		weapontype_unknown
	};

	enum class e_item_definition_index : short {
		weapon_none = 0 ,
		weapon_deagle ,
		weapon_elite ,
		weapon_fiveseven ,
		weapon_glock ,
		weapon_ak47 = 7 ,
		weapon_aug ,
		weapon_awp ,
		weapon_famas ,
		weapon_g3sg1 ,
		weapon_galilar = 13 ,
		weapon_m249 ,
		weapon_m4a1 = 16 ,
		weapon_mac10 ,
		weapon_p90 = 19 ,
		weapon_mp5sd = 23 ,
		weapon_ump45 ,
		weapon_xm1014 ,
		weapon_bizon ,
		weapon_mag7 ,
		weapon_negev ,
		weapon_sawedoff ,
		weapon_tec9 ,
		weapon_taser ,
		weapon_hkp2000 ,
		weapon_mp7 ,
		weapon_mp9 ,
		weapon_nova ,
		weapon_p250 ,
		weapon_shield ,
		weapon_scar20 ,
		weapon_sg556 ,
		weapon_ssg08 ,
		weapon_knifegg ,
		weapon_knife ,
		weapon_flashbang ,
		weapon_hegrenade ,
		weapon_smokegrenade ,
		weapon_molotov ,
		weapon_decoy ,
		weapon_incgrenade ,
		weapon_c4 ,
		weapon_healthshot = 57 ,
		weapon_knife_t = 59 ,
		weapon_m4a1_silencer ,
		weapon_usp_silencer ,
		weapon_cz75a = 63 ,
		weapon_revolver ,
		weapon_tagrenade = 68 ,
		weapon_fists ,
		weapon_breachcharge ,
		weapon_tablet = 72 ,
		weapon_melee = 74 ,
		weapon_axe ,
		weapon_hammer ,
		weapon_spanner = 78 ,
		weapon_knife_ghost = 80 ,
		weapon_firebomb ,
		weapon_diversion ,
		weapon_frag_grenade ,
		weapon_snowball ,
		weapon_bumpmine ,
		weapon_bayonet = 500 ,
		weapon_knife_css = 500 ,
		weapon_knife_flip = 505 ,
		weapon_knife_gut ,
		weapon_knife_karambit ,
		weapon_knife_m9_bayonet ,
		weapon_knife_tactical ,
		weapon_knife_falchion = 512 ,
		weapon_knife_survival_bowie = 514 ,
		weapon_knife_butterfly ,
		weapon_knife_push ,
		weapon_knife_cord ,
		weapon_knife_canis ,
		weapon_knife_ursus = 519 ,
		weapon_knife_gypsy_jackknife ,
		weapon_knife_outdoor ,
		weapon_knife_stiletto = 522 ,
		weapon_knife_widowmaker ,
		weapon_knife_skeleton = 525 ,
		glove_studded = 5027 ,
		glove_t_side ,
		glove_ct_side ,
		glove_sporty ,
		glove_slick ,
		glove_leather ,
		glove_motorcycle ,
		glove_specialist ,
		glove_hydra
	};

	/* from some random sdk */
	class c_weaponinfo {
	public:
		virtual ~c_weaponinfo ( ) = default;

		//char pad_0000[4]; //0x0000
		char * consolename;				// 0x0004
		char		pad_0008 [ 8 ];				// 0x0008
		void * m_pweapondef;  //0x0010
		int			imaxclip1;					// 0x0014
		int			imaxclip2;					// 0x0018
		int			idefaultclip1;				// 0x001c
		int			idefaultclip2;				// 0x0020
		char		pad_0024 [ 8 ];				// 0x0024
		char * szworldmodel;				// 0x002c
		char * szviewmodel;				// 0x0030
		char * szdroppedmodel;				// 0x0034
		char		pad_0038 [ 4 ];				// 0x0038
		char * n0000023e;					// 0x003c
		char		pad_0040 [ 56 ];				// 0x0040
		char * szemptysound;				// 0x0078
		char		pad_007c [ 4 ];				// 0x007c
		char * szbullettype;				// 0x0080
		char		pad_0084 [ 4 ];				// 0x0084
		char * szhudname;					// 0x0088
		char * szweaponname;				// 0x008c
		char		pad_0090 [ 60 ];				// 0x0090
		int 		weapontype;					// 0x00c8
		int			iweaponprice;				// 0x00cc
		int			ikillaward;					// 0x00d0
		char * szanimationprefix;			// 0x00d4
		float		flcycletime;				// 0x00d8
		float		flcycletimealt;				// 0x00dc
		float		fltimetoidle;				// 0x00e0
		float		flidleinterval;				// 0x00e4
		bool		bfullauto;					// 0x00e8
		char		pad_0x00e5 [ 3 ];			// 0x00e9
		int			idamage;					// 0x00ec
		float		flarmorratio;				// 0x00f0
		int			ibullets;					// 0x00f4
		float		flpenetration;				// 0x00f8
		float		flflinchvelocitymodifierlarge;	// 0x00fc
		float		flflinchvelocitymodifiersmall;	// 0x0100
		float		flrange;					// 0x0104
		float		flrangemodifier;			// 0x0108
		float		flthrowvelocity;			// 0x010c
		char		pad_0x010c [ 16 ];			// 0x0110
		bool		bhassilencer;				// 0x011c
		char		pad_0x0119 [ 3 ];			// 0x011d
		char * psilencermodel;				// 0x0120
		int			icrosshairmindistance;		// 0x0124
		float		flmaxplayerspeed;	        // 0x0128
		float		flmaxplayerspeedalt;		// 0x012c
		char		pad_0x0130 [ 4 ];		    // 0x0130
		float		flspread;					// 0x0134
		float		flspreadalt;				// 0x0138
		float		flinaccuracycrouch;			// 0x013c
		float		flinaccuracycrouchalt;		// 0x0140
		float		flinaccuracystand;			// 0x0144
		float		flinaccuracystandalt;		// 0x0148
		float		flinaccuracyjumpinitial;	// 0x014c
		float		flinaccuracyjump;			// 0x0150
		float		flinaccuracyjumpalt;		// 0x0154
		float		flinaccuracyland;			// 0x0158
		float		flinaccuracylandalt;		// 0x015c
		float		flinaccuracyladder;			// 0x0160
		float		flinaccuracyladderalt;		// 0x0164
		float		flinaccuracyfire;			// 0x0168
		float		flinaccuracyfirealt;		// 0x016c
		float		flinaccuracymove;			// 0x0170
		float		flinaccuracymovealt;		// 0x0174
		float		flinaccuracyreload;			// 0x0178
		int			irecoilseed;				// 0x017c
		float		flrecoilangle;				// 0x0180
		float		flrecoilanglealt;			// 0x0184
		float		flrecoilanglevariance;		// 0x0188
		float		flrecoilanglevariancealt;	// 0x018c
		float		flrecoilmagnitude;			// 0x0190
		float		flrecoilmagnitudealt;		// 0x0194
		float		flrecoilmagnitudevariance;	// 0x0198
		float		flrecoilmagnitudevariancealt;	// 0x019c
		float		flrecoverytimecrouch;		// 0x01a0
		float		flrecoverytimestand;		// 0x01a4
		float		flrecoverytimecrouchfinal;	// 0x01a8
		float		flrecoverytimestandfinal;	// 0x01ac
		int			irecoverytransitionstartbullet;	// 0x01b0 
		int			irecoverytransitionendbullet;	// 0x01b4
		bool		bunzoomaftershot;			// 0x01b8
		bool		bhideviewmodelzoomed;		// 0x01b9
		char		pad_0x01b5 [ 2 ];			// 0x01ba
		char		izoomlevels [ 3 ];			// 0x01bc
		int			izoomfov [ 2 ];				// 0x01c0
		float		fzoomtime [ 3 ];				// 0x01c4
		char * szweaponclass;				// 0x01d4
		float		fladdonscale;				// 0x01d8
		char		pad_0x01dc [ 4 ];			// 0x01dc
		char * szejectbrasseffect;			// 0x01e0
		char * sztracereffect;				// 0x01e4
		int			itracerfrequency;			// 0x01e8
		int			itracerfrequencyalt;		// 0x01ec
		char * szmuzzleflasheffect_1stperson;	// 0x01f0
		char		pad_0x01f4 [ 4 ];				// 0x01f4
		char * szmuzzleflasheffect_3rdperson;	// 0x01f8
		char		pad_0x01fc [ 4 ];			// 0x01fc
		char * szmuzzlesmokeeffect;		// 0x0200
		float		flheatpershot;				// 0x0204
		char * szzoominsound;				// 0x0208
		char * szzoomoutsound;				// 0x020c
		float		flinaccuracypitchshift;		// 0x0210
		float		flinaccuracysoundthreshold;	// 0x0214
		float		flbotaudiblerange;			// 0x0218
		char		pad_0x0218 [ 8 ];			// 0x0220
		char * pwrongteammsg;				// 0x0224
		bool		bhasburstmode;				// 0x0228
		char		pad_0x0225 [ 3 ];			// 0x0229
		bool		bisrevolver;				// 0x022c
		bool		bcannotshootunderwater;		// 0x0230
	};

	class c_weapon {
	public:
		/* netvars */

		/* local weapon data */
		netvar_fn ( float , next_primary_attack , "DT_LocalActiveWeaponData->m_flNextPrimaryAttack" );
		netvar_fn ( float , next_secondary_attack , "DT_LocalActiveWeaponData->m_flNextSecondaryAttack" );
		netvar_fn ( float , time_weapon_idle , "DT_LocalActiveWeaponData->m_flTimeWeaponIdle" );

		/* weapon cs base */
		netvar_fn ( float , postpone_fire_ready_time , "DT_WeaponCSBase->m_flPostponeFireReadyTime" );
		netvar_fn ( float , last_shot_time , "DT_WeaponCSBase->m_fLastShotTime" );
		netvar_fn ( int , clip , "DT_BaseCombatWeapon->m_iClip1" );
		netvar_fn ( int , second_clip , "DT_BaseCombatWeapon->m_iClip2" );

		/* nades */
		netvar_fn ( float , throw_time , "DT_BaseCSGrenade->m_fThrowTime" );
		netvar_fn ( bool , pin_pulled , "DT_BaseCSGrenade->m_bPinPulled" );

		/* offsets */
		// offset ( e_item_definition_index , item_definition_index , 0x2FAA );
		offset ( bool , in_reload , 0x32A5 );

		/* methods */
		/* shit netvar manager forces my hand lolll */
		e_item_definition_index item_definition_index ( ) {
			if ( !this ) return e_item_definition_index::weapon_none;

			static const auto offset = hack::game::netvars::offsets [ fnv1a ( "DT_BaseAttributableItem->m_AttributeManager" ) ] + hack::game::netvars::offsets [ fnv1a ( "DT_AttributeContainer->m_Item" ) ] + hack::game::netvars::offsets [ fnv1a ( "DT_ScriptCreatedItem->m_iItemDefinitionIndex" ) ];
			return *( e_item_definition_index * ) ( std::uintptr_t ( this ) + offset );
		}

		/* this is a meme */
		auto weapon_kind ( ) {
			switch ( this->item_definition_index ( ) ) {
				case sdk::e_item_definition_index::weapon_c4:
					return sdk::e_cs_weapon_type::weapontype_c4;
					break;
				case sdk::e_item_definition_index::weapon_knife:
				case sdk::e_item_definition_index::weapon_knifegg:
				case sdk::e_item_definition_index::weapon_knife_butterfly:
				case sdk::e_item_definition_index::weapon_knife_canis:
				case sdk::e_item_definition_index::weapon_knife_cord:
				case sdk::e_item_definition_index::weapon_knife_css:
				case sdk::e_item_definition_index::weapon_knife_falchion:
				case sdk::e_item_definition_index::weapon_knife_flip:
				case sdk::e_item_definition_index::weapon_knife_ghost:
				case sdk::e_item_definition_index::weapon_knife_gut:
				case sdk::e_item_definition_index::weapon_knife_gypsy_jackknife:
				case sdk::e_item_definition_index::weapon_knife_karambit:
				case sdk::e_item_definition_index::weapon_knife_m9_bayonet:
				case sdk::e_item_definition_index::weapon_knife_push:
				case sdk::e_item_definition_index::weapon_knife_outdoor:
				case sdk::e_item_definition_index::weapon_knife_skeleton:
				case sdk::e_item_definition_index::weapon_knife_stiletto:
				case sdk::e_item_definition_index::weapon_knife_survival_bowie:
				case sdk::e_item_definition_index::weapon_knife_t:
				case sdk::e_item_definition_index::weapon_knife_tactical:
				case sdk::e_item_definition_index::weapon_knife_ursus:
				case sdk::e_item_definition_index::weapon_knife_widowmaker:
					return sdk::e_cs_weapon_type::weapontype_knife;
					break;
				case sdk::e_item_definition_index::weapon_hegrenade:
				case sdk::e_item_definition_index::weapon_decoy:
				case sdk::e_item_definition_index::weapon_smokegrenade:
				case sdk::e_item_definition_index::weapon_molotov:
				case sdk::e_item_definition_index::weapon_incgrenade:
				case sdk::e_item_definition_index::weapon_firebomb:
				case sdk::e_item_definition_index::weapon_flashbang:
				case sdk::e_item_definition_index::weapon_frag_grenade:
				case sdk::e_item_definition_index::weapon_tagrenade:
				case sdk::e_item_definition_index::weapon_snowball:
					return sdk::e_cs_weapon_type::weapontype_grenade;
					break;
				case sdk::e_item_definition_index::weapon_g3sg1:
				case sdk::e_item_definition_index::weapon_scar20:
				case sdk::e_item_definition_index::weapon_awp:
				case sdk::e_item_definition_index::weapon_ssg08:
					return sdk::e_cs_weapon_type::weapontype_sniper_rifle;
					break;
				default:
					return sdk::e_cs_weapon_type::weapontype_unknown;
					break;
			}

			return  sdk::e_cs_weapon_type::weapontype_unknown;
		}

		std::pair<std::string , bool> get_weapon_as_string ( ) {
			if ( !this || !this->data ( ) )
				return std::pair<std::string , bool> ( " " , false );

			std::string name = this->data ( )->consolename;
			name.erase ( 0 , 7 );

			return std::pair<std::string , bool > ( name , false );
		}

		sdk::c_weaponinfo * data ( );
		float inaccuracy ( );
		float spread ( );
		void update_accuracy ( );
	};
}