/* utilities.c */
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
 * void __assert( const char *message )
 * 
 * A very simple assert() implementation.
 * It has to relie on the parameter alone, because Lite-C doesn't have support for __func__
 */
void __assert( const char *message )
{
	// TODO: Transform 384 into some sort of constant. 
	// TODO: Handle NULL argument.
	char *__string = MALLOC(384, char);
	fixed f = file_open_append("lassert.log");
	
	sprintf( __string, "[%i:%i, %i:%i:%i] Assertion failed: %s",
	(int) sys_hours,
	(int) sys_minutes,
	(int) sys_day,
	(int) sys_month,
	(int) sys_year,
	message );
	
	file_str_write(f, __string);
	file_asc_write(f, 13);
	file_asc_write(f, 10);
	
	free(__string);
	sys_exit((void *) 0);
}

/*
 * void swap( int *a, int *b )
 *
 * Swaps two integers. For general-purpose swapping, please
 * refer to the SWAP() macro. (not recommended)
 */
void swap( int *a, int *b )
{
	int c = *a + *b;
	*a = c - *a;
	*b = c - *b;
}

/*
 * void pair_set( pair *p, float first, float second )
 * 
 * Fills a pair with values.
 */
void pair_set( pair *p, float first, float second )
{
	if(!p) return;
	p->first = first;
	p->second = second;
}

/*
 * void pair_swap( pair *p1, pair *p2 )
 * 
 * Swaps two pairs.
 */
void pair_swap( pair *p1, pair *p2 )
{
	if( !p1 || !p2 ) return;
	
	// FIXME: It's ugly.
	pair p3;
	pair_set( &p3, p1->first, p1->second );
	pair_set( p1, p2->first, p2->second );
	pair_set( p2, p3.first, p3.second );
}

/*
 * double log10( double d )
 *
 * Calculates log(10, d).
 * log(10, d) = log(e, x) / log(e, 10) = ln(x) / 2.30258509.
 */
double log10( double d )
{
	return log(d) / 2.30258509;
}

/*
 * int join( int i, int j )
 * 
 * Joins two integers.
 * Because we only need the integer fraction, a cast to int is required, otherwise serious issues would happen.
 */
long join( int i, int j )
{
	int time = 1;
	
	if( j != 0 ) time = pow( 10.0, (int) log10(j) + 1.0 );
	return (i * time) + j;
}

/*
 * void split(int *__result, int value, int slot)
 *
 * Split a value's digit characters into an integer array.
 * The third parameter controls at which position the digits are
 * inserted into the array. So you can insert the digits
 * to your array which already has data.
 */
void split(int *__result, int value, int slot)
{
	// TODO: Handle NULL.
	if( !__result ) return;
	
	if( slot <= 0 ) slot = 0;
	int count = 0, digit = 0;
	while(value)
	{
		digit = value % 10;
		value = value / 10;
		count += 1;
		if( __result + slot + (count - 1) ) *(__result + slot + count - 1) = digit;
	}
}

void split(int *__result, int value)
{
	split(__result, value, 0); // Spit the numbers at the beginning of the array by default.
}

/*
 * void reverse(int *array, int count)
 * 
 * Reverse count elements in the specified array.
 * Because Lite-C doesn't have built-in support for determining
 * a given array's size (sizeof() doesn't work), and to give the 
 * user more control over how many elements can be reversed,
 * an additional second parameter is provided.
 */
void reverse(int *array, int count)
{
	// TODO: Handle NULL.
	if( !array || !count ) return;
	
	int i = 0, j = 0;
	while(i < count / 2)
	{
		swap( *(array + i), *(array + count - 1 - i) );
		i++;
		
		wait(1.0);
	}
}

/*
 * String *str_clip_ex( String *str, int n )
 *
 * An extension to str_clip() which returns the cut string.
 */
String *str_clip_ex( String *str, int n )
{
	// TODO: Handle NULL.
	String *temp = str_create(str);
	str_trunc(temp, str_len(str) - n);
	str_clip(str, n);
	
	return str;
}
