#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ants.h"

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

typedef struct node Node;

struct node {
    Node *next;
    Coordinate key;
    int value;
};

struct dict {
    int size; // size of the pointer table */
    int numNodes; // number of elements stored */
    Node **table; // this is a pointer to an array of Nodes
};

/* dictionary initialization code used in both DictCreate and grow */
Dict internalDictCreate(int size)
{
    // Dict myDict = malloc(sizeof(Dict));
    // VERSUS
    Dict myDict = malloc(sizeof(Dict) * size);
    myDict->size = size;
    myDict->numNodes = 0;
    myDict->table = calloc(myDict->size, sizeof(Node));

    return myDict;
}

Dict dictCreate(void)
{
    return internalDictCreate(INITIAL_SIZE);
}

void dictDestroy(Dict myDict) // need to free each node in the dict
{
    int i;
    Node *tempNode;
   	Node *next;

    for (i = 0; i < myDict->size; i++) 
    {
        for (tempNode = myDict->table[i]; tempNode != 0; tempNode = next)
        {
            next = tempNode->next;
            free(tempNode);
        }
    }

    free(myDict->table);
    free(myDict);
}

static unsigned long hashFunction(int x, int y, int z)
{
	unsigned long output = 0;
	output = (x * 53) ^ (y * 97) ^ (z * 193);

	return output;
}

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void dictDelete(Dict d, Coordinate key)
{
    Node **prev;          /* what to change when elt is deleted */
    Node *e;              /* what to delete */

    for (prev = &(d->table[hashFunction(key.xCoord, key.yCoord, key.zCoord) % d->size]); *prev != 0; prev = &((*prev)->next)) 
    {
        if ((*prev)->key.xCoord == key.xCoord && (*prev)->key.yCoord == key.yCoord && (*prev)->key.zCoord == key.zCoord)
        {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e);

            return;
        }
    }
}

static void grow(Dict d)
{
    Dict d2;            /* new temporary dictionary we'll create */
    struct dict swap;   /* temporary structure for brain transplant */
    int i;
    Node *e;

    d2 = internalDictCreate(d->size * GROWTH_FACTOR);

    for (i = 0; i < d->size; i++)
    {
        for (e = d->table[i]; e != 0; e = e->next)
        {
            dictInsert(d2, e->key, e->value); // inserts e->key and e->value into d2
        }
    }
    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call DictDestroy on d2 */
    swap = *d; // so swap is a dictionary containing the contents of original dictionary
    *d = *d2; // the contents of original dictionary is the new dictionary
    *d2 = swap; // the new dictionary is now equal to the contents of the original?

    dictDestroy(d2);
}

/* insert a new key-value pair into an existing dictionary */
void dictInsert(Dict d, Coordinate key, int value)
{
	dictDelete(d, key);

    Node *e = malloc(sizeof(Node));

    assert(e);

    e->key = key;
    e->value = value;

    unsigned long h = hashFunction(key.xCoord, key.yCoord, key.zCoord) % d->size;

    e->next = d->table[h];
    d->table[h] = e;
    d->numNodes++;

    /* grow table if there is not enough room */
    if (d->numNodes >= (d->size * MAX_LOAD_FACTOR)) 
    {
        grow(d);
    }
}

// return the character at the given coordinates or -1 if no matching key is present
const int dictSearch(Dict d, const Coordinate key)
{
    Node *e;

    for (e = d->table[hashFunction(key.xCoord, key.yCoord, key.zCoord) % d->size]; e != 0; e = e->next) 
    {
        if (key.xCoord == e->key.xCoord && key.yCoord == e->key.yCoord && key.zCoord == e->key.zCoord)
        {
            /* got it */
            return e->value;
        }
    }

    return 1000;
}

Ant* createAnts(void)
{
	Ant* antArmy;
	antArmy = malloc(sizeof(Ant) * 256);

	for (int i = 0; i < 256; i++)
	{
		Ant currentAnt;
		currentAnt.name = i;
		//currentAnt.name = i - 48 + '0';
		currentAnt.antPosition.xCoord = 0;
		currentAnt.antPosition.yCoord = 0;
		currentAnt.antPosition.zCoord = 0;
		antArmy[i] = currentAnt;
	}

	return antArmy;
}

