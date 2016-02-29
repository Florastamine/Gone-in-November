/* gui_menu.c */
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
 * __________________________________________________________________
 *
 * <gui_menu>
 * This is a very simple menu managing code written by 3run, which first originated from
 * the TUST project (https://github.com/MasterQ32/TUST). Basically too lazy and want to save
 * some time to focus on other parts of the project, I decided to take his code, simplified, fixed,
 * corrected and improved it to fit to my project.
 * With the new changes, now you can use a text file (just pass it to menu_init()) which defines
 * the strings in the various menus' components. This way you can have a multilingual menu.
 * Another thing is you can alter the menu theme now.
 * __________________________________________________________________
 */
__static BMAP *bmap_create_ext(float w, float h, int f, VECTOR *c)
{
	BMAP *b = bmap_createblack(abs(w), abs(h), f);
	bmap_fill(b, c, 100.0);

	return b;
}

void menu_init_buttons()
{
	game_log_write("Initializing GUI... [// ]");

	#ifdef __GUI_IP_EX__
		if (!bmapMenuButtonOff)
		{
			bmapMenuButtonOff = bmap_create_ext(MENU_BUTTON_SIZE_X,MENU_BUTTON_SIZE_Y,24, __gui_color_theme__);
			bmapMenuButtonOff = bmap_draw_border(bmapMenuButtonOff, 0, 0, __gui_color_border__, 1);
		}

		if (!bmapMenuButtonOn)
		{
			bmapMenuButtonOn = bmap_create_ext(MENU_BUTTON_SIZE_X,MENU_BUTTON_SIZE_Y,24, __gui_color_click__);
			bmapMenuButtonOn = bmap_draw_border(bmapMenuButtonOn, 0, 0, __gui_color_border__, 1);
		}

		if (!bmapOptionsChoiceBg)
		{
			bmapOptionsChoiceBg = bmap_create_ext(MENU_BUTTON_SIZE_X * 4 + MENU_BUTTON_GAP * 5, MENU_BUTTON_SIZE_Y + MENU_BUTTON_GAP * 2, 24, __gui_color_theme__);
			bmapOptionsChoiceBg = bmap_draw_border(bmapOptionsChoiceBg, 0, 0, __gui_color_border__, 1);
		}

		if (!bmapOptionsBg)
		{
			// If the define is set to -1, the size of the options choice bar
			// is used to determine the width of the options window.
			if (MENU_OPTIONS_SIZE_X == -1)
				bmapOptionsBg = bmap_create_ext(bmap_width(bmapOptionsChoiceBg), MENU_OPTIONS_SIZE_Y, 24, __gui_color_theme__);
			else
				bmapOptionsBg = bmap_create_ext(MENU_OPTIONS_SIZE_X, MENU_OPTIONS_SIZE_Y, 24, __gui_color_theme__);

			bmapOptionsBg = bmap_draw_border(bmapOptionsBg, 0, 0, __gui_color_border__, 1);
		}

		if (bmapSliderKnob == NULL)
		{
			bmapSliderKnob = bmap_create_ext(MENU_KNOB_SIZE_X, MENU_KNOB_SIZE_Y, 24, __gui_color_click__);
			bmapSliderKnob = bmap_draw_border(bmapSliderKnob, 0, 0, __gui_color_border__, 1);
		}

		if (bmapSliderBg == NULL)
		{
			bmapSliderBg = bmap_create_ext(MENU_SLIDER_SIZE_X, MENU_SLIDER_SIZE_Y, 24, __gui_color_theme__);
			bmapSliderBg = bmap_draw_border(bmapSliderBg, 0, 0, __gui_color_border__, 1);
		}

		if (bmapCheckBoxCheckedOn == NULL)
		{
			bmapCheckBoxCheckedOn = bmap_create_ext(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24, __gui_color_click__);
			bmapCheckBoxCheckedOn = bmap_draw_border(bmapCheckBoxCheckedOn, 0, 0, __gui_color_border__, 1);
		}

		if (bmapCheckBoxCheckedOff == NULL)
		{
			bmapCheckBoxCheckedOff = bmap_create_ext(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24, __gui_color_click__);
			bmapCheckBoxCheckedOff = bmap_draw_border(bmapCheckBoxCheckedOff, 0, 0, __gui_color_border__, 1);
		}

		if (bmapCheckBoxUncheckedOn == NULL)
		{
			bmapCheckBoxUncheckedOn = bmap_create_ext(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24, __gui_color_click__);
			bmapCheckBoxUncheckedOn = bmap_draw_border(bmapCheckBoxUncheckedOn, 0, 0, __gui_color_border__, 1);
		}

		if (bmapCheckBoxUncheckedOff == NULL)
		{
			bmapCheckBoxUncheckedOff = bmap_create_ext(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24, __gui_color_theme__);
			bmapCheckBoxUncheckedOff = bmap_draw_border(bmapCheckBoxUncheckedOff, 0, 0, __gui_color_border__, 1);
		}

		// Combobox
		if (bmapComboboxOn == NULL)
		{
			bmapComboboxOn = bmap_create_ext(MENU_COMBOBOX_SIZE_X, MENU_COMBOBOX_SIZE_Y, 24, __gui_color_theme__);
			bmapComboboxOn = bmap_draw_border(bmapComboboxOn, 0, 0, __gui_color_border__, 1);
		}

		if (bmapComboboxOff == NULL) {
			bmapComboboxOff = bmap_create_ext(MENU_COMBOBOX_SIZE_X, MENU_COMBOBOX_SIZE_Y, 24, __gui_color_click__);
			bmapComboboxOff = bmap_draw_border(bmapComboboxOff, 0, 0, __gui_color_click__, 1);
		}
	#else
		var vFormat;
		var vPixel;
		int i;

		// Create all images manually
		if (bmapMenuButtonOff == NULL)
		{
			bmapMenuButtonOff = bmap_createblack(MENU_BUTTON_SIZE_X,MENU_BUTTON_SIZE_Y,24);
			var vFormat = bmap_lock(bmapMenuButtonOff, 0);
			var vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			int i;
			// Draw Borders
			for (i=0; i<bmap_width(bmapMenuButtonOff); i++) {
				pixel_to_bmap(bmapMenuButtonOff, i, 0, vPixel);
				pixel_to_bmap(bmapMenuButtonOff, i, bmap_height(bmapMenuButtonOff)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapMenuButtonOff); i++) {
				pixel_to_bmap(bmapMenuButtonOff, 0, i, vPixel);
				pixel_to_bmap(bmapMenuButtonOff, bmap_width(bmapMenuButtonOff)-1, i, vPixel);
			}
			bmap_unlock(bmapMenuButtonOff);
		}

		if (bmapMenuButtonOn == NULL)
		{
			bmapMenuButtonOn = bmap_createblack(MENU_BUTTON_SIZE_X,MENU_BUTTON_SIZE_Y,24);
			bmap_fill(bmapMenuButtonOn, __gui_color_click__, 100);
			vFormat = bmap_lock(bmapMenuButtonOn, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);

			// Draw Borders
			for (i=0; i<bmap_width(bmapMenuButtonOn); i++) {
				pixel_to_bmap(bmapMenuButtonOn, i, 0, vPixel);
				pixel_to_bmap(bmapMenuButtonOn, i, bmap_height(bmapMenuButtonOn)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapMenuButtonOn); i++) {
				pixel_to_bmap(bmapMenuButtonOn, 0, i, vPixel);
				pixel_to_bmap(bmapMenuButtonOn, bmap_width(bmapMenuButtonOn)-1, i, vPixel);
			}
			bmap_unlock(bmapMenuButtonOn);
		}

		// Create options elements
		if (bmapOptionsChoiceBg == NULL)
		{
			bmapOptionsChoiceBg = bmap_createblack(MENU_BUTTON_SIZE_X * 4 + MENU_BUTTON_GAP * 5, MENU_BUTTON_SIZE_Y + MENU_BUTTON_GAP * 2, 24);
			vFormat = bmap_lock(bmapOptionsChoiceBg, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapOptionsChoiceBg); i++) {
				pixel_to_bmap(bmapOptionsChoiceBg, i, 0, vPixel);
				pixel_to_bmap(bmapOptionsChoiceBg, i, bmap_height(bmapOptionsChoiceBg)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapOptionsChoiceBg); i++) {
				pixel_to_bmap(bmapOptionsChoiceBg, 0, i, vPixel);
				pixel_to_bmap(bmapOptionsChoiceBg, bmap_width(bmapOptionsChoiceBg)-1, i, vPixel);
			}
			bmap_unlock(bmapOptionsChoiceBg);
		}

		// Create the background for the options menues
		if (bmapOptionsBg == NULL)
		{
			// If the define is set to -1, the size of the options choice bar
			// is used to determine the width of the options window.
			if (MENU_OPTIONS_SIZE_X == -1)
				bmapOptionsBg = bmap_createblack(bmap_width(bmapOptionsChoiceBg), MENU_OPTIONS_SIZE_Y, 24);
			else
				bmapOptionsBg = bmap_createblack(MENU_OPTIONS_SIZE_X, MENU_OPTIONS_SIZE_Y, 24);

			vFormat = bmap_lock(bmapOptionsBg, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapOptionsBg); i++) {
				pixel_to_bmap(bmapOptionsBg, i, 0, vPixel);
				pixel_to_bmap(bmapOptionsBg, i, bmap_height(bmapOptionsBg)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapOptionsBg); i++) {
				pixel_to_bmap(bmapOptionsBg, 0, i, vPixel);
				pixel_to_bmap(bmapOptionsBg, bmap_width(bmapOptionsBg)-1, i, vPixel);
			}
			bmap_unlock(bmapOptionsBg);
		}

		// Create Knob and Slider bitmaps
		if (bmapSliderKnob == NULL)
		{
			bmapSliderKnob = bmap_createblack(MENU_KNOB_SIZE_X, MENU_KNOB_SIZE_Y, 24);
			bmap_fill(bmapSliderKnob, __gui_color_click__, 100);
			vFormat = bmap_lock(bmapSliderKnob, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapSliderKnob); i++)
			{
				pixel_to_bmap(bmapSliderKnob, i, 0, vPixel);
				pixel_to_bmap(bmapSliderKnob, i, bmap_height(bmapSliderKnob)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapSliderKnob); i++)
			{
				pixel_to_bmap(bmapSliderKnob, 0, i, vPixel);
				pixel_to_bmap(bmapSliderKnob, bmap_width(bmapSliderKnob)-1, i, vPixel);
			}
			bmap_unlock(bmapSliderKnob);
		}

		if (bmapSliderBg == NULL) {
			bmapSliderBg = bmap_createblack(MENU_SLIDER_SIZE_X, MENU_SLIDER_SIZE_Y, 24);
			vFormat = bmap_lock(bmapSliderBg, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapSliderBg); i++) {
				pixel_to_bmap(bmapSliderBg, i, 0, vPixel);
				pixel_to_bmap(bmapSliderBg, i, bmap_height(bmapSliderBg)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapSliderBg); i++) {
				pixel_to_bmap(bmapSliderBg, 0, i, vPixel);
				pixel_to_bmap(bmapSliderBg, bmap_width(bmapSliderBg)-1, i, vPixel);
			}
			bmap_unlock(bmapSliderBg);
		}

		// Checkbox
		if (bmapCheckBoxCheckedOn == NULL) {
			bmapCheckBoxCheckedOn = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
			bmap_fill(bmapCheckBoxCheckedOn, __gui_color_click__, 100);
			vFormat = bmap_lock(bmapCheckBoxCheckedOn, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapCheckBoxCheckedOn); i++) {
				pixel_to_bmap(bmapCheckBoxCheckedOn, i, 0, vPixel);
				pixel_to_bmap(bmapCheckBoxCheckedOn, i, bmap_height(bmapCheckBoxCheckedOn)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapCheckBoxCheckedOn); i++) {
				pixel_to_bmap(bmapCheckBoxCheckedOn, 0, i, vPixel);
				pixel_to_bmap(bmapCheckBoxCheckedOn, bmap_width(bmapCheckBoxCheckedOn)-1, i, vPixel);
			}
			bmap_unlock(bmapCheckBoxCheckedOn);
		}

		if (bmapCheckBoxCheckedOff == NULL) {
			bmapCheckBoxCheckedOff = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
			bmap_fill(bmapCheckBoxCheckedOff, __gui_color_click__, 100);
			vFormat = bmap_lock(bmapCheckBoxCheckedOff, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapCheckBoxCheckedOff); i++) {
				pixel_to_bmap(bmapCheckBoxCheckedOff, i, 0, vPixel);
				pixel_to_bmap(bmapCheckBoxCheckedOff, i, bmap_height(bmapCheckBoxCheckedOff)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapCheckBoxCheckedOff); i++) {
				pixel_to_bmap(bmapCheckBoxCheckedOff, 0, i, vPixel);
				pixel_to_bmap(bmapCheckBoxCheckedOff, bmap_width(bmapCheckBoxCheckedOff)-1, i, vPixel);
			}
			bmap_unlock(bmapCheckBoxCheckedOff);
		}

		if (bmapCheckBoxUncheckedOn == NULL) {
			bmapCheckBoxUncheckedOn = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
			bmap_fill(bmapCheckBoxUncheckedOn, __gui_color_click__, 100);
			vFormat = bmap_lock(bmapCheckBoxUncheckedOn, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapCheckBoxUncheckedOn); i++) {
				pixel_to_bmap(bmapCheckBoxUncheckedOn, i, 0, vPixel);
				pixel_to_bmap(bmapCheckBoxUncheckedOn, i, bmap_height(bmapCheckBoxUncheckedOn)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapCheckBoxUncheckedOn); i++) {
				pixel_to_bmap(bmapCheckBoxUncheckedOn, 0, i, vPixel);
				pixel_to_bmap(bmapCheckBoxUncheckedOn, bmap_width(bmapCheckBoxUncheckedOn)-1, i, vPixel);
			}
			bmap_unlock(bmapCheckBoxUncheckedOn);
		}

		if (bmapCheckBoxUncheckedOff == NULL) {
			bmapCheckBoxUncheckedOff = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
			vFormat = bmap_lock(bmapCheckBoxUncheckedOff, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapCheckBoxUncheckedOff); i++) {
				pixel_to_bmap(bmapCheckBoxUncheckedOff, i, 0, vPixel);
				pixel_to_bmap(bmapCheckBoxUncheckedOff, i, bmap_height(bmapCheckBoxUncheckedOff)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapCheckBoxUncheckedOff); i++) {
				pixel_to_bmap(bmapCheckBoxUncheckedOff, 0, i, vPixel);
				pixel_to_bmap(bmapCheckBoxUncheckedOff, bmap_width(bmapCheckBoxUncheckedOff)-1, i, vPixel);
			}
			bmap_unlock(bmapCheckBoxUncheckedOff);
		}

		// Combobox
		if (bmapComboboxOn == NULL) {
			bmapComboboxOn = bmap_createblack(MENU_COMBOBOX_SIZE_X, MENU_COMBOBOX_SIZE_Y, 24);
			vFormat = bmap_lock(bmapComboboxOn, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapComboboxOn); i++) {
				pixel_to_bmap(bmapComboboxOn, i, 0, vPixel);
				pixel_to_bmap(bmapComboboxOn, i, bmap_height(bmapComboboxOn)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapComboboxOn); i++) {
				pixel_to_bmap(bmapComboboxOn, 0, i, vPixel);
				pixel_to_bmap(bmapComboboxOn, bmap_width(bmapComboboxOn)-1, i, vPixel);
			}
			bmap_unlock(bmapComboboxOn);
		}

		if (bmapComboboxOff == NULL)
		{
			bmapComboboxOff = bmap_createblack(MENU_COMBOBOX_SIZE_X, MENU_COMBOBOX_SIZE_Y, 24);
			bmap_fill(bmapComboboxOff, __gui_color_click__, 100);
			vFormat = bmap_lock(bmapComboboxOff, 0);
			vPixel = pixel_for_vec(COLOR_WHITE, 100, vFormat);
			// Draw Borders
			for (i=0; i<bmap_width(bmapComboboxOff); i++) {
				pixel_to_bmap(bmapComboboxOff, i, 0, vPixel);
				pixel_to_bmap(bmapComboboxOff, i, bmap_height(bmapComboboxOff)-1, vPixel);
			}
			for (i=0; i<bmap_height(bmapComboboxOff); i++) {
				pixel_to_bmap(bmapComboboxOff, 0, i, vPixel);
				pixel_to_bmap(bmapComboboxOff, bmap_width(bmapComboboxOff)-1, i, vPixel);
			}
			bmap_unlock(bmapComboboxOff);
		}
	#endif
}

