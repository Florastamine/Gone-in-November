/* realloc.c */
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
 * realloc.c
 * This demonstrates how you can implement a similar alternative to C++'s std::vector<>.
 * This is used as the building blocks for <serializer>, because apparently providing just 
 * a big ass, fixed chunk of memory to be filled in later is not smart, not smart at all.
 * 
 * A similar vanilla C recreation can be found in realloc_C.c.
 */
#include <acknex.h>
#include <stdio.h>
#include "..\utilities.h"

#define ENFORCE_STRICT

typedef struct {
	int *i;

	int pos;
	int size;
} box;

box *b = NULL;

int *mrealloc( int **in, int n, int copyn )
{
	int oldval = 0;
	
	int *backup = (int *) malloc(copyn * sizeof(int));
	*backup = *in;
	oldval = **in;
	
	int i = 1;
	while(i < copyn - 1)
	{
		*(backup + i) = *( *in + i);
		++i;
	}

	int *w = (int *) realloc( *in, sizeof(int) * n );

	if(w != NULL)
	{
		
		i = 1;
		*w = oldval;
		while(i < copyn - 1)
		{
			// printf("(backup+%i) = %i\n", i, *( backup + i));
			*(w + i) = *( backup + i);
			// printf("(w + %i) = %i\n", i, *( w + i));
			++i;
		}
	}
	else
	w = *in;

	return w;
}

void setval(box *b)
{
	int i = 0;
	for(; i < b->size; i++)
	{
		*(b->i + i) = i;
		(b->pos)++;
	}
}

void printval(box *b)
{
	printf("\n");
	int i = 0;
	for(; i < b->pos; i++) printf("b%i = %i\n", i, *(b->i + i));
}

void printvalpos(box *b, int pos)
{
	printf("b%i = %i\n", pos, *(b->i + pos));
}

void addval(box *b, int i)
{
	if(b->pos <= b->size - 1)
	{
		*(b->i + b->pos) = i;
	}
	else
	{
		int newsize = b->pos + 1;
		b->i = mrealloc( &(b->i), newsize, newsize);
		if(b->i)
		{
			*(b->i + newsize - 1) = i;
		}
	}
	(b->pos) += 1;
}

void quit() { sys_exit( (void *) 0 ); }

int main( void )
{
	/*
	 * stdout redirection - to the startup window instead.
	 */
	video_screen = 0;
	printf("\n");
	BIND_KEY(esc, quit);
	random_seed(0);
	
	/*
	 * Initializes the struct with an initial size of 2 - whenever something else got pushed to the box, 
	 * realloc() will expand original size of the array.
	 */
	b = MALLOC(1, box);
	b->size = 2;
	b->pos  = 0;
	b->i = MALLOC(b->size, int);

	int i = 0;
	while(i < b->size )
	{
		addval(b, i);
		i += 1;
	} // <- At this point, any addval() call should expand the array storage by 1 int, like std::vector<>.

	printval(b);
	
	for(i = 0; i < 10; ++i)
	    addval(b, rand() % 65536);
	
	printval(b);
	printf("%i", (int) b->pos );
}
