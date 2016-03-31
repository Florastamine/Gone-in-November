# Deprecated and/or removed headers

```/dlls/bmap_ex.h```
In the start, the game was designed to be multiple chapters. This bitmap header was then created so that whenever a chapter was completed, its thumbnail image (similar to the one in Half-Life 2) is greyed out with a simple call to one of the bmap_*() functions. But later, at some point in the development process, the original idea was scrapped and (what you see now) is just a single-level, 20-minute game, so this header is never used.

```/dlls/download.h```
This header was created to resolve the automatic updates functionality, however the plugin cannot work with downloaded archives (whether it's .tar or .zip), plus the user have to manually restart the game after the process. So this header is also unused and a separate golang based-launcher was created. 

```/game/anlang/*```
These headers were supposed to act as a simple scripting language to separate the core code from the game-specific code. However, I've decided that a separate scripting language was too complex and time-wasting at the time.

```/game/common/inventory.h```
Originally the game design allows for a simple inventory system, but later as the design changed, the header is no longer needed.

```/game/io/file.h```
This header was supposed to wrap around Gamestudio's file_*() functions, making them easier to use, but later got removed because of 

```/game/io/trophy.h```
This header serves as some kind of achivements system, but later got deprecated because of a number of reasons, one of which is integration with Steam achivements would render the header obsolete.

```/game/io/crypto.h```
This header provides some basic cryptography functionalities that operate on STRING objects, but it is never used as this is a simple game (after the design and the storyline changed).

```/game/io/serializer.h```
This header provides a simple interface for serializing a few basic data types to plain text files that can be read out/deserialized later. It has an annoying bug (#8).
