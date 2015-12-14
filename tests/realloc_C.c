/* realloc_C.c */
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
 * realloc_C.c
 * This demonstrates how you can implement a similar alternative to C++'s std::vector<> for C, using realloc().
 * This is used as the building blocks for <serializer>, because apparently providing just
 * a big ass, fixed chunk of memory to be filled in later is not smart, not smart at all.
 */

#ifdef    LC // If you're using the Acknex compiler, enable LC
#include <litec.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
	int *i;

	int pos;
	int size;
} box;

box *b = NULL;

int *mrealloc( int **in, int n, int copyn );

void setval(box *b);
void printval(box *b);
void printvalpos(box *b, int pos);
void addval(box *b, int i);

int main( void )
{
	b = (box *) malloc(sizeof(box));
	b->size = 5; // Five elements at startup
	b->pos  = 0;
	b->i = (int *) malloc(b->size * sizeof(int));

	setval(b); // Fill the box with values. From now on, any call to addval() will automagically creates additional memory.
	printval(b);

	int i;
	for(i = 0;i < 15; ++i)
		addval(b, rand() % 1000);

	printval(b);

	return 0;
}

int *mrealloc( int **in, int n, int copyn )
{
	int *backup = (int *) malloc(copyn * sizeof(int));
	int i = 0;
	while(i < copyn - 1)
	{
		*(backup + i) = *( *in + i);
		++i;
	}

	int *w = (int *) realloc( *in, sizeof(int) * n );
	if(w)
	{
		printf("new memory!\n");
		i = 0;
		while(i < copyn - 1)
		{
			printf("(backup+%i) = %i\n", i, *( backup + i));
			*(w + i) = *( backup + i);
			printf("(w + %i) = %i\n", i, *( w + i));
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
	(b->pos)++;
	printf("pos = %i\n", b->pos);
}

