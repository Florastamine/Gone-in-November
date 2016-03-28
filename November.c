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
#include "November.h"
#include "Unmanaged.h"

/*
 * int main(int argc, char **argl)
 *
 * GiN entry point. Initialize variables, structs, video devices, parsing
 * configuration files, setting up parameters, booting up subsystems,
 * and handling the main game loop, as well as cleaning up.
 */
int main(int argc, char **argl)
{
	// See if the game was launched through the Go-based launcher.
	// ASSERT(game_locker_check() != 0, "Please run the game through the launcher!");
	ASSERT(file_exists("./translation/__language.pad") != 0, "Language list cannot be found!");

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

	on_level = __level_load_event;

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

	// Detect supported languages.
	const char *s_language = "en";

	region_new();
	region_scan();
	bool b = region_set_language(s_language);

	if(b) {
		game_log_write(str_printf(NULL, "Language successfully set to %s", region_get_language()));
	}

	localized_init();

	// After setting up the GUI states, we can tweak a few knobs to customize
	// its default behavior, like setting up the default loading screen, and the reticule.
	#ifdef    DEBUG
		game_gui_set_reticule( game_asset_get_gui("reticule.bmp") );
	#endif

	game_scene_set_load_screen("image.jpg");
	game_scene_set_delay(3.0);
	game_scene_set_fade_speed(4.5);
	game_scene_set_fade(false);

	game_scene_set_load_text_pos(10.0, 10.0);
	game_scene_set_desc_text_pos(50.0, 125.0);
	game_scene_set_load_text("Custom loading text");
	game_scene_set_desc_text_font("Times#25");

	// Creates a new chapter and push it to the scene list.
	scene_new();

	ChapterData *ChapterOne = scene_data_new("359",                               // Name of the chapter.
	                                        _chr(game_asset_get_scene("scene.wmb")),   // Internal name of the chapter. (scene name)
											NULL,                                      // Name of the thumbnail image.
											NULL,                                      // Sun color.
											NULL,                                      // Fog color.
											NULL,                                      // Fog range.
											150.0);                                    // Sun light intensity.
	scene_add(ChapterOne);

	// Loads a chapter from the list, with a custom scene loader (game_scene_load()).
	scene_load(ChapterOne, level_load);
	pXent_setgroup(level_ent, LEVEL_GROUP);

	// Activates PSSM shadows (four passes) and renders fog.
	// pssm_run(4);

	game_fog_set(1, vector(189.0, 238.0, 240.0), vector(15.0, 12424.0, 0.0)); // Set the fog color and its range to the first (1) fog color slot.
	game_fog_render(1); // Merely sets fog_color to ID.

	// If the video card does meet the game's required version of vertex/pixel shaders.
	if( game_psvs_test() )
	{
		render_new();

		// Sets up HDR.
		render_setup_rt();
		render_hdr_new();
		render_hdr_set_queued(true);

		(render_hdr_get_singleton())->bloom_strength *= 0.5;

		render_hdr();

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
	gui_title_set_sound( title, _chr(game_asset_get_sound("typewriter-key-1.wav")) );
	gui_credits_set_font(title, "avery.bmp");
	gui_title_show(title);

	// Main game loop, which can be terminated with the "ESC" key.
	while( !key_esc )
	{
		game_physx_loop();

		wait(1.0);
	}

	RETURN(0);
}
