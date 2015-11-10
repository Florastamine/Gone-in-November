#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int *i;

	int pos;
	int size;
} box;

box *b = NULL;

int *mrealloc( int **in, int n, int copyn )
{
	int *backup = (int *) malloc(sizeof(int));
	int i = 0;
	while(i < copyn - 1)
	{
		*(backup + i) = *( *in + i);
		++i;
	}

	int *w = (int *) realloc( *in, n );
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

int main( void )
{
	// Initialize the box.
	b = (box *) malloc(sizeof(box));
	b->size = 5;
	b->pos  = 0;
	b->i = (int *) malloc(b->size * sizeof(int));

        int i = 0;
        while(i < b->size )
        {
            addval(b, i);
            i += 1;
        }

	printval(b);

	addval(b, 15);
	addval(b, 17);
	addval(b, 15665);
	addval(b, 42);

	printval(b);
}
