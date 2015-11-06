/* crypto.h */
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
 * <crypto>
 * A dead simple cryptography library, with the *sole* intention is only 
 * for learning purposes, and to hide data (for example serialized save games)
 * from the users who want to peek over them.
 * 
 * Currently does not support Unicode (yet).
 * 
 * Authors: Huy Nguyen (http://vn-sharing.net/forum/member.php?u=15466)
 * __________________________________________________________________
 */
#ifndef    _CRYPTO_H_
#define    _CRYPTO_H_

#include "file.h"

#define __In 
#define __Out 

#define __namespace(namespace)             {}

#define    __SHIFT 75 

__namespace(Darwin) {
	void encrypt_string( __In STRING *sstr, __In STRING *estr, __In int shift );
	void encrypt_string( __In STRING *sstr, __In STRING *estr );
	
	void decrypt_string( __In STRING *sstr, __In STRING *estr, __In int shift );
	void decrypt_string( __In STRING *sstr, __In STRING *estr );
}

#include "crypto.c"
#endif /* crypto.h */
