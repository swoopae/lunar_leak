#include "framework.hh"

std::string config_name = ""; 

void callback_function ( ) {

}

void hack::menu::impl::init ( ) {
	auto menu_window = std::make_shared < controls::c_window > ( STR ( "main window" ) , true ); {
		auto home_form = std::make_shared < controls::c_home > ( STR ( "poopybutt" ) ); {
			auto rage_form = std::make_shared < controls::c_form > ( STR ( "rage, " ) , STR ( "rage" ) ); {
				auto rage_subtab = std::make_shared < controls::c_subtab > ( STR ( "ragebot" ) ); {
					auto rage_control = std::make_shared < controls::c_groupbox > ( STR ( "ragebot" ) , STR ( "automatically aim at enemies" ) ); {
						std::vector<std::string> target_selection_kinds = { STR ( "by distance to crosshair" ) , STR ( "by distance to origin" ) , STR ( "by lowest hp" ) , STR ( "by height" ) };
						auto r_enabled = std::make_shared < controls::c_checkbox > ( STR ( "enable ragebot" ) , fnv1a ( "r_enabled" ) , false );
						auto r_silent = std::make_shared < controls::c_checkbox > ( STR ( "silent aim" ) , fnv1a ( "r_silent" ) , false );
						auto r_targetselection = std::make_shared < controls::c_combobox > ( STR ( "target selection" ) , fnv1a ( "r_targetselection" ) , target_selection_kinds );
						auto r_autoscope = std::make_shared < controls::c_checkbox > ( STR ( "auto scope" ) , fnv1a ( "r_autoscope" ) , false );
						auto r_tickbasemanip = std::make_shared < controls::c_checkbox > ( STR ( "tickbase manipulation" ) , fnv1a ( "r_tickbasemanip" ) , false );
						auto r_tickbasemanip_key = std::make_shared < controls::c_keybind > ( fnv1a ( "r_tickbasemanip_key" ) , 2 );
					#if defined(_DEBUG) || defined(LUNAR_DEV) || defined(LUNAR_ALPHA)
						auto r_overridetickbasedefault = std::make_shared < controls::c_checkbox > ( STR ( "override tickbase shift" ) , fnv1a ( "r_overridetickbasedefault" ) , false );
						auto r_tickbaseshift = std::make_shared < controls::c_slider > ( STR ( "tickbase shift" ) , fnv1a ( "r_tickbaseshift" ) , 15 , 2 , 16 );
					#endif
						auto r_tickbase_teleport = std::make_shared < controls::c_checkbox > ( STR ( "accelerate on shift" ) , fnv1a ( "r_tickbase_teleport" ) , false );

						r_tickbasemanip->bind ( r_tickbasemanip_key );

						rage_control->bind ( r_enabled );
						rage_control->bind ( r_silent );
						rage_control->bind ( r_targetselection );
						rage_control->bind ( r_autoscope );
						rage_control->bind ( r_tickbasemanip );
						rage_control->bind ( r_tickbase_teleport );
					#if defined(_DEBUG) || defined(LUNAR_DEV) || defined(LUNAR_ALPHA)
						rage_control->bind ( r_overridetickbasedefault );
						rage_control->bind ( r_tickbaseshift );
					#endif
					}

					rage_subtab->bind ( rage_control );
				}
				auto rageconfig_subtab = std::make_shared < controls::c_subtab > ( STR ( "config" ) ); {
					auto rage_weapon = std::make_shared < controls::c_groupbox > ( STR ( "config" ) , STR ( "ragebot configuration for weapons" ) ); {
						std::vector<std::string> extra_hboxes = { STR ( "hands" ) , STR ( "legs" ) , STR ( "feet" ) };
						std::vector<std::uint32_t> extra_hboxes_hashes = { fnv1a ( "rc_hitbox_hands" ) , fnv1a ( "rc_hitbox_legs" ) , fnv1a ( "rc_hitbox_feet" ) };
						std::vector<std::string> safepoint_conditions = { STR ( "lethal" ) , STR ( "lethal x2" ) , STR ( "in air" ) , STR ( "on key" ) };
						std::vector<std::uint32_t> safepoint_conditions_hashes = { fnv1a ( "rc_safepoint_lethal" ) , fnv1a ( "rc_safepoint_lethal2" ), fnv1a ( "rc_safepoint_air" ), fnv1a ( "rc_safepoint_key" ) };

						auto rc_override = std::make_shared < controls::c_checkbox > ( STR ( "use same config for every weapon" ) , fnv1a ( "rc_override" ) , true );
						auto rc_hitchance = std::make_shared < controls::c_slider > ( STR ( "hit chance" ) , fnv1a ( "rc_hitchance" ) , 45 , 0 , 100 );
						auto rc_mindmg = std::make_shared < controls::c_slider > ( STR ( "minimum damage" ) , fnv1a ( "rc_mindmg" ) , 25 , 0 , 100 );
						auto rc_extrahitboxes = std::make_shared < controls::c_multicombobox > ( STR ( "extra hitscan bones" ) , extra_hboxes_hashes , extra_hboxes );
						auto rc_safepoint_conditions = std::make_shared < controls::c_multicombobox > ( STR ( "safepoint conditions" ) , safepoint_conditions_hashes , safepoint_conditions );
						auto rc_staticmultipoint = std::make_shared < controls::c_checkbox > ( STR ( "force static multipoint" ) , fnv1a ( "rc_staticmultipoint" ) , false );
						auto rc_headmp = std::make_shared < controls::c_slider > ( STR ( "head multipoint" ) , fnv1a ( "rc_headmp" ) , 55 , 0 , 100 );
						auto rc_bodymp = std::make_shared < controls::c_slider > ( STR ( "body multipoint" ) , fnv1a ( "rc_bodymp" ) , 70 , 0 , 100 );

						// rage_weapon->bind ( rc_override );
						rage_weapon->bind ( rc_hitchance );
						rage_weapon->bind ( rc_mindmg );
						rage_weapon->bind ( rc_extrahitboxes );
						rage_weapon->bind ( rc_staticmultipoint );
						rage_weapon->bind ( rc_headmp );
						rage_weapon->bind ( rc_bodymp );
						rage_weapon->bind ( rc_safepoint_conditions );
					}

					rageconfig_subtab->bind ( rage_weapon );
				}
				auto antiaim_subtab = std::make_shared < controls::c_subtab > ( STR ( "antiaim" ) ); {
					auto antiaim_control = std::make_shared < controls::c_groupbox > ( STR ( "antiaim" ) , STR ( "prevent getting hit from enemies" ) ); {
						auto aa_enabled = std::make_shared < controls::c_checkbox > ( STR ( "enable antiaim" ) , fnv1a ( "aa_enabled" ) , false );
						auto aa_at_target = std::make_shared < controls::c_checkbox > ( STR ( "at targets" ) , fnv1a ( "aa_at_target" ) , false );
						std::vector<std::string> modifiers = { STR ( "none" ) , STR ( "180z" ) , STR ( "jitter" ) , STR ( "spin" ) };
						auto aa_yaw = std::make_shared < controls::c_slider > ( STR ( "base yaw" ) , fnv1a ( "aa_yaw" ) , 0 , -180 , 180 );
						auto aa_modifier = std::make_shared < controls::c_combobox > ( STR ( "yaw modifier" ) , fnv1a ( "aa_modifier" ) , modifiers );
						auto aa_modifier_range = std::make_shared < controls::c_slider > ( STR ( "yaw modifier range" ) , fnv1a ( "aa_modifier_range" ) , 30 , 0 , 180 );
						auto aa_desync = std::make_shared < controls::c_checkbox > ( STR ( "desync" ) , fnv1a ( "aa_desync" ) , false );
						auto aa_desync_invert_key = std::make_shared < controls::c_keybind > ( fnv1a ( "aa_desync_invert_key" ) , 1 );
						std::vector<std::string> desync_modifiers = { STR ( "static" ) , STR ( "sway" ) , STR ( "sidemove" ) };
						auto aa_desync_strength = std::make_shared < controls::c_slider > ( STR ( "desync strength" ) , fnv1a ( "aa_desync_strength" ) , 75 , 0 , 100 );
						auto aa_desync_modifier = std::make_shared < controls::c_combobox > ( STR ( "desync modifier" ) , fnv1a ( "aa_desync_modifier" ) , desync_modifiers );

						aa_desync->bind ( aa_desync_invert_key );

						antiaim_control->bind ( aa_enabled );
						antiaim_control->bind ( aa_at_target );
						antiaim_control->bind ( aa_yaw );
						antiaim_control->bind ( aa_modifier );
						antiaim_control->bind ( aa_modifier_range );
						antiaim_control->bind ( aa_desync );
						antiaim_control->bind ( aa_desync_strength );
						antiaim_control->bind ( aa_desync_modifier );
					};

					antiaim_subtab->bind ( antiaim_control );
				}
				auto fakelag_subtab = std::make_shared < controls::c_subtab > ( STR ( "fakelag" ) ); {
					auto fakelag_control = std::make_shared < controls::c_groupbox > ( STR ( "fakelag" ) , STR ( "control fakelag amount" ) ); {
						auto lag_enabled = std::make_shared < controls::c_checkbox > ( STR ( "enable fakelag" ) , fnv1a ( "lag_enabled" ) , false );
						/* limited to 3 because that's how much we need to properly desync without some dumb shit (might be incorrect but there's close to no harm done) */
						auto lag_standing = std::make_shared < controls::c_slider > ( STR ( "standing fakelag" ) , fnv1a ( "lag_standing" ) , 3 , 2 , 16 );
						auto lag_moving = std::make_shared < controls::c_slider > ( STR ( "moving fakelag" ) , fnv1a ( "lag_moving" ) , 6 , 2 , 16 );
						auto lag_air = std::make_shared < controls::c_slider > ( STR ( "air fakelag" ) , fnv1a ( "lag_air" ) , 16 , 2 , 16 );
						std::vector<std::string> fakelag_type = { STR ( "linear" ) , STR ( "adaptive" ) };
						auto lag_modifier = std::make_shared < controls::c_combobox > ( STR ( "fakelag modifier" ) , fnv1a ( "lag_modifier" ) , fakelag_type );
						std::vector<std::string> fakelag_spike = { STR ( "on peek" ) , STR ( "on land" ) , STR ( "on reload" ) , STR ( "on key" ) };
						std::vector<std::uint32_t> fakelag_spike_hashes = { fnv1a ( "lag_onpeek" ) , fnv1a ( "lag_onland" ) , fnv1a ( "lag_onreload" ) , fnv1a ( "lag_onkey" ) };
						/* should be a multicombo */
						auto lag_spike = std::make_shared < controls::c_multicombobox > ( STR ( "fakelag spike triggers" ) , fakelag_spike_hashes , fakelag_spike );
						auto lag_spike_key = std::make_shared < controls::c_keybind > ( fnv1a ( "lag_spike_key" ) , 2 );

						/* TODO: support for children on combobox/multicombo */
						lag_spike->bind ( lag_spike_key );

						fakelag_control->bind ( lag_enabled );
						fakelag_control->bind ( lag_standing );
						fakelag_control->bind ( lag_moving );
						fakelag_control->bind ( lag_air );
						fakelag_control->bind ( lag_modifier );
						fakelag_control->bind ( lag_spike );
					}

					fakelag_subtab->bind ( fakelag_control );
				}
				auto misc_subtab = std::make_shared < controls::c_subtab > ( STR ( "misc" ) ); {
					auto misc_control = std::make_shared < controls::c_groupbox > ( STR ( "misc" ) , STR ( "miscellaneous rage features" ) ); {
						auto rage_fakeduck = std::make_shared < controls::c_checkbox > ( STR ( "fake duck" ) , fnv1a ( "rage_fakeduck" ) , false );
						auto rage_fakeduck_key = std::make_shared < controls::c_keybind > ( fnv1a ( "rage_fakeduck_key" ) , 2 );

						rage_fakeduck->bind ( rage_fakeduck_key );

						misc_control->bind ( rage_fakeduck );
					}

					misc_subtab->bind ( misc_control );
				}

				rage_form->bind ( rage_subtab );
				rage_form->bind ( rageconfig_subtab );
				rage_form->bind ( antiaim_subtab );
				rage_form->bind ( fakelag_subtab );
				rage_form->bind ( misc_subtab );
			}
			auto vis_form = std::make_shared < controls::c_form > ( STR ( "visuals, " ) , STR ( "visuals" ) ); {
				auto players_subtab = std::make_shared < controls::c_subtab > ( STR ( "players" ) ); {
					auto players_control = std::make_shared < controls::c_groupbox > ( STR ( "players" ) , STR ( "player related visuals" ) ); {
						auto e_enabled = std::make_shared < controls::c_checkbox > ( STR ( "enable esp" ) , fnv1a ( "e_enabled" ) , false );
						auto e_box = std::make_shared < controls::c_checkbox > ( STR ( "box" ) , fnv1a ( "e_box" ) , false );
						auto e_acc_col = std::make_shared < controls::c_colorpicker > ( fnv1a ( "e_acc_col" ) , c_color ( 255 , 168 , 250 ) );
						auto e_sec_col = std::make_shared < controls::c_colorpicker > ( fnv1a ( "e_sec_col" ) , c_color ( 0 , 212 , 255 ) );
						auto e_border_col = std::make_shared < controls::c_colorpicker > ( fnv1a ( "e_border_col" ) , c_color ( 0 , 0 , 0 , 165 ) );
						auto e_name = std::make_shared < controls::c_checkbox > ( STR ( "name" ) , fnv1a ( "e_name" ) , false );
						auto e_weap = std::make_shared < controls::c_checkbox > ( STR ( "weapon name" ) , fnv1a ( "e_weap" ) , false );
						auto e_hpbar = std::make_shared < controls::c_checkbox > ( STR ( "health" ) , fnv1a ( "e_hpbar" ) , false );
						auto e_hpbar_acc = std::make_shared < controls::c_colorpicker > ( fnv1a ( "e_hpbar_acc" ) , c_color ( 255 , 168 , 250 ) );
						auto e_hpbar_sec = std::make_shared < controls::c_colorpicker > ( fnv1a ( "e_hpbar_sec" ) , c_color ( 0 , 212 , 255 ) );

						e_box->bind ( e_border_col );
						e_box->bind ( e_sec_col );
						e_box->bind ( e_acc_col );

						e_hpbar->bind ( e_hpbar_sec );
						e_hpbar->bind ( e_hpbar_acc );

						players_control->bind ( e_enabled );
						players_control->bind ( e_box );
						players_control->bind ( e_name );
						players_control->bind ( e_weap );
						players_control->bind ( e_hpbar );
					}
					players_subtab->bind ( players_control );
				}
				auto chams_subtab = std::make_shared < controls::c_subtab > ( STR ( "chams" ) ); {
					auto chams_control = std::make_shared<controls::c_groupbox> ( STR ( "chams" ) , STR ( "player model visual effects" ) ); {
					}
					chams_subtab->bind ( chams_control );
				}
				auto effects_subtab = std::make_shared < controls::c_subtab > ( STR ( "effects" ) ); {
					auto effects_control = std::make_shared < controls::c_groupbox > ( STR ( "effects" ) , STR ( "audio-visual effects" ) ); {
						auto fx_fov = std::make_shared < controls::c_slider > ( STR ( "field of view" ) , fnv1a ( "fx_fov" ) , 90 , 70 , 140 );
						auto fx_fov_override = std::make_shared < controls::c_checkbox > ( STR ( "override zoom with field of view" ) , fnv1a ( "fx_fov_override" ) , false );
						auto fx_precipitation = std::make_shared < controls::c_checkbox > ( STR ( "precipitation" ) , fnv1a ( "fx_precipitation" ) , false );
						auto fx_interpolate_origin = std::make_shared < controls::c_checkbox > ( STR ( "interpolate model origin" ) , fnv1a ( "fx_interpolate_origin" ) , true );
						auto fx_world_modulation = std::make_shared < controls::c_checkbox > ( STR ( "world modulation" ) , fnv1a ( "fx_world_modulation" ) , false );
						auto fx_world_modulation_color = std::make_shared < controls::c_colorpicker > ( fnv1a ( "fx_world_modulation_color" ) , c_color ( 128 , 128 , 128 , 200 ) );

						fx_world_modulation->bind ( fx_world_modulation_color );

						effects_control->bind ( fx_fov );
						effects_control->bind ( fx_fov_override );
						effects_control->bind ( fx_precipitation );
						effects_control->bind ( fx_interpolate_origin );
						effects_control->bind ( fx_world_modulation );
					}
					effects_subtab->bind ( effects_control );
				}
				auto misc_subtab = std::make_shared < controls::c_subtab > ( STR ( "misc" ) ); {
					auto misc_control = std::make_shared<controls::c_groupbox> ( STR ( "misc" ) , STR ( "miscellaneous visuals" ) ); {
						auto fx_thirdperson = std::make_shared < controls::c_checkbox > ( STR ( "third person" ) , fnv1a ( "fx_thirdperson" ) , false );
						auto fx_thirdperson_key = std::make_shared < controls::c_keybind > ( fnv1a ( "fx_thirdperson_key" ) , 1 );
						auto fx_thirdperson_on_dead = std::make_shared < controls::c_checkbox > ( STR ( "force third person on spectate" ) , fnv1a ( "fx_thirdperson_on_dead" ) , false );
						auto fx_thirdperson_value = std::make_shared < controls::c_slider > ( STR ( "third person distance" ) , fnv1a ( "fx_thirdperson_value" ) , 110 , 50 , 150 );
						auto fx_indicators = std::make_shared < controls::c_checkbox > ( STR ( "indicators" ) , fnv1a ( "fx_indicators" ) , false );

						fx_thirdperson->bind ( fx_thirdperson_key );

						misc_control->bind ( fx_thirdperson );
						misc_control->bind ( fx_thirdperson_on_dead );
						misc_control->bind ( fx_thirdperson_value );
						misc_control->bind ( fx_indicators );
					}
					misc_subtab->bind ( misc_control );
				}

				vis_form->bind ( players_subtab );
				vis_form->bind ( chams_subtab );
				vis_form->bind ( effects_subtab );
				vis_form->bind ( misc_subtab );
			}
			auto misc_form = std::make_shared < controls::c_form > ( STR ( "misc, " ) , STR ( "misc" ) ); {
				auto movement_subtab = std::make_shared < controls::c_subtab > ( STR ( "movement" ) ); {
					auto home_groupbox = std::make_shared < controls::c_groupbox > ( STR ( "movement" ) , STR ( "miscellaneous movement features" ) ); {
						auto m_bhop = std::make_shared < controls::c_checkbox > ( STR ( "auto jump" ) , fnv1a ( "m_bhop" ) , false );
						auto m_autostrafer = std::make_shared < controls::c_checkbox > ( STR ( "auto strafer" ) , fnv1a ( "m_autostrafer" ) , false );
						auto m_noduckdelay = std::make_shared < controls::c_checkbox > ( STR ( "no duck delay" ) , fnv1a ( "m_noduckdelay" ) , false );
						auto m_selfpeek = std::make_shared < controls::c_checkbox > ( STR ( "self peek" ) , fnv1a ( "m_selfpeek" ) , false );
						auto m_selfpeek_key = std::make_shared < controls::c_keybind > ( fnv1a ( "m_selfpeek_key" ) , 2 );

						m_selfpeek->bind ( m_selfpeek_key );

						home_groupbox->bind ( m_bhop );
						home_groupbox->bind ( m_autostrafer );
						home_groupbox->bind ( m_noduckdelay );
						home_groupbox->bind ( m_selfpeek );
					}

					movement_subtab->bind ( home_groupbox );
				}
				auto misc_subtab = std::make_shared < controls::c_subtab > ( STR ( "misc" ) ); {
					auto boss_groupbox = std::make_shared < controls::c_groupbox > ( STR ( "misc" ) , STR ( "miscellaneous features" ) ); {
						auto m_clantag = std::make_shared < controls::c_checkbox > ( STR ( "clantag changer" ) , fnv1a ( "m_clantag" ) , false );

						boss_groupbox->bind ( m_clantag );
					}
					misc_subtab->bind ( boss_groupbox );
				}

				misc_form->bind ( movement_subtab );
				misc_form->bind ( misc_subtab );
			}
			auto home_t = std::make_shared < controls::c_form > ( STR ( "home" ) , STR ( "home" ) ); {
				auto home_subtab = std::make_shared < controls::c_subtab > ( STR ( "home" ) ); {
				}
				auto cfg_subtab = std::make_shared < controls::c_subtab > ( STR ( "config" ) ); {
					auto cfg_groupbox = std::make_shared < controls::c_groupbox > ( STR ( "config" ) , STR ( "manage your hack configs" ) ); {
						auto cfg_name = std::make_shared < controls::c_textedit > ( STR ( "config name" ) , &config_name );
						auto cfg_save = std::make_shared < controls::c_button > ( STR ( "save config" ) , callback_function );
						auto cfg_load = std::make_shared < controls::c_button > ( STR ( "load config" ) , callback_function );
						auto cfg_add = std::make_shared < controls::c_button > ( STR ( "add config" ) , callback_function );
						auto cfg_remove = std::make_shared < controls::c_button > ( STR ( "remove config" ) , callback_function );

						cfg_groupbox->bind ( cfg_name );
						cfg_groupbox->bind ( cfg_save );
						cfg_groupbox->bind ( cfg_load );
						cfg_groupbox->bind ( cfg_add );
						cfg_groupbox->bind ( cfg_remove );
					}

					cfg_subtab->bind ( cfg_groupbox );
				}
				auto scripts_subtab = std::make_shared < controls::c_subtab > ( STR ( "scripts" ) ); {
					auto scripts_groupbox = std::make_shared < controls::c_groupbox > ( STR ( "scripts" ) , STR ( "manage your hack scripts" ) ); {
						auto script_toggle = std::make_shared < controls::c_button > ( STR ( "toggle script file" ) , callback_function );

						scripts_groupbox->bind ( script_toggle );
					}

					scripts_subtab->bind ( scripts_groupbox );
				}

				home_t->bind ( home_subtab );
				home_t->bind ( cfg_subtab );
				home_t->bind ( scripts_subtab );
			}

			home_form->bind ( rage_form );
			home_form->bind ( vis_form );
			home_form->bind ( misc_form );
			home_form->bind ( home_t );
		}

		menu_window->bind ( home_form );
	}

	menu_window->init ( c_vector2 ( 460 , 375 ) , c_vector2 ( 600 , 350 ) );

	window_list.emplace_back ( menu_window );

	auto test_window = std::make_shared < controls::c_window > ( STR ( "test window" ) , false ); {
		auto custom_home_form = std::make_shared < controls::c_custom_home > ( );

		test_window->bind ( custom_home_form );
	}

	test_window->init ( c_vector2 ( 75 , 20 ) );

	window_list.emplace_back ( test_window );
}

void hack::menu::impl::run_menu ( ) {
	/* TODO: implement scrolling */
	controls::scroll_factor = 0;

	if ( !input::grab_input ( STR ( "Counter-Strike: Global Offensive" ) ) )
		return;

	if ( input::key_released ( VK_INSERT ) )
		impl::menu_opened = !impl::menu_opened;

	for ( auto & window : window_list ) {
		if ( !window->m_on_key || ( impl::menu_opened && window->m_on_key ) ) {
			window->think ( );
			window->draw ( );
		}
	}
}
