#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include <limits.h>
#include <assert.h>

#define NUM_CHILDREN (2)

#define LEFT (0)
#define RIGHT (1)

// Invariants: 
// - Every key below child[LEFT] < key < every key below child[RIGHT]
// - Every heapKey in both subtrees < heapKey.
// - Every node stores a correct value for size.
// heapKeys are chosen randomly to ensure balance with high probability.
struct node {
    int key;
    size_t size;
    int combineValue;
    struct node *parent;
    struct node *child[NUM_CHILDREN];
};
typedef struct node Node;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef int (*Combine) (int a, int b);

struct array {
	int numElements; // this stuff is just for the normal C array
	int* normArray;
	Combine combineFunction; // pointer to the combine function we'll use for this Array
	Node *headNode; // pointer to the head node, which will have a value of the aggregate of the whole array
};

Node* arrayCreateHelper(Node* currentNode, size_t n, int *numLeavesMade) // this ultimately returns a pointer to the top node
{
	if (n >= 2) // if we have at least two leaves to make, then we are making a non-leaf node, should malloc them and call recursively
	{
		Node *leftNode = malloc(sizeof(Node));
		leftNode->key = 0;
		leftNode->combineValue = 0;
		leftNode->size = 0;
		currentNode->child[LEFT] = arrayCreateHelper(leftNode, (n - n/2), numLeavesMade);
		currentNode->child[LEFT]->parent = currentNode;
		currentNode->size++;

		Node *rightNode = malloc(sizeof(Node));
		rightNode->key = 0;
		rightNode->combineValue = 0;
		rightNode->size = 0;
		currentNode->child[RIGHT] = arrayCreateHelper(rightNode, (n/2), numLeavesMade);
		currentNode->child[RIGHT]->parent = currentNode;
		currentNode->size++;


		currentNode->size = currentNode->child[LEFT]->size + currentNode->child[RIGHT]->size;
		return currentNode;
	}

	else // here, n == 1 so we are creating a leaf node, so return that leaf node after making it
	{
		currentNode->key = *numLeavesMade;
		currentNode->combineValue = 0;
		currentNode->size = 1;
		currentNode->child[0] = NULL;
		currentNode->child[1] = NULL;
		(*numLeavesMade)++;
		return currentNode;
	}

	return currentNode; // this outputs the headNode
}

// Create a new array holding n values, all initially 0.
// Behavior is undefined if n == 0.
// Cost: O(n).
Array *arrayCreate(int (*combine)(int, int), size_t n)
{
	if (n == 0)
	{
		return 0;
	}

	Array *myArray = malloc(sizeof(Array));
	myArray->numElements = n;
	myArray->normArray = calloc(n, sizeof(int));
	myArray->combineFunction = combine;

	// now, I need to create the binary search tree, starting from the bottom and working my way up
	Node *treeHead = malloc(sizeof(Node));
	treeHead->key = n - 1;
	treeHead->combineValue = 0;
	treeHead->parent = NULL;

	int numLeavesMade = 0;

	treeHead = arrayCreateHelper(treeHead, n, &numLeavesMade); // creating the tree
	treeHead->size = treeHead->size;
	myArray->headNode = treeHead; // set the array to have the tree head as the head node

	return myArray;
}

void treeDestroy(Node *thisNode)
{
	if (thisNode)
	{
		treeDestroy(thisNode->child[LEFT]);
		treeDestroy(thisNode->child[RIGHT]);
		free(thisNode);
	}
}

// Free all space used by an array.
// Cost: O(n).
void arrayDestroy(Array *thisArray)
{
	free(thisArray->normArray);
	treeDestroy(thisArray->headNode);
	free(thisArray);
}

// Return the number of elements of an array.
// Cost: O(1).
size_t arraySize(const Array *thisArray)
{
	return thisArray->numElements;
}

// Return the i-th element of an array
// or 0 if i is out of range.
// Cost: O(log n).
int arrayGet(const Array *thisArray, size_t i)
{
	if (i < 0 || i >= thisArray->numElements)
	{
		return 0;
	}

	else
	{
		return thisArray->normArray[i];
	}
}

