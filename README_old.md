# Gone In November

Welcome to Gone In November's official development page!

Gone In November is a first-person exploration, surreal interactive story which is currently in active development by one person and it is written with the help of the Acknex rendering/game library.

As the game code is updated directly on this GitHub page, you can easily tell if the game is still being developed just by looking at the last commit's date. If it was made more than a month ago, then something must be wrong!

If you wish to collaborate, let me know. I'll be very happy! (as the game is progressing fairly slow). Leave me a message at:
* My personal Gmail account (florastamine@gmail.com)
* My vnsharing account (http://vnsharing.site/forum/member.php?u=15466)
* My opserver.de/ubb7 account (http://www.opserver.de/ubb7/ubbthreads.php?ubb=showprofile&User=18896)
* My Game Jolt account (http://gamejolt.com/profile/huy-nguyen/410187)
* My Steam account (http://steamcommunity.com/id/Florastamine/)

More information (early releases, progress, news) can be found on the game's main web page: [Here!](http://florastamine.github.io/Gone-in-November/)

## Instructions on how to build the source code
The source code cannot be built at the moment as it requires game assets which unfortunately cannot be given out due to licensing problems. You will need to wait for the game to be released.

### What you'll need:
* Git (I use [git-scm](https://git-scm.com/))
* [Atom](http://atom.io/), or your favorite text editor.
* Acknex toolset (which can be downloaded [here](http://server.conitec.net/down/gstudio8_setup.exe))
* The original game.
* Custom game DLLs, which you can obtain [here](https://dl.dropboxusercontent.com/u/26857618/acknex_plugins.zip). Unzip everything to your `acknex_plugins` folder.
* A [Go](https://golang.org/) compiler for building the game launcher. **(optional!)**
* [g++](https://gcc.gnu.org/) for building the messenger. **(optional!)**

### Downloading & Installing the toolset
Since the game is written in **Lite-C** (which is actually C but with some elements borrowed from C++), you will need a special Lite-C toolset to be able to compile GiN, which can be downloaded [here](http://server.conitec.net/down/gstudio8_setup.exe). Upon installing the tools, select the option to install the free version. **(GiN can still be compiled with the free version, but shaders will not be enabled)**.

~~Note that, when viewing the repository within [GitHub.com](https://github.com/), you can see a small portion of the code is marked as the "**FLUX**" language. This is not correct - they are Direct3D shader code (HLSL).~~ Seems like the issue was resolved [with the release of Linguist 4.8.0](https://github.com/github/linguist/pull/2842).

### Getting the source code & Preparing
I recommend building from the "tagged" releases as it is guaranteed to be stable. Thus, when you've done cloning the repository, make sure to "revert" the codebase to the nearest tag before compiling.
```bash
$ git clone https://github.com/Florastamine/Gone-in-November
$ cd ./Gone-in-November
$ git tag -l
$ git checkout tags/<tag_name>
```

Next, you'll need to modify some environment variables... Open up **./utils/ackvars.bat**. These are essential variables (therefore must be changed) for compiling the game:

#### REPOPATH
You have to specify the **absolute path** to the repository you were just cloned.

#### GAMEPATH
You have to specify the absolute path or relative path to the game folder, which contains the game resource files. Otherwise (if the resources couldn't be found), Lite-C compiler will try to compile the resources, which will fail (resources not presented).

#### ACKPATH
Absolute path to the Acknex toolset folder (which contains **acknex.exe**)

A complete list of environment variables can be found in the following files: [**./utils/ackvars.md**](https://github.com/Florastamine/Gone-in-November/blob/master/utils/ackvars.md), which contains a list of variables and their descriptions; [**./utils/ackvars.bat**](https://github.com/Florastamine/Gone-in-November/blob/master/utils/ackvars.bat), which is the actual "header", containing the variables for use during the compiling process.

### Compiling/Running the source code
After you've changed these variables in **./utils/ackvars.bat**, open **./November.c** with your text editor/IDE, and right after the first comment block, comment out the ```A8_FREE``` switch (if you're using the free version). Now simply run **./utils/build.bat** to start the build process. The result files will be in the **./builds/** folder.

To run the project, simply move the resource file to the root of your cloned repository, and run **./utils/run.bat**. Or just open up SED, navigate to **./November.c** and hit F6.

If you have a [Go](https://golang.org/) compiler lying around, the build script will also automagically build the game launcher for you. This is an optional step, as the game can still be started without the launcher.

If [g++](https://gcc.gnu.org/) is available for use through ```GNUPATH``` (defined in **./utils/ackvars.bat**), the build script will attempt to compile the messenger. This is also an optional step, as you can just copy the appropriate executable from your original copy of GiN.

## License
Except most of the code in `./source/render/` and game assets, everything else is released under the [WTFPL](http://www.wtfpl.net/) license, which means you can literally do whatever the f*ck you want with it.

## Screenshots
![](http://dl.dropboxusercontent.com/u/26857618/GIN1.jpg)
![](http://dl.dropboxusercontent.com/u/26857618/GIN2.jpg)
![](http://dl.dropboxusercontent.com/u/26857618/GIN3.jpg)
![](http://dl.dropboxusercontent.com/u/26857618/GIN4.jpg)
