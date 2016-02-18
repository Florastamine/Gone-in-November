#cs
   A very basic AutoIT script which does all the 32-to-24-bit skin conversion (through the manner of GUI automation)
   because apparently Acknex have problems rendering models with 32-bit skins (which were exported through LightUp).

   For now the script does not lock the user input during its execution so please do *NOT* touch anything while the script
   is running. (TODO: Block user input during script execution).

   Tweakable variables (please read them carefully!):
   * $SCENE_SKINS: Total number of skins to be modified.
   * $MED_PATH: Absolute path to your MED installation.
   * $MED_WAIT_TIME: Time to wait for MED to open itself.
   * $REPO_PATH: Absolute path to the game's repository.
   * $SCENE_NAME: Scene name, which can be found under <game repository directory>/scene
   * $SCENE_IMPORT_TIME: An approximation of how long the scene importer takes to finish its job. Grabbing the progress bar's value is way too complex,
   * especially with a total AutoIT noob like me (this is also my first attempt at writing AutoIT scripts), so I just use a small delay. Better use a medium-to-large value.

   How to use
   -------------------------
   1. Download the AutoIT package.
   2. Open SciTE (which can be found inside the AutoIT installation/archive) and open the script.
   3. Adjust the global variables (except $__WID__) to your needs.
   4. Open the AutoIT runtime and run the script.
   -------------------------

   Release history
   -------------------------
   - v0.1.0 (Feb. 09 2016)
        * First release.
   - v0.1.1 (Feb. 15 2016)
        * Increased $MED_WAIT_TIME.
        * Fixed some spelling/grammar errors and rewrote the script file for readability and cleanliness.
   -------------------------

   Written by Huy Nguyen (https://github.com/Florastamine)
#ce

#RequireAdmin

#include-once
#include "_utilities.au3"

Global Const $MED_PATH          = "H:\Development\Gamestudio\GStudio8"
Global Const $MED_WAIT_TIME     = 10.0

Global Const $REPO_PATH         = "H:\Development\DPFL"

Global Const $SCENE_NAME        = "obstacle"
Global Const $SCENE_IMPORT_TIME = 1000
Global Const $SCENE_SKINS       = 3

Global Const $__SCENE_SKIN_ORDER__[3] = [0, 1, 0]
Global Static $__WID__[2] ; To prevent concurrency.

fmain()

Func fmain()
   ; BlockInput() does not work correctly under Windows XP SP1. (https://www.autoitscript.com/autoit3/docs/functions/BlockInput.htm)
   u_assert(@OSVersion <> "WIN_XP" And @OSVersion <> "WIN_XPe", "Windows XP is not supported!")

   ; See if MED can be found under $MED_PATH, and run it.
   Local Const $__MED_PATH__ = $MED_PATH & "\med.exe"
   u_assert(FileExists($__MED_PATH__), "Cannot open MED at the specified location.")

   ; Run MED and wait for the main window to become active.
   Run($__MED_PATH__)
   Local $hWnd = WinWait("med", "")
   WinActivate($hWnd)

   ; Lock user controls.
   lock_controls($hWnd)

   ; Import the .fbx file.
   import_models($hWnd)
   While Not ($__WID__[0] = True)
   WEnd

   ; Fix its skins' bit depth.
   fix_bits($hWnd)
   While Not ($__WID__[1] = True)
   WEnd

   ; Everything is done, unlock user controls and exit the program.
   unlock_controls($hWnd)
   Exit
EndFunc

Func import_models($hWnd)
   Send("{LALT}+{F}") ; File Menu
   Send("{I}")        ; Import

   Local $count = 0

   While $count < 3        ; Navigate to import FBX (2010).
	  Send("{DOWN}")
	  $count += 1
   WEnd
   Send("{ENTER}")

   u_window_wait_active("[CLASS:#32770]") ; Wait for the window to become active and then send our string, and hit {ENTER}.
   Send($REPO_PATH & "\scene\" & $SCENE_NAME & '\' & $SCENE_NAME & ".fbx", 1)
   Send("{ENTER}")

   u_window_wait_active("[CLASS:#32770]") ; Wait for the importer window to become ready and then we hit the "Convert to points animation" switch.
   $count = 0
   While $count < 4
	  Send("{DOWN}")
	  $count += 1
   WEnd
   Send("{SPACE} + {ENTER}")

   Sleep($SCENE_IMPORT_TIME) ; Wait for the importer to finish its job. For the sake of simplicity,
                             ; I only use time approximation (in miliseconds), rather than
							 ; taking the burden of getting the progress bar's handle, get the progress bar's value
							 ; from the handle... that'd be way too complex and would distract me away from the project.
   Send("{ENTER}")

   $__WID__[0] = True
EndFunc

Func fix_bits($hWnd)
   Local $i = 0

   While $i < $SCENE_SKINS
	  Local $ret = __set_bits_for_skin($hWnd, $__SCENE_SKIN_ORDER__[$i])
	  While Not $ret = True
	  WEnd

	  Sleep(500) ; Magic

	  $i += 1
   WEnd

   $__WID__[1] = True
EndFunc

Func __set_bits_for_skin($hWnd, $ID)
   WinActivate($hWnd)
   Send("{LALT}+{O}") ; Open the "Manage Skins" window.

   Local $count = 0
   While $count < 3
	  Send("{DOWN}")
	  $count += 1
   WEnd
   Send("{ENTER}")

   Local $tsstitle = "Skin Settings"
   Local $tstitle  = "Skins"

   ControlClick($tstitle, "", "[CLASS:ListBox; INSTANCE:1]") ; Click and navigate through the skin list
   u_window_wait_active("[CLASS:#32770]")
   Send("{SPACE}")

   If Not $ID = 0 Then
	  $count = 0
	  While $count < $ID
		 Send("{UP}")
		 $count += 1
	  WEnd
   EndIf

   ; Re-set the skin bits to 24.
   ControlClick($tstitle, "", "[CLASS:Button; INSTANCE:3]")
   u_window_wait_active("[CLASS:#32770]")
   ControlClick($tsstitle, "", "[CLASS:Button; INSTANCE:27]")
   ControlClick($tsstitle, "", "[CLASS:Button; INSTANCE:34]")
   ControlClick($tsstitle, "", "[CLASS:Button; INSTANCE:29]")

   ; Close the windows.
   Local $i = 0
   While $i < 2
	  Local $b = u_window_close_active()
	  While Not ($b = True)
	  WEnd

	  $i += 1
   WEnd

   Return True
EndFunc

Func lock_controls($hWnd)
   ; BlockInput($BI_DISABLE)
EndFunc

Func unlock_controls($hWnd)
   ; BlockInput($BI_ENABLE)
EndFunc