void goUpArray(Array *thisArray, Node *currentNode)
{
	Node *parentNode;

	if (currentNode != thisArray->headNode) // go until we get to the headNode
	{
		parentNode = currentNode->parent;
	}
	else
	{
		parentNode = currentNode;
	}

	// at each new parent node, we recompute the aggregate until we reach the top
	parentNode->combineValue = thisArray->combineFunction(parentNode->child[LEFT]->combineValue, parentNode->child[RIGHT]->combineValue);
	parentNode->key = parentNode->child[RIGHT]->key;

	if (parentNode != thisArray->headNode) // if we aren't done, run again from the parent node
	{
		goUpArray(thisArray, parentNode);
	}
}

void goDownArray(Array *thisArray, Node *currentNode, size_t i, int v)
{
	if (currentNode->child[LEFT] == NULL)
	{
		currentNode->combineValue = v;
	}

	else if (i < currentNode->child[LEFT]->size)
	{
		goDownArray(thisArray, currentNode->child[LEFT], i, v);
		currentNode->combineValue = thisArray->combineFunction(currentNode->child[LEFT]->combineValue, currentNode->child[RIGHT]->combineValue);
	}

	else if (i >= currentNode->child[LEFT]->size)
	{
		goDownArray(thisArray, currentNode->child[RIGHT], i - (currentNode->child[LEFT]->size), v);
		currentNode->combineValue = thisArray->combineFunction(currentNode->child[LEFT]->combineValue, currentNode->child[RIGHT]->combineValue);
	}
}

// Set the i-th element of an array to v.
// No effect if i is out of range.
// Cost: O(log n).
void arraySet(Array *thisArray, size_t i, int v)
{
	if (i < thisArray->numElements && i >= 0) // checking to see if i is in range
	{
		thisArray->normArray[i] = v;
		// now the hard part – setting the i-th element to v, and updating the parent (if applicable) to be the aggregate of the children
		goDownArray(thisArray, thisArray->headNode, i, v);
	}
}
// want this to return the very first node where node->key = k OR first node where node->key = k - 1
// then we can just add or subtract whatever is at the desired location
Node* combineSuperHelper(Node *currentNode, size_t k, int n) 
{
	while (currentNode->child[LEFT]->key != k // while we haven't found the correct key
		|| currentNode->child[LEFT]->key != (k-1) 
		|| currentNode->child[RIGHT]->key != k 
		|| currentNode->child[RIGHT]->key != (k-1))
	{
		if (k > (n / 2)) // size 6, if k is 4, 5, or 6, then go right
		{
			currentNode = currentNode->child[RIGHT];

		}
		else // size 6, if k is 1, 2, or 3, go left
		{
			currentNode = currentNode->child[LEFT];
		}
	}
	// now currentNode is the parent of the desired leaf
	return currentNode;
}

int traverseRight(Node *currentNode)
{
	while (currentNode->child[RIGHT] != NULL)
	{
		currentNode = currentNode->child[RIGHT];
	}
	return currentNode->combineValue;
}

int combineHelper(const Array *thisArray, Node *currentNode, size_t k) // find the first one right above k - 1 and return that combineValue - k->combineValue
{
	if (k == 0 || k >= currentNode->size)
	{
		return currentNode->combineValue;
	}

	else if (k > currentNode->child[LEFT]->size)
	{
		return thisArray->combineFunction(currentNode->child[LEFT]->combineValue, combineHelper(thisArray, currentNode->child[RIGHT], k - (currentNode->child[LEFT]->size)));
	}

	else if (k <= currentNode->child[LEFT]->size)
	{
		return combineHelper(thisArray, currentNode->child[LEFT], k);
	}

	return 0;
}

// Return the result of aggregating the first k elements
// of an array in order using the combine function.
// If k is zero or greater than size, returns combination of all elements.
// Cost: O(log n).
int arrayCombine(const Array *thisArray, size_t k)
{
	if (k == 0 || k >= thisArray->numElements)
	{
		return thisArray->headNode->combineValue;
	}
	else
	{
		Node* currentNode = thisArray->headNode;
		return combineHelper(thisArray, currentNode, k);
	}
}