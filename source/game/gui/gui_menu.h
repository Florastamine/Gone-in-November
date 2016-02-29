/* gui_menu.h */
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
#ifndef    __GUI_MENU_H__
#define    __GUI_MENU_H__

#define    __static
#define    __In
#define    __Out
#define    __namespace(namespace) {}

#define    MENU_BUTTON_SIZE_X         175			// Size for auto generated button images
#define    MENU_BUTTON_SIZE_Y         25

#define    MENU_SAVE_SLOT_SIZE_X      150		// For optimal alignment set to the same as MENU_BUTTON_SIZE_X
#define    MENU_SAVE_SLOT_SIZE_Y      100

#define    MENU_SLIDER_SIZE_X         250			// Size of the slider
#define    MENU_SLIDER_SIZE_Y         5

#define    MENU_KNOB_SIZE_X           11.5				// Size of the knob for the sliders
#define    MENU_KNOB_SIZE_Y           11.5

#define    MENU_CHECKBOX_SIZE_X       15			// Size of Checkboxes
#define    MENU_CHECKBOX_SIZE_Y       15

#define    MENU_COMBOBOX_SIZE_X       200			// Size of the combobox
#define    MENU_COMBOBOX_SIZE_Y       15

#define    MENU_BUTTON_GAP            2.5

#define    MENU_OPTIONS_SIZE_X        -1			// Size of the entire options menu. -1 = Same size as Options choice bar
#define    MENU_OPTIONS_SIZE_Y        380

#define    MENU_OPTIONS_CAPTION_POS_X 130	// x-distance between option caption and control

#define    MENU_MAIN_MAX_ITEMS        4
#define    MENU_IN_GAME_MAX_ITEMS     4

/**
 * Defines for the current menu that is visible
 */
#define    MENU_NONE             0
#define    MENU_START            1
#define    MENU_IN_GAME          2
#define    MENU_OPTIONS_GAME     3
#define    MENU_OPTIONS_GRAPHICS 4
#define    MENU_OPTIONS_AUDIO    5
#define    MENU_CREDITS          6

#define    COLOR_AMBIENT         1
#define    COLOR_CLICKED         2
#define    COLOR_BORDER          3

#define    SOUND_TEST_MUSIC      1
#define    SOUND_TEST_SPEECH     2
#define    SOUND_TEST_EFFECTS    3

FONT   *__gui_rendering_font  = "Arial#12b";
VECTOR *__gui_color_click__   = { x = 0; y = 0; z = 128; }
VECTOR *__gui_color_border__  = { x = 255; y = 255; z = 255; }
VECTOR *__gui_color_theme__   = { x = 0; y = 0; z = 0; }

int nCurrentMenu			= 0;
int nMessageBoxResult	    = 0;

BMAP *bmapMenuButtonOn			= NULL;
BMAP *bmapMenuButtonOff			= NULL;
BMAP *bmapOptionsChoiceBg		= NULL;
BMAP *bmapOptionsBg				= NULL;

BMAP *bmapComboboxOn				= NULL;
BMAP *bmapComboboxOff			= NULL;

BMAP *bmapSliderBg				= NULL;
BMAP *bmapSliderKnob				= NULL;

BMAP *bmapCheckBoxCheckedOn	= NULL;
BMAP *bmapCheckBoxCheckedOff	= NULL;
BMAP *bmapCheckBoxUncheckedOn	= NULL;
BMAP *bmapCheckBoxUncheckedOff= NULL;

PANEL* panStartMenu				= NULL;
PANEL* panInGameMenu				= NULL;
PANEL* panOptionsMenu			= NULL; // The top bar to switch between option panels
PANEL* panOptionsGame			= NULL;
	PANEL* panOptionsGameDifficulty	= NULL;
		TEXT* txtOptionsGameEasy					= NULL;
		TEXT* txtOptionsGameMedium					= NULL;
		TEXT* txtOptionsGameHard					= NULL;
