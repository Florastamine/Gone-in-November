/* text_reader.h */
/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                     Version 2, December 2004
 *
 *  Copyright (C) 2004 Sam Hocefixed <sam@hocefixed.net>
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
 * <text_reader>
 * A generic text file processing DLL, which simplifies the burden of using
 * the native Acknex file functions. You can also read the file line-by-line.
 *
 * In order for the header to work, you must have the DLL installed inside your
 * acknex_plugins folder, which can be obtained using this link:
 * https://dl.dropboxusercontent.com/u/26857618/acknex_plugins.zip
 * ...or by following the compilation instructions on GitHub.
 *
 * Authors: TechMuc (http://techmuc.kilu.de/)
 *          Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#ifndef    __TEXT_READER_DLL_H__
#define    __TEXT_READER_DLL_H__

/*
 * fixed TextFileRead(char *fl)
 *
 * opens a specific file which already exists, and returns a unique identifier,
 * with which you can access this file in further instructions.
 */
fixed TextFileRead(char *fl);

/*
 * fixed TextFileCreate(char *str)
 *
 * creates a specific file which does not exist.
 */
fixed TextFileCreate(char *str);

/*
 * fixed TextFileWrite(fixed m_id)
 *
 * Writes all changes you've made to the opened/created file
 */
fixed TextFileWrite(fixed m_id);

void TextFileClose(fixed m_id);
void TextFileAddLine(fixed m_id, char *str);
void TextFileInsertLine(fixed m_id, fixed m_line, char *str);
void TextFileRemoveLine(fixed m_id, fixed m_line);

fixed TextFileGetLineCount(fixed m_id);

char *TextFileGetLine(fixed m_id, fixed line);

#endif /* text_reader.h */