void menu_init(const char *language_file, const int unicodeness)
{
	game_log_write("Initializing GUI... [/  ]");

	if( !language_file || file_exists(language_file) == 0 )
	{
		game_log_write("Failed to initialize the GUI (string file cannot be found).");
		return;
	}

	Text *translation = txt_create(64, LAYER_DEBUG_1);   // Contains the current translation for the menu buttons.
	int c             = 0;
	int i             = 0;

	if(unicodeness) // Parses the translation based on unicodeness specification.
		c = txt_loadw(translation, language_file);
	else
		c = txt_load(translation, language_file);

	if(c > 0)
		game_log_write(str_printf(NULL, "%i strings in <%s> loaded.", c, language_file));
	else
		game_log_write("Failed to initialize the GUI (corrupt string file)");

	menu_init_buttons();

	game_log_write("Initializing GUI... [///]");
	// Now creating GUI elements.
	panStartMenu = pan_create(NULL, LAYER_GUI_1);
	vec_set(&panStartMenu->size_x, vector(MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP * 2, MENU_BUTTON_SIZE_Y * 5 + MENU_BUTTON_GAP * 6, 0));
	pan_setbutton(panStartMenu, 0, 0, 0, 0, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_start_click, NULL, NULL); // New game

	for(i = 0; i < MENU_MAIN_MAX_ITEMS - 1; i++)
		pan_setbutton(panStartMenu, 0, 0, 0, (MENU_BUTTON_SIZE_Y  + MENU_BUTTON_GAP) * (i + 1), bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_start_click, NULL, NULL);

	// In game menu
	panInGameMenu = pan_create(NULL, LAYER_GUI_1);
	vec_set(&panInGameMenu->size_x, vector(MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP * 4, MENU_BUTTON_SIZE_Y * 6 + MENU_BUTTON_GAP * 7, 0));
	pan_setbutton(panInGameMenu, 0, 0, 0, 0, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL); // New game

	for(i = 0; i < MENU_IN_GAME_MAX_ITEMS - 1; i++)
		pan_setbutton(panInGameMenu, 0, 0, 0,(MENU_BUTTON_SIZE_Y + MENU_BUTTON_GAP) * (i + 1), bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL);

	// Options Menu on top
	panOptionsMenu = pan_create(NULL, LAYER_GUI_1);
	panOptionsMenu->bmap = bmapOptionsChoiceBg;
	// TODO: Should not be necessary!
	vec_set(&panOptionsMenu->size_x, vector(bmap_width(bmapOptionsChoiceBg), bmap_height(bmapOptionsChoiceBg), 0));
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_GAP, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_options_choose, NULL, NULL);

	for(i = 0; i < 2; i++)
		pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_SIZE_X * (i + 1) + MENU_BUTTON_GAP * 2, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_options_choose, NULL, NULL);

	// Game Options
	panOptionsGame = pan_create(NULL, LAYER_GUI_1);
	panOptionsGame->bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(&panOptionsGame->size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsGame, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_apply_click, NULL, NULL); // Apply
	pan_setbutton(panOptionsGame, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back

	for(i = 2; i <= 4; i++)
		pan_setbutton(panOptionsGame, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * i, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL);

	// Difficulty
	panOptionsGameDifficulty = pan_create(NULL, LAYER_GUI_1 + 1);
	i = 0;
	while(i < 160)
	{
		pan_setbutton(panOptionsGameDifficulty, 0, 4, i, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_difficulty_click, NULL, NULL);
		i += 80;
	}

	// Graphic Options
	panOptionsGraphics = pan_create(NULL, LAYER_GUI_1);
	panOptionsGraphics->bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(&panOptionsGraphics->size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsGraphics, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_apply_click, NULL, NULL); // Apply
	pan_setbutton(panOptionsGraphics, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back
	pan_setslider(panOptionsGraphics, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 3, bmapSliderBg, bmapSliderKnob, 0, 100, &vNewGameBrightness); // Brightness
	pan_setdigits(panOptionsGraphics, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 3, "%.0f", __gui_rendering_font, 1, &vNewGameBrightness); // Show brightness
	pan_setbutton(panOptionsGraphics, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 4, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Shader
	pan_setbutton(panOptionsGraphics, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 5, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Shadows

	// Details
	panOptionsGraphicsDetails = pan_create(NULL, LAYER_GUI_1 + 1);
	panOptionsGraphicsDetails->size_x = 160;
	panOptionsGraphicsDetails->size_y = 15;
	i = 0;
	while(i < 160)
	{
		pan_setbutton(panOptionsGraphicsDetails, 0, 4, i, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL);
		i += 80;
	}

	// Antialiasing
	panOptionsGraphicsAntiAliasing = pan_create(NULL, LAYER_GUI_1 + 1);
	panOptionsGraphicsAntiAliasing->size_x = 160;
	panOptionsGraphicsAntiAliasing->size_y = 15;
	i = 0;
	while(i < 150)
	{
		pan_setbutton(panOptionsGraphicsAntiAliasing, 0, 4, i, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_anti_aliasing_click, NULL, NULL);
		i += 50;
	}

	pan_setbutton(panOptionsGraphics, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 7, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Fullscreen

	// Audio Options
	panOptionsAudio = pan_create(NULL, LAYER_GUI_1);
	panOptionsAudio->bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(&panOptionsAudio->size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsAudio, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_apply_click, NULL, NULL); // Apply
	pan_setbutton(panOptionsAudio, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back
	pan_setslider(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 1, bmapSliderBg, bmapSliderKnob, 0, 100, &vNewGameMusicVolume); // Music volume
	pan_setdigits(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 1, "%.0f", __gui_rendering_font, 1, &vNewGameMusicVolume); // Show music volume
	pan_setslider(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 2, bmapSliderBg, bmapSliderKnob, 0, 100, &vNewGameSpeechVolume); // Speech volume
	pan_setdigits(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 2, "%.0f", __gui_rendering_font, 1, &vNewGameSpeechVolume); // Show speech volume
	pan_setslider(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 3, bmapSliderBg, bmapSliderKnob, 0, 100, &vNewGameEffectsVolume); // Effects volume
	pan_setdigits(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 3, "%.0f", __gui_rendering_font, 1, &vNewGameEffectsVolume); // Show effects volume
	pan_setbutton(panOptionsAudio, 0, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6, MENU_BUTTON_SIZE_Y * 1, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, menu_audio_test_box_click, NULL, NULL);
	pan_setbutton(panOptionsAudio, 0, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6, MENU_BUTTON_SIZE_Y * 2, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, menu_audio_test_box_click, NULL, NULL);
	pan_setbutton(panOptionsAudio, 0, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6, MENU_BUTTON_SIZE_Y * 3, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, menu_audio_test_box_click, NULL, NULL);

	// Create texts
	txtMenuNewGame							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuContinueGame						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuOptions							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuOptionsGame						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuOptionsGraphics					= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuOptionsAudio						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuOptionsApply						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuDifficulty						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuViolence							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuShowDialogs						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuShowHints						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuFullscreen						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuAntialias						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuBrightness						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuShader							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuShadows							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuDetails							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuCredits							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuExitGame							= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuReturnToWin						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuBack								= txt_create(1, LAYER_GUI_1 + 1);

	txtOptionsGameEasy						= txt_create(1, LAYER_GUI_1 + 1);
	txtOptionsGameMedium					= txt_create(1, LAYER_GUI_1 + 1);
	txtOptionsGameHard						= txt_create(1, LAYER_GUI_1 + 1);

	txtOptionsGraphicsDetailsLow			= txt_create(1, LAYER_GUI_1 + 1);
	txtOptionsGraphicsDetailsMedium			= txt_create(1, LAYER_GUI_1 + 1);
	txtOptionsGraphicsDetailsHigh			= txt_create(1, LAYER_GUI_1 + 1);

	txtOptionsGraphicsAAOff					= txt_create(1, LAYER_GUI_1 + 1);
	txtOptionsGraphicsAA1x					= txt_create(1, LAYER_GUI_1 + 1);
	txtOptionsGraphicsAA4x					= txt_create(1, LAYER_GUI_1 + 1);
	txtOptionsGraphicsAA9x					= txt_create(1, LAYER_GUI_1 + 1);

	txtMenuMusicVolume						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuSpeechVolume						= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuEffectsVolume					= txt_create(1, LAYER_GUI_1 + 1);

	txtMenuMusicVolumeTest					= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuSpeechVolumeTest					= txt_create(1, LAYER_GUI_1 + 1);
	txtMenuEffectsVolumeTest				= txt_create(1, LAYER_GUI_1 + 1);

	str_cpy((txtMenuNewGame->pstring)[0], (translation->pstring)[0]);
	str_cpy((txtMenuContinueGame->pstring)[0], (translation->pstring)[1]);
	str_cpy((txtMenuOptions->pstring)[0], (translation->pstring)[2]);
	str_cpy((txtMenuOptionsGame->pstring)[0], (translation->pstring)[3]);
	{
		str_cpy((txtMenuDifficulty->pstring)[0], (translation->pstring)[4]);
		{
			str_cpy((txtOptionsGameEasy->pstring)[0], (translation->pstring)[5]);
			str_cpy((txtOptionsGameMedium->pstring)[0], (translation->pstring)[6]);
			str_cpy((txtOptionsGameHard->pstring)[0], (translation->pstring)[7]);
		}
		str_cpy((txtMenuViolence->pstring)[0], (translation->pstring)[8]);
		str_cpy((txtMenuShowDialogs->pstring)[0], (translation->pstring)[9]);
		str_cpy((txtMenuShowHints->pstring)[0], (translation->pstring)[10]);
	}
	str_cpy((txtMenuOptionsGraphics->pstring)[0], (translation->pstring)[11]);
	{

		str_cpy((txtMenuAntialias->pstring)[0], (translation->pstring)[12]); // AntiAliasing
		{
			str_cpy((txtOptionsGraphicsAAOff->pstring)[0], (translation->pstring)[13]);
			str_cpy((txtOptionsGraphicsAA1x->pstring)[0], (translation->pstring)[14]);
			str_cpy((txtOptionsGraphicsAA4x->pstring)[0], (translation->pstring)[15]);
			str_cpy((txtOptionsGraphicsAA9x->pstring)[0], (translation->pstring)[16]);
		}
		str_cpy((txtMenuBrightness->pstring)[0], (translation->pstring)[17]);
		str_cpy((txtMenuShader->pstring)[0], (translation->pstring)[18]);
		str_cpy((txtMenuShadows->pstring)[0], (translation->pstring)[19]);
		str_cpy((txtMenuDetails->pstring)[0], (translation->pstring)[20]);
		{
			str_cpy((txtOptionsGraphicsDetailsLow->pstring)[0], (translation->pstring)[21]);
			str_cpy((txtOptionsGraphicsDetailsMedium->pstring)[0], (translation->pstring)[22]);
			str_cpy((txtOptionsGraphicsDetailsHigh->pstring)[0], (translation->pstring)[23]);
		}
		str_cpy((txtMenuFullscreen->pstring)[0], (translation->pstring)[24]);
	}
	str_cpy((txtMenuOptionsAudio->pstring)[0], (translation->pstring)[25]);
	{
		str_cpy((txtMenuMusicVolume->pstring)[0], (translation->pstring)[26]);
		str_cpy((txtMenuSpeechVolume->pstring)[0], (translation->pstring)[27]);
		str_cpy((txtMenuEffectsVolume->pstring)[0], (translation->pstring)[28]);

		str_cpy((txtMenuMusicVolumeTest->pstring)[0], (translation->pstring)[29]);
		str_cpy((txtMenuSpeechVolumeTest->pstring)[0], (translation->pstring)[30]);
		str_cpy((txtMenuEffectsVolumeTest->pstring)[0], (translation->pstring)[31]);
	}
	str_cpy((txtMenuOptionsApply->pstring)[0], (translation->pstring)[32]);
	str_cpy((txtMenuCredits->pstring)[0], (translation->pstring)[33]);
	str_cpy((txtMenuExitGame->pstring)[0], (translation->pstring)[34]);
	str_cpy((txtMenuReturnToWin->pstring)[0], (translation->pstring)[35]);
	str_cpy((txtMenuBack->pstring)[0], (translation->pstring)[36]);

	set(txtMenuNewGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuContinueGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptions, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptionsGame, CENTER_X | CENTER_Y | OUTLINE);
	{
		set(txtMenuDifficulty, OUTLINE);
		{
			set(txtOptionsGameEasy, OUTLINE);
			set(txtOptionsGameMedium, OUTLINE);
			set(txtOptionsGameHard, OUTLINE);
		}
		set(txtMenuViolence, OUTLINE);
		set(txtMenuShowDialogs, OUTLINE);
		set(txtMenuShowHints, OUTLINE);
	}
	set(txtMenuOptionsGraphics, CENTER_X | CENTER_Y | OUTLINE);
	{
		set(txtMenuAntialias, OUTLINE);
		{
			set(txtOptionsGraphicsAAOff, OUTLINE);
			set(txtOptionsGraphicsAA1x, OUTLINE);
			set(txtOptionsGraphicsAA4x, OUTLINE);
			set(txtOptionsGraphicsAA9x, OUTLINE);
		}
		set(txtMenuBrightness, OUTLINE);
		set(txtMenuShader, OUTLINE);
		set(txtMenuShadows, OUTLINE);
		set(txtMenuDetails, OUTLINE);
		{
			set(txtOptionsGraphicsDetailsLow, OUTLINE);
			set(txtOptionsGraphicsDetailsMedium, OUTLINE);
			set(txtOptionsGraphicsDetailsHigh, OUTLINE);
		}
		set(txtMenuFullscreen, OUTLINE);
	}
	set(txtMenuOptionsAudio, CENTER_X | CENTER_Y | OUTLINE);
	{
		set(txtMenuMusicVolume, OUTLINE);
		set(txtMenuSpeechVolume, OUTLINE);
		set(txtMenuEffectsVolume, OUTLINE);

		set(txtMenuMusicVolumeTest, OUTLINE | CENTER_X | CENTER_Y);
		set(txtMenuSpeechVolumeTest, OUTLINE | CENTER_X | CENTER_Y);
		set(txtMenuEffectsVolumeTest, OUTLINE | CENTER_X | CENTER_Y);
	}
	set(txtMenuOptionsApply, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuCredits, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuExitGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuReturnToWin, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuBack, CENTER_X | CENTER_Y | OUTLINE);

	menu_message_box_init();
	menu_center();
}

void menu_show(int _menu)
{
	menu_hide();

	switch (_menu)
	{
		case MENU_START:
		{
			nCurrentMenu = MENU_START;
			set(panStartMenu, SHOW);
			set(txtMenuNewGame, SHOW);
			set(txtMenuOptions, SHOW);
			set(txtMenuCredits, SHOW);
			set(txtMenuReturnToWin, SHOW);
			break;
		}

		case MENU_IN_GAME:
		{
			nCurrentMenu = MENU_IN_GAME;
			set(panInGameMenu, SHOW);
			set(txtMenuNewGame, SHOW);
			set(txtMenuContinueGame, SHOW);
			set(txtMenuOptions, SHOW);
			set(txtMenuExitGame, SHOW);
			break;
		}

		case MENU_OPTIONS_GAME:
		{
			nCurrentMenu = MENU_OPTIONS_GAME;
			set(panOptionsMenu, SHOW);
			set(panOptionsGame, SHOW);
			set(txtMenuOptionsGame, SHOW);
			set(txtMenuOptionsGraphics, SHOW);
			set(txtMenuOptionsAudio, SHOW);
			set(txtMenuOptionsApply, SHOW);
			set(txtMenuBack, SHOW);
			set(txtMenuDifficulty, SHOW);
			set(txtMenuViolence, SHOW);
			set(txtMenuShowDialogs, SHOW);
			set(txtMenuShowHints, SHOW);

			set(panOptionsGameDifficulty, SHOW);
			set(txtOptionsGameEasy, SHOW);
			set(txtOptionsGameMedium, SHOW);
			set(txtOptionsGameHard, SHOW);

			// Restore values
			button_state(panOptionsMenu, 1, 1);
			vNewGameDifficulty	= vGameDifficulty;
			vNewGameBlood			= vGameBlood;
			vNewGameShowDialogs	= vGameShowDialogs;
			vNewGameShowHints		= vGameShowHints;
			if (vNewGameBlood) {
				button_state(panOptionsGame, 3, 1);
			} else {
				button_state(panOptionsGame, 3, 0);
			}

			if (vNewGameShowDialogs) {
				button_state(panOptionsGame, 4, 1);
			} else {
				button_state(panOptionsGame, 4, 0);
			}

			if (vNewGameShowHints) {
				button_state(panOptionsGame, 5, 1);
			} else {
				button_state(panOptionsGame, 5, 0);
			}

			switch(vNewGameDifficulty)
			{
				case 1: button_state(panOptionsGameDifficulty, 1, 1); break;
				case 2: button_state(panOptionsGameDifficulty, 2, 1); break;
				case 3: button_state(panOptionsGameDifficulty, 3, 1); break;
			}

			break;
		}

		case MENU_OPTIONS_GRAPHICS:
		{
			nCurrentMenu = MENU_OPTIONS_GRAPHICS;
			set(panOptionsMenu, SHOW);
			set(panOptionsGraphics, SHOW);
			set(txtMenuOptionsGame, SHOW);
			set(txtMenuOptionsGraphics, SHOW);
			set(txtMenuOptionsAudio, SHOW);
			set(txtMenuOptionsApply, SHOW);
			set(txtMenuBack, SHOW);
			set(txtMenuAntialias, SHOW);
			set(txtMenuBrightness, SHOW);
			set(txtMenuShader, SHOW);
			set(txtMenuShadows, SHOW);
			set(txtMenuDetails, SHOW);
			set(panOptionsGraphicsDetails, SHOW);
			set(panOptionsGraphicsAntiAliasing, SHOW);
			set(txtMenuFullscreen, SHOW);

			// Restore values
			button_state(panOptionsMenu, 2, 1);
			{
				set(txtOptionsGraphicsAAOff, SHOW);
				set(txtOptionsGraphicsAA1x, SHOW);
				set(txtOptionsGraphicsAA4x, SHOW);
				set(txtOptionsGraphicsAA9x, SHOW);
				vNewGameAntiAlias = d3d_antialias;
				switch(vNewGameAntiAlias) {
					case 0: button_state(panOptionsGraphicsAntiAliasing, 1, 1); break;
					case 1: button_state(panOptionsGraphicsAntiAliasing, 2, 1); break;
					case 4: button_state(panOptionsGraphicsAntiAliasing, 3, 1); break;
					case 9: button_state(panOptionsGraphicsAntiAliasing, 4, 1); break;
				}
			}

			{
				set(txtOptionsGraphicsDetailsLow, SHOW);
				set(txtOptionsGraphicsDetailsMedium, SHOW);
				set(txtOptionsGraphicsDetailsHigh, SHOW);

				switch(vNewGameDetails) {
					case 0: button_state(panOptionsGraphicsDetails, 1, 1); break;
					case 1: button_state(panOptionsGraphicsDetails, 1, 1); break;
					case 2: button_state(panOptionsGraphicsDetails, 1, 1); break;
				}
			}

			vNewGameBrightness = video_gamma;

			vNewGameShaders = d3d_shaderversion;
			if (vNewGameShaders > 0) {
				button_state(panOptionsGraphics, 4, 1);
			} else {
				button_state(panOptionsGraphics, 4, 0);
			}

			vNewGameShadows = shadow_stencil;
			if (vNewGameShadows > -1) {
				button_state(panOptionsGraphics, 5, 1);
			} else {
				button_state(panOptionsGraphics, 5, 0);
			}

			vNewGameFullscreen = video_screen;
			if (vNewGameFullscreen == 1) {
				button_state(panOptionsGraphics, 6, 1);
			} else {
				button_state(panOptionsGraphics, 6, 0);
			}

			break;
		}

		case MENU_OPTIONS_AUDIO:
		{
			nCurrentMenu = MENU_OPTIONS_AUDIO;
			set(panOptionsMenu, SHOW);
			set(panOptionsAudio, SHOW);
			set(txtMenuOptionsGame, SHOW);
			set(txtMenuOptionsGraphics, SHOW);
			set(txtMenuOptionsAudio, SHOW);
			set(txtMenuOptionsApply, SHOW);
			set(txtMenuBack, SHOW);
			set(txtMenuMusicVolume, SHOW);
			set(txtMenuSpeechVolume, SHOW);
			set(txtMenuEffectsVolume, SHOW);
			set(txtMenuMusicVolumeTest, SHOW);
			set(txtMenuSpeechVolumeTest, SHOW);
			set(txtMenuEffectsVolumeTest, SHOW);

			// Restore values
			button_state(panOptionsMenu, 3, 1);
			vNewGameMusicVolume		= vGameMusicVolume;
			vNewGameSpeechVolume		= vGameSpeechVolume;
			vNewGameEffectsVolume	= vGameEffectsVolume;
		}
		break;
	}
	menu_align(_menu);
}

void menu_hide()
{
	if (panStartMenu != NULL) reset(panStartMenu, SHOW);
	if (panInGameMenu != NULL) reset(panInGameMenu, SHOW);
	if (panOptionsMenu != NULL) reset(panOptionsMenu, SHOW);
	if (panOptionsGame != NULL) reset(panOptionsGame, SHOW);
	if (panOptionsGraphics != NULL) reset(panOptionsGraphics, SHOW);
	if (panOptionsAudio != NULL) reset(panOptionsAudio, SHOW);
	if (txtMenuOptionsGame != NULL) reset(txtMenuOptionsGame, SHOW);
	if (txtMenuDifficulty != NULL) reset(txtMenuDifficulty, SHOW);
	if (txtMenuViolence != NULL) reset(txtMenuViolence, SHOW);
	if (txtMenuShowDialogs != NULL) reset(txtMenuShowDialogs, SHOW);
	if (txtMenuShowHints != NULL) reset(txtMenuShowHints, SHOW);
	if (txtMenuOptionsGraphics != NULL) reset(txtMenuOptionsGraphics, SHOW);
	if (txtMenuDetails != NULL) reset(txtMenuDetails, SHOW);
	if (txtMenuShadows != NULL) reset(txtMenuShadows, SHOW);
	if (txtMenuShader != NULL) reset(txtMenuShader, SHOW);
	if (txtMenuBrightness != NULL) reset(txtMenuBrightness, SHOW);
	if (txtMenuAntialias != NULL) reset(txtMenuAntialias, SHOW);
	if (txtMenuFullscreen != NULL) reset(txtMenuFullscreen, SHOW);
	if (txtMenuOptionsAudio != NULL) reset(txtMenuOptionsAudio, SHOW);
	if (txtMenuNewGame != NULL) reset(txtMenuNewGame, SHOW);
	if (txtMenuContinueGame != NULL) reset(txtMenuContinueGame, SHOW);
	if (txtMenuOptions != NULL) reset(txtMenuOptions, SHOW);
	if (txtMenuOptionsApply != NULL) reset(txtMenuOptionsApply, SHOW);
	if (txtMenuCredits != NULL) reset(txtMenuCredits, SHOW);
	if (txtMenuExitGame != NULL) reset(txtMenuExitGame, SHOW);
	if (txtMenuReturnToWin != NULL) reset(txtMenuReturnToWin, SHOW);
	if (txtMenuBack != NULL) reset(txtMenuBack, SHOW);
	if (panOptionsGraphicsDetails != NULL) reset(panOptionsGraphicsDetails, SHOW);
	if (panOptionsGraphicsAntiAliasing != NULL) reset(panOptionsGraphicsAntiAliasing, SHOW);
	if (txtOptionsGraphicsAAOff != NULL) reset(txtOptionsGraphicsAAOff, SHOW);
	if (txtOptionsGraphicsAA1x != NULL) reset(txtOptionsGraphicsAA1x, SHOW);
	if (txtOptionsGraphicsAA4x != NULL) reset(txtOptionsGraphicsAA4x, SHOW);
	if (txtOptionsGraphicsAA9x != NULL) reset(txtOptionsGraphicsAA9x, SHOW);
	if (txtOptionsGraphicsDetailsLow != NULL) reset(txtOptionsGraphicsDetailsLow, SHOW);
	if (txtOptionsGraphicsDetailsMedium != NULL) reset(txtOptionsGraphicsDetailsMedium, SHOW);
	if (txtOptionsGraphicsDetailsHigh != NULL) reset(txtOptionsGraphicsDetailsHigh, SHOW);

	if (txtMenuMusicVolume != NULL) reset(txtMenuMusicVolume, SHOW);
	if (txtMenuSpeechVolume != NULL) reset(txtMenuSpeechVolume, SHOW);
	if (txtMenuEffectsVolume != NULL) reset(txtMenuEffectsVolume, SHOW);
	if (txtMenuMusicVolumeTest != NULL) reset(txtMenuMusicVolumeTest, SHOW);
	if (txtMenuSpeechVolumeTest != NULL) reset(txtMenuSpeechVolumeTest, SHOW);
	if (txtMenuEffectsVolumeTest != NULL) reset(txtMenuEffectsVolumeTest, SHOW);

	if (panOptionsGameDifficulty != NULL) reset(panOptionsGameDifficulty, SHOW);
	if (txtOptionsGameEasy != NULL) reset(txtOptionsGameEasy, SHOW);
	if (txtOptionsGameMedium != NULL) reset(txtOptionsGameMedium, SHOW);
	if (txtOptionsGameHard != NULL) reset(txtOptionsGameHard, SHOW);
}

void menu_align(int _menu)
{
	if (panStartMenu == NULL) return;

	switch(_menu)
	{
		case MENU_START:
			txtMenuNewGame->pos_x = panStartMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuNewGame->pos_y = panStartMenu->pos_y + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptions->pos_x = panStartMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptions->pos_y = panStartMenu->pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_Y / 2;

			txtMenuCredits->pos_x = panStartMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuCredits->pos_y = panStartMenu->pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_Y / 2;

			txtMenuReturnToWin->pos_x = panStartMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuReturnToWin->pos_y = panStartMenu->pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_Y / 2;
		break;
		case MENU_IN_GAME:

			txtMenuNewGame->pos_x = panInGameMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuNewGame->pos_y = panInGameMenu->pos_y + MENU_BUTTON_SIZE_Y / 2;

			txtMenuContinueGame->pos_x = panInGameMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuContinueGame->pos_y = panInGameMenu->pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptions->pos_x = panInGameMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptions->pos_y = panInGameMenu->pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_Y / 2;

			txtMenuExitGame->pos_x = panInGameMenu->pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuExitGame->pos_y = panInGameMenu->pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_Y / 2;
		break;

		case MENU_OPTIONS_GAME:
			panOptionsGame->pos_x = panOptionsMenu->pos_x;
			panOptionsGame->pos_y = panOptionsMenu->pos_y + panOptionsMenu->size_y + MENU_BUTTON_GAP;

			txtMenuOptionsGame->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGame->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsGraphics->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGraphics->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsAudio->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsAudio->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsApply->pos_x = panOptionsGame->pos_x + bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X / 2 - MENU_BUTTON_GAP;
			txtMenuOptionsApply->pos_y = panOptionsGame->pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

			txtMenuBack->pos_x = panOptionsGame->pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack->pos_y = panOptionsGame->pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

			txtMenuDifficulty->pos_x = panOptionsGame->pos_x + 10;
			txtMenuDifficulty->pos_y = panOptionsGame->pos_y + MENU_BUTTON_SIZE_Y * 1.1;

			panOptionsGameDifficulty->pos_x = panOptionsGame->pos_x + MENU_OPTIONS_CAPTION_POS_X;
			panOptionsGameDifficulty->pos_y = panOptionsGame->pos_y + MENU_BUTTON_SIZE_Y * 1;

			txtOptionsGameEasy->pos_x = panOptionsGameDifficulty->pos_x + 20;
			txtOptionsGameEasy->pos_y = panOptionsGameDifficulty->pos_y * 1.01;

			txtOptionsGameMedium->pos_x = panOptionsGameDifficulty->pos_x + 100;
			txtOptionsGameMedium->pos_y = panOptionsGameDifficulty->pos_y * 1.01;

			txtOptionsGameHard->pos_x = panOptionsGameDifficulty->pos_x + 180;
			txtOptionsGameHard->pos_y = panOptionsGameDifficulty->pos_y * 1.01;

			txtMenuViolence->pos_x = panOptionsGame->pos_x + 10;
			txtMenuViolence->pos_y =panOptionsGame->pos_y + MENU_BUTTON_SIZE_Y * 2.1;

			txtMenuShowDialogs->pos_x = panOptionsGame->pos_x + 10;
			txtMenuShowDialogs->pos_y = panOptionsGame->pos_y + MENU_BUTTON_SIZE_Y * 3.1;

			txtMenuShowHints->pos_x = panOptionsGame->pos_x + 10;
			txtMenuShowHints->pos_y = panOptionsGame->pos_y + MENU_BUTTON_SIZE_Y * 4.1;
		break;
		case MENU_OPTIONS_GRAPHICS:
			panOptionsGraphics->pos_x = panOptionsMenu->pos_x;
			panOptionsGraphics->pos_y = panOptionsMenu->pos_y + panOptionsMenu->size_y + MENU_BUTTON_GAP;

			txtMenuOptionsGame->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGame->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsGraphics->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGraphics->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsAudio->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsAudio->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsApply->pos_x = panOptionsGraphics->pos_x + bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X / 2 - MENU_BUTTON_GAP;
			txtMenuOptionsApply->pos_y = panOptionsGraphics->pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

			txtMenuBack->pos_x = panOptionsGraphics->pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack->pos_y = panOptionsGraphics->pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

			txtMenuAntialias->pos_x = panOptionsGraphics->pos_x + 10;
			txtMenuAntialias->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 2.1;

			txtMenuBrightness->pos_x = panOptionsGraphics->pos_x + 10;
			txtMenuBrightness->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 3.1;

			txtMenuShader->pos_x = panOptionsGraphics->pos_x + 10;
			txtMenuShader->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 4.1;

			txtMenuShadows->pos_x = panOptionsGraphics->pos_x + 10;
			txtMenuShadows->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 5.1;

			txtMenuDetails->pos_x = panOptionsGraphics->pos_x + 10;
			txtMenuDetails->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 6.1;

			panOptionsGraphicsDetails->pos_x = panOptionsGraphics->pos_x + MENU_OPTIONS_CAPTION_POS_X;
			panOptionsGraphicsDetails->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 6;
			{
				txtOptionsGraphicsDetailsLow->pos_x = panOptionsGraphicsDetails->pos_x + 20;
				txtOptionsGraphicsDetailsLow->pos_y = panOptionsGraphicsDetails->pos_y;

				txtOptionsGraphicsDetailsMedium->pos_x = panOptionsGraphicsDetails->pos_x + 100;
				txtOptionsGraphicsDetailsMedium->pos_y = panOptionsGraphicsDetails->pos_y;

				txtOptionsGraphicsDetailsHigh->pos_x = panOptionsGraphicsDetails->pos_x + 180;
				txtOptionsGraphicsDetailsHigh->pos_y = panOptionsGraphicsDetails->pos_y;
			}

			panOptionsGraphicsAntiAliasing->pos_x = panOptionsGraphics->pos_x + MENU_OPTIONS_CAPTION_POS_X;
			panOptionsGraphicsAntiAliasing->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 2;
			{
				txtOptionsGraphicsAAOff->pos_x = panOptionsGraphicsAntiAliasing->pos_x + 20;
				txtOptionsGraphicsAAOff->pos_y = panOptionsGraphicsAntiAliasing->pos_y;

				txtOptionsGraphicsAA1x->pos_x = panOptionsGraphicsAntiAliasing->pos_x + 70;
				txtOptionsGraphicsAA1x->pos_y = panOptionsGraphicsAntiAliasing->pos_y;

				txtOptionsGraphicsAA4x->pos_x = panOptionsGraphicsAntiAliasing->pos_x + 120;
				txtOptionsGraphicsAA4x->pos_y = panOptionsGraphicsAntiAliasing->pos_y;

				txtOptionsGraphicsAA9x->pos_x = panOptionsGraphicsAntiAliasing->pos_x + 170;
				txtOptionsGraphicsAA9x->pos_y = panOptionsGraphicsAntiAliasing->pos_y;
			}

			txtMenuFullscreen->pos_x = panOptionsGraphics->pos_x + 10;
			txtMenuFullscreen->pos_y = panOptionsGraphics->pos_y + MENU_BUTTON_SIZE_Y * 7.1;
		break;
		case MENU_OPTIONS_AUDIO:
			panOptionsAudio->pos_x = panOptionsMenu->pos_x;
			panOptionsAudio->pos_y = panOptionsMenu->pos_y + panOptionsMenu->size_y + MENU_BUTTON_GAP;

			txtMenuOptionsGame->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGame->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsGraphics->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGraphics->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsAudio->pos_x = panOptionsMenu->pos_x + MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsAudio->pos_y = panOptionsMenu->pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;

			txtMenuOptionsApply->pos_x = panOptionsAudio->pos_x + bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X / 2 - MENU_BUTTON_GAP;
			txtMenuOptionsApply->pos_y = panOptionsAudio->pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

			txtMenuBack->pos_x = panOptionsAudio->pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack->pos_y = panOptionsAudio->pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

			txtMenuMusicVolume->pos_x = panOptionsAudio->pos_x + 10;
			txtMenuMusicVolume->pos_y = panOptionsAudio->pos_y + MENU_BUTTON_SIZE_Y * 1.1;

			txtMenuSpeechVolume->pos_x = panOptionsAudio->pos_x + 10;
			txtMenuSpeechVolume->pos_y = panOptionsAudio->pos_y + MENU_BUTTON_SIZE_Y * 2.1;

			txtMenuEffectsVolume->pos_x = panOptionsAudio->pos_x + 10;
			txtMenuEffectsVolume->pos_y = panOptionsAudio->pos_y + MENU_BUTTON_SIZE_Y * 3.1;

			txtMenuMusicVolumeTest->pos_x = panOptionsAudio->pos_x + MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6 + MENU_CHECKBOX_SIZE_X / 2;
			txtMenuMusicVolumeTest->pos_y = panOptionsAudio->pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_CHECKBOX_SIZE_Y / 2;

			txtMenuSpeechVolumeTest->pos_x = panOptionsAudio->pos_x + MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6 + MENU_CHECKBOX_SIZE_X / 2;
			txtMenuSpeechVolumeTest->pos_y = panOptionsAudio->pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_CHECKBOX_SIZE_Y / 2;

			txtMenuEffectsVolumeTest->pos_x = panOptionsAudio->pos_x + MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6 + MENU_CHECKBOX_SIZE_X / 2;
			txtMenuEffectsVolumeTest->pos_y = panOptionsAudio->pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_CHECKBOX_SIZE_Y / 2;
		break;
	}
}

void menu_center()
{
	if (panStartMenu != NULL)
	{
		gui_panel_set_pos(panStartMenu, screen_size.x / 2 - panStartMenu->size_x / 2,
			                            screen_size.y / 2 - panStartMenu->size_y / 2);

		gui_panel_set_pos(panInGameMenu, screen_size.x / 2 - panStartMenu->size_x / 2,
		                                 screen_size.y / 2 - panStartMenu->size_y / 2);

		gui_panel_set_pos(panOptionsMenu, screen_size.x / 2 - panOptionsMenu->size_x / 2,
		                                  30);

		menu_align(nCurrentMenu);
	}
}

void menu_set_pos(int _x, int _y)
{
	if (panStartMenu != NULL)
	{
		gui_panel_set_pos(panStartMenu, _x, _y);
		gui_panel_set_pos(panInGameMenu, _x, _x);
		gui_panel_set_pos(panOptionsMenu, _x, _y);

		menu_align(nCurrentMenu);
	}
}

__static void menu_message_box_init()
{
	// Message box background
	panMessageBoxBg = pan_create(NULL, LAYER_GUI_1 + 2);
	panMessageBoxBg->flags |= (LIGHT | TRANSLUCENT);
	panMessageBoxBg->alpha = 70;
	vec_set(&panMessageBoxBg->blue, vector(30,30,30));

	// Message box text
	txtMessageBoxCaption = txt_create(1, LAYER_GUI_1 + 4);
	txtMessageBoxCaption->flags |= (OUTLINE);

	// Buttons
	txtMessageBoxBtn1 = txt_create(1, LAYER_GUI_1 + 5);
	txtMessageBoxBtn1->flags |= (OUTLINE);
	txtMessageBoxBtn2 = txt_create(1, LAYER_GUI_1 + 5);
	txtMessageBoxBtn2->flags |= (OUTLINE);
}

void menu_show_message(STRING* _msg, STRING* _button)
{
	if (panMessageBoxBg == NULL)
		return;

	if (panMessageBoxBg->flags & SHOW)
		return;

	nMessageBoxResult = 0;

	panMessageBoxBg->size_x = screen_size.x;
	panMessageBoxBg->size_y = screen_size.y;

	// Needs to be recreated due to button count
	if (panMessageBox != NULL) ptr_remove(panMessageBox);
	panMessageBox = pan_create(NULL, LAYER_GUI_1 + 3);
	set(panMessageBox, LIGHT);
	vec_set(&panMessageBox->blue, vector(10,10,10));
	panMessageBox->size_x = 400;
	panMessageBox->size_y = 100;

	panMessageBox->pos_x = screen_size.x / 2 - panMessageBox->size_x / 2;
	panMessageBox->pos_y = screen_size.y / 2 - panMessageBox->size_y / 2;

	if (_button != NULL) {
		pan_setbutton(panMessageBox, 0, 1, 200 - MENU_BUTTON_SIZE_X / 2, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
		pan_setcolor(panMessageBox, 3, 1, __gui_color_border__);
		str_cpy((txtMessageBoxBtn1->pstring)[0], _button);
		txtMessageBoxBtn1->pos_x = panMessageBox->pos_x + 200;
		txtMessageBoxBtn1->pos_y = panMessageBox->pos_y + 100 - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
		set(txtMessageBoxBtn1, SHOW | CENTER_X | CENTER_Y);
	}

	str_cpy((txtMessageBoxCaption->pstring)[0], _msg);
	txtMessageBoxCaption->pos_x = panMessageBox->pos_x + 200;
	txtMessageBoxCaption->pos_y = panMessageBox->pos_y + 30;

	// Show them all
	set(panMessageBoxBg, SHOW);
	set(panMessageBox, SHOW);
	set(txtMessageBoxCaption, SHOW | CENTER_X | CENTER_Y);
}

int menu_show_choice_message(STRING* _msg, STRING* _button1, STRING* _button2)
{
	if (panMessageBoxBg == NULL)
		return;

	if (panMessageBoxBg->flags & SHOW)
		return;

	nMessageBoxResult = 0;

	panMessageBoxBg->size_x = screen_size.x;
	panMessageBoxBg->size_y = screen_size.y;

	// Needs to be recreated due to button count
	if (panMessageBox != NULL) ptr_remove(panMessageBox);
	panMessageBox = pan_create(NULL, LAYER_GUI_1 + 3);
	set(panMessageBox, LIGHT);
	vec_set(&panMessageBox->blue, vector(10,10,10));
	panMessageBox->size_x = 400;
	panMessageBox->size_y = 100;
	pan_setbutton(panMessageBox, 0, 1, MENU_BUTTON_GAP, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
	pan_setbutton(panMessageBox, 0, 1, 400 - MENU_BUTTON_GAP - MENU_BUTTON_SIZE_X, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
	pan_setcolor(panMessageBox, 3, 1, __gui_color_border__);
	pan_setcolor(panMessageBox, 3, 2, __gui_color_border__);
	panMessageBox->pos_x = screen_size.x / 2 - panMessageBox->size_x / 2;
	panMessageBox->pos_y = screen_size.y / 2 - panMessageBox->size_y / 2;

	str_cpy((txtMessageBoxBtn1->pstring)[0], _button1);
	str_cpy((txtMessageBoxBtn2->pstring)[0], _button2);

	str_cpy((txtMessageBoxCaption->pstring)[0], _msg);
	txtMessageBoxCaption->pos_x = panMessageBox->pos_x + 200;
	txtMessageBoxCaption->pos_y = panMessageBox->pos_y + 30;

	str_cpy((txtMessageBoxBtn1->pstring)[0], _button1);
	txtMessageBoxBtn1->pos_x = panMessageBox->pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
	txtMessageBoxBtn1->pos_y = panMessageBox->pos_y + 100 - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

	str_cpy((txtMessageBoxBtn2->pstring)[0], _button2);
	txtMessageBoxBtn2->pos_x = panMessageBox->pos_x + 400 - MENU_BUTTON_GAP - MENU_BUTTON_SIZE_X / 2;
	txtMessageBoxBtn2->pos_y = panMessageBox->pos_y + 100 - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;

	// Show them all
	set(panMessageBoxBg, SHOW);
	set(panMessageBox, SHOW);
	set(txtMessageBoxCaption, SHOW | CENTER_X | CENTER_Y);
	set(txtMessageBoxBtn1, SHOW | CENTER_X | CENTER_Y);
	set(txtMessageBoxBtn2, SHOW | CENTER_X | CENTER_Y);
}

__static void menu_options_choose(var _button_number, PANEL* _panel)
{
	switch(_button_number)
	{
		case 1: menu_show(MENU_OPTIONS_GAME); break;
		case 2: menu_show(MENU_OPTIONS_GRAPHICS); break;
		case 3: menu_show(MENU_OPTIONS_AUDIO); break;
	}
}

__static void menu_back_click(var _button_number, PANEL* _panel)
{
	if (!nInGame)
		menu_show(MENU_START);
	else
		menu_show(MENU_IN_GAME);
}

__static void menu_start_click(var _button_number, PANEL* _panel)
{

	switch(_button_number) {
		case 1:
			printf("new game");
		break;
		case 2:
			menu_show(MENU_OPTIONS_GAME);
		break;
		case 3:
			printf("credits");
		break;
		case 4:
			sys_exit(NULL);
		break;
	}
}

__static void menu_ingame_click(var _button_number, PANEL* _panel) {
	switch(_button_number) {
		case 1:
			printf("new game");
		break;
		case 2:
			printf("continue");
		break;
		case 3:
			menu_show(MENU_OPTIONS_GAME);
		break;
		case 4:
			nInGame = 0;
			menu_show(MENU_START);
		break;
	}
}

__static void menu_apply_click(var _button_number, PANEL* _panel)
{
	if (_panel == panOptionsGame)
	{
		// These if comparision serve as method to check, if the
		// original value has changed. Can be deleted if wanted.
		if (vNewGameDifficulty != vGameDifficulty) {
			vGameDifficulty = vNewGameDifficulty;
		}

		vNewGameBlood = button_state(panOptionsGame, 1, -1);
		if (vNewGameBlood != vGameBlood) {
			vGameBlood = vNewGameBlood;
		}

		vNewGameShowDialogs = button_state(panOptionsGame, 2, -1);
		if (vNewGameShowDialogs != vGameShowDialogs) {
			vGameShowDialogs = vNewGameShowDialogs;
		}

		vNewGameShowHints = button_state(panOptionsGame, 3, -1);
		if (vNewGameShowHints != vGameShowHints) {
			vGameShowHints = vNewGameShowHints;
		}
	}

	if (_panel == panOptionsGraphics) {

		// Resolution and full screen
		if (button_state(panOptionsGraphics, 6, -1) == 1) {
			vNewGameFullscreen = 1;
		} else {
			vNewGameFullscreen = 2;
		}

		// Anti aliasing
		if (vNewGameAntiAlias != d3d_antialias) {
			d3d_antialias = vNewGameAntiAlias;
			// ToDo: Requires game restart
		}

		// Brightness
		if (vNewGameBrightness != video_gamma) {
			video_gamma = vNewGameBrightness;
		}

		vNewGameShaders = button_state(panOptionsGraphics, 4, -1);
		if (vNewGameShaders != d3d_shaderversion) {
			if (vNewGameShaders == 0) {
				d3d_shaderversion = 0;
			} else {
				d3d_shaderversion = vInitialShaderVersion;
			}
		}

		vNewGameShadows = button_state(panOptionsGraphics, 5, -1);
		if (vNewGameShadows == 1) {
			vNewGameShadows = 2;
		} else {
			vNewGameShadows = -1;
		}
		if (vNewGameShadows != shadow_stencil) {
			shadow_stencil = vNewGameShadows;
		}

		if (vNewGameDetails != vGameDetails) {
			vGameDetails = vNewGameDetails;
		}
	}

	if (_panel == panOptionsAudio) {

		if (vNewGameMusicVolume != vGameMusicVolume) {
			vGameMusicVolume = vNewGameMusicVolume;
		}

		if (vNewGameSpeechVolume != vGameSpeechVolume) {
			vGameSpeechVolume = vNewGameSpeechVolume;
		}

		if (vNewGameEffectsVolume != vGameEffectsVolume) {
			vGameEffectsVolume = vNewGameEffectsVolume;
		}
	}
}

__static void menu_difficulty_click(var _button_number, PANEL* _panel) {
	vNewGameDifficulty = _button_number;
}

void menu_anti_aliasing_click(var _button_number, PANEL* _panel) {
	switch(_button_number) {
		case 1: vNewGameAntiAlias = 0; break;
		case 2: vNewGameAntiAlias = 1; break;
		case 3: vNewGameAntiAlias = 4; break;
		case 4: vNewGameAntiAlias = 9; break;
	}
}

__static void menu_details_click(var _button_number, PANEL* _panel) {
	vNewGameDetails = _button_number;
}

__static void menu_message_box_click(var _button_number, PANEL* _panel) {
	nMessageBoxResult = (long)_button_number;
	menu_hide_message();
}

__static void menu_hide_message()
{
	if (panMessageBoxBg != NULL) reset(panMessageBoxBg, SHOW);
	if (panMessageBox != NULL) reset(panMessageBox, SHOW);
	if (txtMessageBoxCaption != NULL) reset(txtMessageBoxCaption, SHOW | CENTER_X | CENTER_Y);
	if (txtMessageBoxBtn1 != NULL) reset(txtMessageBoxBtn1, SHOW | CENTER_X | CENTER_Y);
	if (txtMessageBoxBtn2 != NULL) reset(txtMessageBoxBtn2, SHOW | CENTER_X | CENTER_Y);
}

__static void menu_audio_test_box_click(var _button_number, PANEL* _panel)
{
	switch(_button_number)
	{
		case 3:
		{
			if(sndMusicTest)
				snd_play(sndMusicTest, vNewGameMusicVolume, 0);
			break;
		}

		case 4:
		{
			if(sndSpeechTest)
				snd_play(sndSpeechTest, vNewGameSpeechVolume, 0);
			break;
		}
		case 5:
		{
			if(sndEffectsTest)
				snd_play(sndEffectsTest, vNewGameEffectsVolume, 0);
			break;
		}
	}
}

int menu_message_visible()
{
	if (panMessageBoxBg != NULL)
		return (int) ifelse(panMessageBoxBg->flags & SHOW, 1, 0);
	return 0;
}

int menu_message_result()
{
	return nMessageBoxResult;
}

/*
 * void menu_change_theme(int section, float r, float g, float b)
 *
 * Alters the current color of one part of the menu.
 * Valid constants (which is passed to the first parameter) are:
 * COLOR_AMBIENT  -- Will alter the "normal" (background) color of the menu.
 * COLOR_CLICKED  -- Will alter the color of the clicked component.
 * COLOR_BORDER   -- Will alter the color of the component's border.
 */
void menu_change_theme(int section, float r, float g, float b)
{
	switch(section)
	{
		case    COLOR_AMBIENT:
		{
			vec_set(__gui_color_theme__, vector(b, g, r));
			break;
		}

		case    COLOR_CLICKED:
		{
			vec_set(__gui_color_click__, vector(b, g, r));
			break;
		}

		case    COLOR_BORDER:
		{
			vec_set(__gui_color_border__, vector(b, g, r));
		}
	}
}

/*
 * void menu_change_sound_test(int section, const char *file)
 *
 * Alters the current sound effect used for testing the volume in the main menu.
 * Possible constants (which is passed to the first parameter) are:
 * SOUND_TEST_MUSIC     -- Will alter the sound effect of the "music" slider.
 * SOUND_TEST_SPEECH    -- Will alter the sound effect of the "speech" slider.
 * SOUND_TEST_EFFECTS   -- Will alter the sound effect of the "effects" slider.
 */
void menu_change_sound_test(int section, const char *file)
{
	if(!file)
		return;

	switch(section)
	{
		case    SOUND_TEST_MUSIC:
		{
			if(sndMusicTest)
				snd_remove(sndMusicTest);
			sndMusicTest = snd_create(file);

			break;
		}

		case    SOUND_TEST_SPEECH:
		{
			if(sndSpeechTest)
				snd_remove(sndSpeechTest);
			sndSpeechTest = snd_create(file);

			break;
		}

		case    SOUND_TEST_EFFECTS:
		{
			if(sndEffectsTest)
				snd_remove(sndEffectsTest);
			sndEffectsTest = snd_create(file);
		}
	}
}