PANEL* panOptionsGraphics		= NULL;
	PANEL* panOptionsGraphicsDetails				= NULL;
		TEXT* txtOptionsGraphicsDetailsLow		= NULL;
		TEXT* txtOptionsGraphicsDetailsMedium	= NULL;
		TEXT* txtOptionsGraphicsDetailsHigh		= NULL;
	PANEL* panOptionsGraphicsAntiAliasing		= NULL;
		TEXT* txtOptionsGraphicsAAOff				= NULL;
		TEXT* txtOptionsGraphicsAA1x				= NULL;
		TEXT* txtOptionsGraphicsAA4x				= NULL;
		TEXT* txtOptionsGraphicsAA9x				= NULL;
PANEL* panOptionsAudio			= NULL;
PANEL* panGraphicMenu			= NULL;
PANEL* panAudioMenu				= NULL;
PANEL* panCreditsMenu			= NULL;

TEXT* txtMenuNewGame				= NULL;
TEXT* txtMenuContinueGame		= NULL;
TEXT* txtMenuOptions				= NULL;
	TEXT* txtMenuOptionsGame		= NULL;
		// Game options
		TEXT* txtMenuDifficulty			= NULL;
		TEXT* txtMenuViolence			= NULL;
		TEXT* txtMenuShowDialogs		= NULL;
		TEXT* txtMenuShowHints			= NULL;
	TEXT* txtMenuOptionsGraphics	= NULL;
		TEXT* txtMenuFullscreen				= NULL;
		TEXT* txtMenuAntialias				= NULL;
		TEXT* txtMenuBrightness				= NULL;
		TEXT* txtMenuCurrentBrightness	= NULL;
		TEXT* txtMenuShader					= NULL;
		TEXT* txtMenuShadows					= NULL;
		TEXT* txtMenuDetails					= NULL;
	TEXT* txtMenuOptionsAudio		= NULL;
		TEXT* txtMenuMusicVolume			= NULL;
		TEXT* txtMenuMusicVolumeTest		= NULL;
		TEXT* txtMenuSpeechVolume			= NULL;
		TEXT* txtMenuSpeechVolumeTest		= NULL;
		TEXT* txtMenuEffectsVolume			= NULL;
		TEXT* txtMenuEffectsVolumeTest	= NULL;
TEXT* txtMenuOptionsApply		= NULL;
TEXT* txtMenuCredits				= NULL;
TEXT* txtMenuExitGame			= NULL;
TEXT* txtMenuReturnToWin		= NULL;
TEXT* txtMenuBack					= NULL;

PANEL* panMessageBoxBg			= NULL;
PANEL* panMessageBox				= NULL;
TEXT* txtMessageBoxCaption		= NULL;
TEXT* txtMessageBoxBtn1			= NULL;
TEXT* txtMessageBoxBtn2			= NULL;

SOUND* sndMusicTest				= NULL;
SOUND* sndSpeechTest			= NULL;
SOUND* sndEffectsTest			= NULL;

// Variables to hold a game state until changes are applied
var vNewGameDifficulty			= 0;
var vNewGameBlood				= 0;
var vNewGameShowDialogs			= 0;
var vNewGameShowHints			= 0;

var vNewGameAntiAlias			= 0;
var vNewGameBrightness			= 80;
var vNewGameShaders				= 0;
var vNewGameShadows				= 0;
var vNewGameDetails				= 1;
var vNewGameFullscreen			= 0;

var vNewGameMusicVolume			= 0;
var vNewGameSpeechVolume		= 0;
var vNewGameEffectsVolume		= 0;

var vInitialShaderVersion	= d3d_shaderversion;
var vGameDifficulty			= 1; // 1: Easy, 2: Medium, 3: Hard
var vGameBlood				= 1;
var vGameShowDialogs		= 1;
var vGameShowHints			= 1;
var vGameDetails			= 2; // 1: Low, 2: Medium, 3: High
var vGameMusicVolume		= 50;
var vGameSpeechVolume		= 50;
var vGameEffectsVolume		= 50;
int nInGame					= 1; // Is the game running or are we in the start menu?

