/* download.h */
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
 * <download>
 * A very simple downloader header.
 *
 * In order for the header to work, you must have the DLL installed inside your
 * acknex_plugins folder, which can be obtained using this link:
 * https://dl.dropboxusercontent.com/u/26857618/acknex_plugins.zip
 * ...or by following the compilation instructions on GitHub.
 *
 * Authors: TechMuc (http://techmuc.kilu.de/)
 *          Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    __DOWNLOADER_DLL_H__
#define    __DOWNLOADER_DLL_H__

/*
 * fixed DownloadStart( const char *site, const char *file )
 *
 * Start the download and returns a unique identifier to the current download process.
 */
fixed DownloadStart( const char *file, const char *dest );

/*
 * fixed DownloadGetProgressInKBytes(fixed m_id)
 *
 * Returns the progress of a download ID (in KB)
 */
fixed DownloadGetProgressInKBytes(fixed m_id);

/*
 * fixed DownloadGetSize(fixed m_id)
 *
 * Returns the absolute size (to download) of a certain id (in KB)
 */
fixed DownloadGetSize(fixed m_id);

/*
 * fixed DownloadStop(fixed m_id)
 *
 * Stops a download.
 */
fixed DownloadStop(fixed m_id);

/*
 * fixed DownloadGetStatus(fixed m_id)
 *
 * Returns the status of an id (1 = downloading, 0 = not downloading)
 */
fixed DownloadGetStatus(fixed m_id);

#endif /* download.h */
