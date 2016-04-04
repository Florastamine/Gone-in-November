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

#define    MAX_SCENES    5

ChapterData *Chapters[MAX_SCENES];

// Initializes the scene list, the scenes, and push them into the list.
void nov_scene_list_init()
{
	on_level = __level_load_event;

	// Open the scene list.
	scene_new();

	// Creates the scene nodes.
	Chapters[0] = scene_data_new("Fragment #1",                              // Name of the chapter.
								_chr(game_asset_get_scene("fragment1.wmb")), // Internal name of the chapter. (scene name)
								NULL,                                        // Name of the thumbnail image.
								NULL,                                        // Sun color.
								NULL,                                        // Fog color.
								NULL,                                        // Fog range.
								150.0);                                      // Sun light intensity.

	Chapters[1] = scene_data_new("Fragment #2", _chr(game_asset_get_scene("fragment2.wmb")), NULL, NULL, NULL, NULL, 150.0);
	Chapters[2] = scene_data_new("Fragment #3", _chr(game_asset_get_scene("fragment3.wmb")), NULL, NULL, NULL, NULL, 150.0);
	Chapters[3] = scene_data_new("Fragment #4", _chr(game_asset_get_scene("fragment4.wmb")), NULL, NULL, NULL, NULL, 150.0);
	Chapters[4] = scene_data_new("Fragment #5", _chr(game_asset_get_scene("fragment5.wmb")), NULL, NULL, NULL, NULL, 150.0);

	int i = 0;
	for(; i < MAX_SCENES; i++)
		scene_add(Chapters[i]);
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

		#ifdef    DEBUG
			printf("Language selected: %s", language);
		#endif
	}

	localized_init();
}


// Initializes static variables/subsystems.
void nov_gui_static_init()
{
	// After setting up the GUI states, we can tweak a few knobs to customize
	// its default behavior, like setting up the default loading screen, and the reticule.
	#ifdef    DEBUG
		#ifndef    UI_LESS
			game_gui_set_reticule( game_asset_get_gui("reticule.bmp") );
		#endif
	#endif

	game_gui_set_paper_texture( game_asset_get_gui("paper.jpg") );

	game_scene_set_load_screen("image.jpg");
	game_scene_set_delay(3.0);
	game_scene_set_fade_speed(4.5);
	game_scene_set_fade(false);

	game_scene_set_load_text_pos(10.0, 10.0);
	game_scene_set_desc_text_pos(50.0, 125.0);
	game_scene_set_load_text("Custom loading text");
	game_scene_set_desc_text_font("Times#25");
}

__static void __ui_flag_vn_event() {
    nov_region_init("vn");

	game_gui_set_state(GUI_MAIN_MENU);
	game_gui_render();

	// Loads a chapter from the list, with a custom scene loader (game_scene_load()).
	scene_load(Chapters[0], level_load);
}

__static void __ui_flag_en_event() {
    nov_region_init("en");

	game_gui_set_state(GUI_MAIN_MENU);
	game_gui_render();

	scene_load(Chapters[0], level_load);
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

	// Continue to initialize the remaining subsystems. (PhysX engine and the GUI state).
	game_physx_new();
	game_gui_state_new();

	// Post-initialization.
	gui_button_set_event(__GUIState_singleton->intro_lang_vn_button, EVENT_BUTTON_PUSH, NULL, __ui_flag_vn_event);
	gui_button_set_event(__GUIState_singleton->intro_lang_en_button, EVENT_BUTTON_PUSH, NULL, __ui_flag_en_event);
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

	// See if the game was launched through the Go-based launcher.
	#ifndef    DEBUG
		ASSERT(game_locker_check() != 0, __err_launcher);
	#endif

	ASSERT(file_exists("./translation/__language.pad") != 0, __err_language);

	// Overrides some of Acknex's global variables (their default values are way too low)
	game_globals_set();

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

	// Static initialization.
	game_static_init();

	// Initializes subsystems (game state, GUI, PhysX, ...)
	nov_modules_init();

	// Initializes static variables/subsystems.
	nov_gui_static_init();

	// Initializes the scene list, the scenes, and push them into the list.
	nov_scene_list_init();

	// Pops up the GUI to let the user chooses their desired language.
	// After that, a call to nov_region_init() will be invoked to pick the chosen language and perform string initialization.
	game_gui_set_state(GUI_INTRO);
	game_gui_render();

	// If the video card does meet the game's required version of vertex/pixel shaders.
/*	if( game_psvs_test() )
	{
		render_new();

		// Sets up HDR.

		// Sets up light shafts.
		render_light_rays_new();
		render_light_rays_setup(1.7, 8.4);

		render_light_rays();

		#ifdef    DEBUG
			render_light_rays_set_debug();
		#endif
	}

	// Creates a sky cube which "wraps" around the scene.
	Object *cube = object_sky_create( game_asset_get_2d_sprite("noon+6.tga"), 1 );

	// Shows the GUI.
	game_gui_render();

	const char *text = "I wonder how she's doing lately.\nWe haven't seen each other for so long...";

	StaticTitleText *title = gui_title_new( vector(20.0, screen_size.y - 150.0, 0.0) , COLOR_BLEU_DE_FRANCE, text, 5.0, 15 );
	gui_title_set_sound( title, 1, _chr(game_asset_get_sound("laptop_notebook_delete_key_press.wav")) );
	gui_title_set_sound( title, 2, _chr(game_asset_get_sound("laptop_notebook_key_press.wav")) );
	gui_title_set_sound( title, 3, _chr(game_asset_get_sound("laptop_notebook_return_or_enter_key_press.wav")) );
	gui_title_set_sound( title, 4, _chr(game_asset_get_sound("laptop_notebook_spacebar_press.wav")) );
	gui_credits_set_font(title, "avery.bmp");
	// gui_title_show(title);

	// Activates PSSM shadows (four passes) and renders fog.
	// pssm_run(4);

	gui_open_eyes(6.5, 1);

	game_fog_set(1, vector(189.0, 238.0, 240.0), vector(15.0, 12424.0, 0.0)); // Set the fog color and its range to the first (1) fog color slot.
	game_fog_render(1); // Merely sets fog_color to ID.
*/
	Vector3 tx;
	tx.x = screen_size.x - 150.0;
	tx.y = 15.0;
	tx.z = 0;

	// Main game loop, which can be terminated with the "ESC" key.
	while( !key_esc )
	{
		// Update PhysX.
		game_physx_loop();

		// Continuously update the area the player is currently in.
		#ifndef    UI_LESS
			draw_text(game_region_check(), tx.x, tx.y, COLOR_WHITE);
		#endif

		vec_set( &mouse_pos, &mouse_cursor );

		wait(1.0);
	}

	RETURN(0);
}
