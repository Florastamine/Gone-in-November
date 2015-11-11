/* serializer.h */
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
 * <serializer>
 * A simple serializer engine with a std::vector<>-like archive.
 * 
 */
#ifndef    _SERIALIZATION_H_
#define    _SERIALIZATION_H_

#define __static 
#define __In
#define __Out
#define __namespace(namespace)             {}

__namespace(serializer) {
	typedef struct {
		Text *pointer_string;
		int pos_string;
		
		float *pointer_float;
		int pos_float;
		
		int *pointer_int;
		int pos_int;
		
		int __initial_size;
	} Archive;
	
	Archive *archive_new( __In int size );
	void archive_free( __In __Out Archive *arch );
	
	void      serialize( __In Archive *arch, __In fixed channel );
	Archive   *deserialize( __In fixed channel );
	
	int archive_get_initial_size( __In Archive *arch );
	
	int archive_get_int( __In Archive *arch, __In int pos );
	float archive_get_float( __In Archive *arch, __In int pos );
	STRING *archive_get_string( __In Archive *arch, __In int pos );
	
	int archive_get_int_size      ( __In Archive *arch );
	int archive_get_float_size    ( __In Archive *arch );
	int archive_get_string_size   ( __In Archive *arch );
	
	void archive_set_string( __In __Out Archive *arch, __In STRING *data, __In int pos );
	void archive_set_float( __In __Out Archive *arch, __In float data, __In int pos );
	void archive_set_int( __In __Out Archive *arch, __In int data, __In int pos );
	
	void archive_store_string( __In __Out Archive *arch, __In STRING *data );
	void archive_store_float( __In __Out Archive *arch, __In float data );
	void archive_store_int( __In __Out Archive *arch, __In int data );
}

#include "serializer.c"
#endif /* serializer.h */
