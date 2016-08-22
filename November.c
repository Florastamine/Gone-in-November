/* November.c */
/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                     Version 2, December 2004
 *
 *  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 */
#include "Unmanaged.h"
#include "November.h"

// Initializes the scene list, the scenes, and push them into the list.
void nov_scene_list_init()
{
	on_level = __level_load_event;

	// Open the scene list.
	scene_new();

	// Creates the scene nodes.
	day[0] = scene_data_new("Fragment #1",                              // Name of the chapter.
								_chr(game_asset_get_scene("fragment1.wmb")), // Internal name of the chapter. (scene name)
								NULL,                                        // Name of the thumbnail image.
								NULL,                                        // Sun color.
								NULL,                                        // Fog color.
								NULL,                                        // Fog range.
								150.0);                                      // Sun light intensity.

	day[1] = scene_data_new("Fragment #2", _chr(game_asset_get_scene("fragment2.wmb")), NULL, NULL, NULL, NULL, 150.0);
	day[2] = scene_data_new("Fragment #3", _chr(game_asset_get_scene("fragment3.wmb")), NULL, NULL, NULL, NULL, 150.0);
	day[3] = scene_data_new("Fragment #4", _chr(game_asset_get_scene("fragment4.wmb")), NULL, NULL, NULL, NULL, 150.0);
	day[4] = scene_data_new("Fragment #5", _chr(game_asset_get_scene("fragment5.wmb")), NULL, NULL, NULL, NULL, 150.0);
	day[5] = scene_data_new("Fragment #6", _chr(game_asset_get_scene("fragment6.wmb")), NULL, NULL, NULL, NULL, 150.0);
	day[6] = scene_data_new("Fragment #7", _chr(game_asset_get_scene("empty.wmb")), NULL, NULL, NULL, NULL, 150.0);

	int i = 0;
	for(; i < TOTAL_DAYS; i++)
		scene_add( day[i] );
}

// Picks a language and perform string initialization based on the chosen language.
void nov_region_init(const char *language)
{
	ASSERT(language != NULL, "Crash in nov_region_init() due to empty parameters being passed. This should not happen!");

	// Detect supported languages.
	region_new();
	region_scan();

	bool b = region_set_language(language);
	if(b) {
		game_log_write( str_printf( NULL, "Language successfully set to %s", _chr(region_get_language()) ) );
	}

	localized_init();
}

// Initializes static variables/subsystems.
void nov_gui_static_init()
{
	// After setting up the GUI states, we can tweak a few knobs to customize
	// its default behavior, like setting up the default loading screen, and the reticule.
	#ifndef    UI_LESS
		game_gui_set_reticule( game_asset_get_gui("reticule.bmp") );
	#endif

	game_gui_set_paper_texture( game_asset_get_gui("paper.jpg") );

	game_scene_set_load_screen("loading_image.bmp");
	game_scene_set_delay(0.0);
	game_scene_set_fade_speed(4.5);
	game_scene_set_fade(false);

	float f = screen_size.y / 2.0;

	game_scene_set_load_text_pos(15.0, f);
	game_scene_set_desc_text_pos(48.0, f + 42.0);

	game_scene_set_desc_text_font("Times#25");

	screenshot_set_name("Game");
	on_f12 = screenshot;

	game_static_init();
}

// Initializes subsystems (game state, GUI, PhysX, ...)
void nov_modules_init()
{
	// Initialize a new game state.
	game_state_new();

	// ...and write an additional line if the game was started for the first time.
	if(game_is_first_time())
		game_log_write("First time booting the game.");

	// Load resources. In the final, completed game, resources are loaded from add_resource().
	game_resources_load();

	// Continue to initialize the remaining subsystems. (PhysX engine, music player and the GUI state).
	game_physx_new();
	game_gui_state_new();
	game_mplayer_new("./sound/stream/", "*.ogg");

	// Post-initialization.
	// gui_button_set_event(__GUIState_singleton->intro_lang_vn_button, EVENT_BUTTON_PUSH, NULL, __ui_flag_vn_event);
	// gui_button_set_event(__GUIState_singleton->intro_lang_en_button, EVENT_BUTTON_PUSH, NULL, __ui_flag_en_event);
}

/*
 * int main(int argc, char **argl)
 *
 * GiN entry point. Initialize variables, structs, video devices, parsing
 * configuration files, setting up parameters, booting up subsystems,
 * and handling the main game loop, as well as cleaning up.
 */
