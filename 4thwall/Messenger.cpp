/* November.cpp */
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
 * __________________________________________________________________
 * A small program which aids in breaking the fourth wall by front-face-ly
 * sending messages to the player. (MessageBox())
 * __________________________________________________________________
 */
#define    WIN32_LEAN_AND_MEAN
#define    WIN32_EXTRA_LEAN
#define    NOCRYPT
#define    NOGDI

#define    _WIN32_WINNT    0x500

#include <windows.h>
#include <shellapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int argc{}, i{};
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if(argv == NULL)
        return 0;

    if(argc)
        for(int i = 1; i < argc; i++)
            MessageBoxW(NULL, argv[i], argv[i], MB_OK | MB_ICONEXCLAMATION);

    LocalFree(argv);
}
