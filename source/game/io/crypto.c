/* crypto.c */
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
 */

/*
 * void encrypt_string( STRING *sstr, STRING *estr, int shift )
 *
 * Performs encrypting on a single string. Both the source string
 * (sstr) and the container (estr) must be valid.
 * Additionally a custom shift value can be passed.
 *
 * Another variant using the default shift macro (__SHIFT) can be found below.
 */
void encrypt_string( STRING *sstr, STRING *estr, int shift )
{
	if( !sstr || !estr )
		return;

	sstr = str_create(sstr);
	estr = str_create("");

	STRING *buffer = "";
	int counter = 0;
	int temp_val = 0;
	int length = str_len(sstr);

	while( counter < length )
	{
		str_cpy(buffer, sstr);
		str_clip(buffer, counter);
		str_trunc(buffer, length - counter - 1);
		temp_val = str_to_asc(buffer) + shift;
		str_for_asc(buffer, temp_val);
		str_cat(estr, buffer);

		counter++;
	}
}

void encrypt_string( STRING *sstr, STRING *estr )
{
	encrypt_string(sstr, estr, __SHIFT);
}

/*
 * void decrypt_string( STRING *sstr, STRING *estr, int shift )
 *
 * Performs decrypting on a single string. Both the source, encrypted string
 * (sstr) and the un-encrypted string container (estr) must be valid.
 * Additionally a custom shift value can be passed.
 */
void decrypt_string( STRING *sstr, STRING *estr, int shift )
{
	encrypt_string(sstr, estr, - shift);
}

void decrypt_string( STRING *sstr, STRING *estr )
{
	decrypt_string(sstr, estr, __SHIFT);
}
