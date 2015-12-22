# Gone In November

Welcome to Gone In November's official development page!

Gone In November is a first-person exploration, surreal interactive story about rain, solitude, being forgotten and loss. It is currently in active development by one person and it is written with the help of the Acknex rendering/game library.

## Description
This directory is where configuration/setting files resides.
Configuration files (video/audio settings, arguments list...) usually have the **.cfg** extension. 

## Feeding arguments to the game executable
Several options are avble for feeding arguments to the executable; each option has a shorter form, for example, `-c` is equivalent to `--console`. For a list of full arguments, please refer to `./source/game/common/common.h`.

### `--console` or `-c`
Opens the console. It differs from the default console provided in `<default>`, since it manages and processes its own commands instead of the full set of Gamestudio commands and variables.

### `--no-log` or `-nl`
Do not log events into the logging file. Can boost performance by a small amount, since some calls require the `str_printf()` instruction and a temporary string, which can be quite expensive.

### `--force-low` or `-fl`
Suppresses fragment & vertex shader check, and do not render any of the surface shaders nor post-process effects.

### `--decryptor` or `-d`
Opens the save game editor. Only available only if the game isn't saved to the Gamestudio format (`.sav`)
