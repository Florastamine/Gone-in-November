/* main.wdl */
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
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 */
//title: WDF
#define = 
WINDOW WINSTART
{
	// section: 
	// entry: Window title
	TITLE "Gone in November";
	
	SIZE 500, 661;
	
	BG_COLOR RGB(240, 240, 240);
	
	/*
	 * Prints "Loading..."
	 */
	// entry: Arbitrary text
	TEXT "Loading...", 10, 10;	
	
	// entry: Text font
	SET FONT "Verdana", RGB(0,0,0);
	
	// enable: Progress bar
	PROGRESS RGB(0,0,255), 42, 5, 560, 495, 12.5;
	
	// enable: Launch button
	BUTTON BUTTON_START, SYS_DEFAULT, "Launch", 212, 600, 75, 30;
	
	/* 
	 * Style can be either STANDARD, IMAGE, FULLSCREEN, or USE_OLDSTYLE.
	 */
	MODE IMAGE;	
	
	/* Frame
	 * type (1 - 4), x, y, width, height.
	 */
	FRAME FTYP3, 0, 0, 500, 661;
	
	/* Engine messages.
	 * x, y, width, height.
	 */
	// enable: Output engine messages
	TEXT_STDOUT "Consolas", RGB(0, 0, 0), 10, 30, 500, 240;
	
	/* Background image. PCX, 255. */
	PICTURE <1.pcx>, OPAQUE, 0, 0;
	
	// entry: Parsing additional cmds
	COMMAND "-diag";
	
	// section: 
} /* main.wdl */