int main(int argc, char **argl)
{
	const char *__err_launcher = "Please run the game through the launcher!";
	const char *__err_language = "Language list cannot be found! Run generate_language_list.exe to let the language definition be generated once.";
	const char *__wrn_lowres   = "The game requires a monitor with a resolution of at least 1,024 x 768. The game will still run, but certain GUI elements won't be displayed correctly.";
	const char *__wrn_lowram   = "You need at least 3 GB RAM in order to play the game. If you're having less than 3 GB RAM, clicking \"OK\" will force the game to run, but it may causes unexpected issues. If you want to quit now, open Task Manager and close the game process.\n\nSorry for the inconvenience.";
	const char *__wrn_redist   = "One or more required DLLs for music playback cannot be found.\n\nOn Steam, try verifying game files and make sure you've installed the redistributable files before launching the game.\n\nitch.io users can launch the game through Launcher.bat.";

	// See if the game was launched through the Go-based launcher.
	#ifndef    DEBUG
		// ASSERT(game_locker_check() != 0, __err_launcher);
	#endif

	if( sys_metrics(0) < 1024.0 && sys_metrics(1) < 768.0 )
		printf(__wrn_lowres);

	if( os_get_ram(S_MB) < 3072 )
		printf(__wrn_lowram);

	String *temp = "";
	str_cat(temp, os_get_system_directory());
	str_cat(temp, "ogg.dll");

	if( !file_exists(temp) )
	{
		printf(__wrn_redist);
	}

	#ifndef    DEBUG
		ASSERT(file_exists("./translation/__language.pad") != 0, __err_language);
	#else
		if( !file_exists("./translation/__language.pad") )
		{
			printf(__err_language);

			sys_exit((void *) 0);
		}
	#endif

	// Overrides some of the global variables (their default values are way too low)
	game_globals_set();

	// mouse_lock(true);

	// Wait for the video device.
	while( !ready() )
		wait(1.0);

	// After the video device is initialized, parse the video configuration file
	// and apply video settings to the current video device, and set the new game title.
	game_video_cfg_parse();
	game_title_set();

	// Exports the game version to the game folder. This is used by the external launcher
	// which uses the file for updating purposes. The completed, released version should
	// have a versioning file ready already.
	__game_version_export();

	// Pops up the GUI to let the user chooses their desired language.
	// After that, a call to nov_region_init() will be invoked to pick the chosen language and perform string initialization.
	nov_region_init("en");

	// Initializes subsystems (game state, GUI, PhysX, ...)
	nov_modules_init();

	// Static initialization.
	nov_gui_static_init();

	// Initializes the scene list, the scenes, and push them into the list.
	nov_scene_list_init();

	game_gui_set_state(STATE_INTRO);
	game_gui_render();

	Vector3 tx;
	tx.x = screen_size.x - 150.0;
	tx.y = 15.0;
	tx.z = 0;

	Text *todo = txt_create(1, LAYER_DEBUG_1);
	todo->font = font_create("UVN remind#30b");

	gui_text_set_color(todo, COLOR_DARK_GREY);
	gui_text_set_pos(todo, __GUIState_singleton->todo_texture->pos_x + 10.0, __GUIState_singleton->todo_texture->pos_y + 10.0);
	gui_text_set_pos(txtSubtitleHandler, (screen_size.x / 2.0) - 156.0, __GUIState_singleton->todo_texture->pos_y + bmap_height(__GUIState_singleton->todo_texture->bmap) + 25.0);

	// Main game loop, which can be terminated with the "Alt + F4" key.
	while(!(__GameState_singleton->exit_switch))
	{
		__GameState_singleton->exit_switch = key_alt && key_f4;

		// Updates PhysX.
		game_physx_loop();

		// Updates the GUI state.
		game_gui_update();

		// Continuously update the area the player is currently in.
		#ifndef    UI_LESS
			if( STATE_NULL == game_gui_get_state() && game_intro_done ) // Because draw_*() functions don't take layers into account,
			                                                            // I have to rely a lot on GUI states in order to manage rendering orders of GUI objects.
			{
				draw_text(game_region_check(), tx.x, tx.y, COLOR_WHITE);
			}
		#endif

		// Updates FOV.
		if( NULL != camera )
		{
			if( !var_cmp(fov, camera->arc) )
				camera->arc = fov;
		}

		// Updates the global volume.
		if( !var_cmp(volume, game_volume) )
			game_volume = volume;

		// Updates the mouse sensivity.
		mickey.x *= sensivity_mul;
		mickey.y *= sensivity_mul;

		if( key_f1 )
		{
			while ( key_f1 ) wait(1.0);
				switch( __invert_y )
				{
					case    1: __invert_y = -1; break;
					case   -1: __invert_y = 1;
				}
		}

		if( key_tab && (STATE_NULL == game_gui_get_state()) )
		{
			switch ( game_day_get() )
			{
				case DAY_3:
				case DAY_4:
				case DAY_5:
				case DAY_6:
				{
					if( (todo->pstring)[0] != lstr_todo_c3 )
						(todo->pstring)[0] = lstr_todo_c3;
					break;
				}

				case DAY_1:
				{
					draw_obj( __GUIState_singleton->todo_texture );

					if( (todo->pstring)[0] != lstr_todo_c1 )
						(todo->pstring)[0] = lstr_todo_c1;
					break;
				}

				case DAY_2:
				{
					draw_obj( __GUIState_singleton->todo_texture );

					if( (todo->pstring)[0] != lstr_todo_c2 )
						(todo->pstring)[0] = lstr_todo_c2;
				}
			}

			draw_obj( todo );
		}

		if( key_esc && __can_press_esc && (game_gui_get_state() != STATE_ENDING) )
		{
			while(key_esc) wait(1.0);

			wait(1.0);

			__GameState_singleton->menu_switch = 1 - __GameState_singleton->menu_switch;

			switch(__GameState_singleton->menu_switch)
			{
				case    1:    game_gui_set_state(STATE_MAIN_MENU); game_gui_render(); break;
				case    0:    game_gui_set_state(STATE_NULL);      game_gui_render();
			}
		}

		wait(1.0);
	}

	RETURN(0);
}
