/* windows+.h */
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
 * 
 * <windows+>
 * 
 * Contains Windows API headers that are currently missing in <windows>.
 * __________________________________________________________________
 */
#ifndef    __LC_WINDOWSP_H__
#define    __LC_WINDOWSP_H__

#include <windows.h>

/*
 * For console functions.
 */
long WINAPI WriteConsole(int Handle, char* Buffer, int CharsToWrite, int* CharsWritten, int reserved);
long WINAPI CreateConsoleScreenBuffer(long dwDesiredAccess, long dwShareMode, long *lpSecurityAttributes, long dwFlags, long lpScreenBufferData);
long WINAPI SetConsoleActiveScreenBuffer(long hConsoleOutput);

#endif /* windows+.h */