// write the current ant's character to its current position
void writeChar(Ant *currentAnt, Dict d)
{
	dictInsert(d, currentAnt->antPosition, currentAnt->name);
}

// print the character written at the ant's current position to stdout
void readAndPrintChar(Ant *currentAnt, Dict d)
{
	int currentChar;
	currentChar = dictSearch(d, currentAnt->antPosition);

	if (currentChar == 1000) // if nothing was found at the location, then print a space.
	{
		printf(" ");
	}

	else
	{
		char output = currentChar;
		printf("%c", output); // if something was found, print that character!
	}
}

void moveAnt(Ant *currentAnt, int command)
{
	switch(command)
	{
		case 104 :
			// move the current ant one position west
			// this means decrease xCoord by 1
			currentAnt->antPosition.xCoord--;
			break;

		case 106 :
			// move the current ant one position south
			// this means decrease yCoord by 1
			currentAnt->antPosition.yCoord--;
			break;

		case 107 :
			// move the current ant one position north
			// this means increase yCoord by 1
			currentAnt->antPosition.yCoord++;
			break;

		case 108 :
			// move the current ant one position east
			// this means increase xCoord by 1
			currentAnt->antPosition.xCoord++;
			break;

		case 60 :
			// move the current ant one position up
			// this means increase zCoord by 1
			currentAnt->antPosition.zCoord++;
			break;

		case 62 :
			// move the current ant one position down
			// this means decrease zCoord by 1
			currentAnt->antPosition.zCoord--;
			break;

		case 42 :
			// move the current ant from its current position of [x,y,z] to [2x,2y,2z]
			// basically double all the coordinates
			currentAnt->antPosition.xCoord *= 2;
			currentAnt->antPosition.yCoord *= 2;
			currentAnt->antPosition.zCoord *= 2;
			break;

		default :
			// do nothing, because it should never get here
			printf("error in moveAnt\n");
	}
}

int main(int argc, char **argv)
{
	// need to create a hash table to keep track of the modified locations in the universe
	Dict d;
	d = dictCreate();

	Ant* antArmy = createAnts();

	int c = getchar();
	int numAnts = 0;
	
	Ant* currentAnt = &antArmy[c];
	numAnts++;

	while (c != EOF) // goes until we reach the end of stdin
	{
		while (c != '\n' && c != EOF) // while we are still on the current ant, continue to get commands and act accordingly
		{
			if (c == 104 || c == 106 || c == 107 || c == 108 || c == 60 || c == 62 || c == 42)
			{
				moveAnt(currentAnt, c);
			}

			else if (c == 46)
			{
				writeChar(currentAnt, d);
			}

			else if (c == 63)
			{
				readAndPrintChar(currentAnt, d);
			}

			c = getchar();
		}
		if (c == EOF)
		{
			break;
		}

		c = getchar();

		if (c == EOF) // if EOF is right after \n, exit
		{
			break;
		}
		currentAnt = &antArmy[c]; // otherwise, we have a new currentAnt, so set it
		numAnts++;
		c = getchar(); // get the next char to iterate through the loop
	}

	free(antArmy);
	currentAnt = NULL;
	// free(currentAnt);
	dictDestroy(d);

	return 0;
	/*
	Ant* antArmy = createAnts(); // creating the array of ants
	moveAnt(&antArmy[0], 'h');
	moveAnt(&antArmy[0], 'j');
	moveAnt(&antArmy[0], '>');
	printf("position: [%i,%i,%i]\n", antArmy[0].xCoord, antArmy[0].yCoord, antArmy[0].zCoord);
	unsigned long x = hashFunction(antArmy[0].xCoord, antArmy[0].yCoord, antArmy[0].zCoord);
	printf("%lu\n", x);
	// need to create 

	free(antArmy); // free the array of ants at the end
	*/
}