/**
 * Creates all buttons for the menu
 */
void menu_init_buttons();

/**
 * Initializes all menus.
 */
void menu_init(const char *language_file, const int unicodeness);

/**
 * Shows one of the defined menus
 * \param	int	Type of menu -> see defines above
 */
void menu_show(int _menu);

/**
 * Hides all menus.
 */
void menu_hide();

/**
 * Fits a menu to the screen resolution
 * \param	int	Menu that should be aligned.
 */
void menu_align(int _menu);

/**
 * Centers all menus corresponding to the screen.
 */
void menu_center();

/**
 * Moves the menu to a certain position
 * \param	int	x-position of all menus
 * \param	int	y-position of all menus
 */
void menu_set_pos(int _x, int _y);

/**
 * Shows a message window that covers all other elements on screen.
 * \param	STRING*	Message to be shown
 * \param	STRING *	Message on the button (If NULL, no button is shown)
 */
void menu_show_message(STRING *_msg, STRING *_button);

/**
 * Shows a message box with 2 choices
 * \param	STRING*	Message to be shown
 * \param	STRING*	Caption of the first button.
 * \param	STRING*	Caption of the second button.
 * \return			0 if first button was clicked, 1 if second
 */
int menu_show_choice_message(STRING *_msg, STRING *_button1, STRING *_button2);

/**
 * Hide the message box programatically
 */
void menu_hide_message();

/**
 * Get the last message box result
 * \return	1 if first key pressed, 2 if second, 0 if message box was closed
 */
int menu_message_result();

/*
 * void menu_change_theme(int section, float r, float g, float b)
 *
 * Alters the current color of one part of the menu.
 * Valid constants (which is passed to the first parameter) are:
 * COLOR_AMBIENT  -- Will alter the "normal" (background) color of the menu.
 * COLOR_CLICKED  -- Will alter the color of the clicked component.
 * COLOR_BORDER   -- Will alter the color of the component's border.
 */
void menu_change_theme(int section, float r, float g, float b);

/*
 * void menu_change_sound_test(int section, const char *file)
 *
 * Alters the sound effect used for testing the volume in the main menu.
 * Possible constants (which is passed to the first parameter) are:
 * SOUND_TEST_MUSIC     -- Will alter the sound effect of the "music" slider.
 * SOUND_TEST_SPEECH    -- Will alter the sound effect of the "speech" slider.
 * SOUND_TEST_EFFECTS   -- Will alter the sound effect of the "effects" slider.
 */
void menu_change_sound_test(int section, const char *file);

/**
 * Click on the game, graphics, audio, input menu
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_options_choose(var _button_number, PANEL* _panel);

/**
 * Apply button clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_apply_click(var _button_number, PANEL* _panel);

/**
 * Back button clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_back_click(var _button_number, PANEL* _panel);

/**
 * A button on the start menu was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_start_click(var _button_number, PANEL* _panel);

/**
 * A button on the ingame menu was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_ingame_click(var _button_number, PANEL* _panel);

/**
 * A button on the difficulty panel was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_difficulty_click(var _button_number, PANEL* _panel);

/**
 * A button on the anti aliasing panel was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_anti_aliasing_click(var _button_number, PANEL* _panel);

/**
 * A button on the details panel was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_details_click(var _button_number, PANEL* _panel);

/**
 * Initializes message boxes (is called in menu_init())
 */
__static void menu_message_box_init();

/**
 * A button on of of the message boxes was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_message_box_click(var _button_number, PANEL* _panel);

/**
 * A button on of of the audio test boxes was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
__static void menu_audio_test_box_click(var _button_number, PANEL* _panel);

#include "gui_menu.c"
#endif /* gui_menu.h */
