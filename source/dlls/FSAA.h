/* FSAA.h */
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
 * <FSAA>
 * FSAA plugin by ChrisB, replacing the engine's built-in anti-aliasing functionality.
 * The cool thing is it can be adjusted and switched in real-time, whereas the engine's
 * built-in one (which can be activated through d3d_antialias) is fixed.
 * Remember, if you're reading this header, be sure to check his cool works at
 * www.swollen-eyeballs.org!
 *
 * In order for the header to work, you must have the DLL installed inside your
 * acknex_plugins folder, which can be obtained using this link:
 * https://dl.dropboxusercontent.com/u/26857618/acknex_plugins.zip
 * ...or by following the compilation instructions on GitHub.
 *
 * Authors: ChrisB (www.swollen-eyeballs.org)
 *          Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    __FSAA_DLL_H__
#define    __FSAA_DLL_H__

dllfunction    fsaa_maxQualityNonMaskable();
dllfunction    fsaa_eventSky();
dllfunction    fsaa_eventEntities();
dllfunction    fsaa_eventLost();
dllfunction    fsaa_setQuality(fixed);

#endif /* FSAA.h */
