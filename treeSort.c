#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node Node;

struct node {
	int n;
	size_t size;
	Node *parent;
    Node *kid;
    Node *sibling;
};

void destroyTree(Node* thisNode)
{
    if (thisNode == NULL)
    {
    	return;
    }
   	destroyTree(thisNode->kid);
   	destroyTree(thisNode->sibling);
   	free(thisNode);
}

void addChild(Node *root, Node *newChild) // adds newChild as the root or sibling of the root node
// if there is no kid for the root, then this is the kid
// if there IS, then we need to add it to the end of the sibling list of the child
{
	Node *temp; // pointer to kid of child of root

	if (root->kid == NULL) // if root has no child and open to access, newChild is the new child of the root
	{
		root->kid = newChild;
		newChild->parent = root;
	}

	else // if root DOES have a child, then you need to add newChild to the end of the linked list of siblings
	{
		temp = root->kid;
		while (temp->sibling)
		{
			temp = temp->sibling;
		}
		temp->sibling = newChild;
		newChild->parent = root;
	}
	root->size++;
}

void printSubtree(Node *headNode)
{
	Node *currentNode = headNode;

	if (currentNode)
	{
		printf("["); // opening the headNode
	}

	if (currentNode->kid == NULL) // if this node has no kids, it has no siblings too, so just print a ']' to close
	{
		printf("]");
	}

	else // otherwise, the node has a kid
	{
		currentNode = currentNode->kid;
		printSubtree(currentNode);

		while (currentNode->sibling)
		{
			currentNode = currentNode->sibling;
			printSubtree(currentNode);
		}

		printf("]"); // now we are done printing everything inside of this node, so close it
	}
}

// A utility function to swap two elements
void swap(Node* a, Node* b)
{
    Node t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (Node* pointerArray[], int low, int high)
{
    int pivot = pointerArray[high]->size;    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (pointerArray[j]->size >= pivot)
        {
            i++;    // increment index of smaller element
            swap(pointerArray[i], pointerArray[j]);
        }
    }
    swap(pointerArray[i + 1], pointerArray[high]);
    return (i + 1);
}
 
/* The main function that implements QuickSort
 pointerArray --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(Node* pointerArray[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(pointerArray, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(pointerArray, low, pi - 1);
        quickSort(pointerArray, pi + 1, high);
    }
}

void sortSubtree(Node *headNode)
{
	if (headNode->kid != NULL && headNode->kid->sibling != NULL)
	{
		int size = 1;

		Node *currentNode; // this will keep track of where we are
		currentNode = headNode->kid;

		while (currentNode->sibling)
		{
			size++;
			currentNode = currentNode->sibling;
		}
		currentNode = headNode->kid;

		Node *pointerArray[size];
		pointerArray[0] = currentNode;
		int index = 1;

		while (currentNode->sibling)
		{
			pointerArray[index] = currentNode->sibling;
			index++;
			currentNode = currentNode->sibling;
		}
		// now we have an array of pointers, index 0 being the kid and 1+ being the siblings
		// time to sort the array by size
		quickSort(pointerArray, 0, size - 1);

		/*
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				if (pointerArray[i]->size < pointerArray[j]->size)
				{
					Node* a = pointerArray[i];
					pointerArray[i] = pointerArray[j];
					pointerArray[j] = a;
				}
			}
		}
		*/


		// now we have an array of pointers sorted by size
		// now we just need to rebuild the tree in order, so array[0] is the kid and array[1, 2, 3...] are the siblings
		headNode->kid = pointerArray[0];
		Node *previousNode = headNode->kid;
		for (int i = 1; i < size; i++)
		{
			currentNode = pointerArray[i];
			previousNode->sibling = currentNode;
			previousNode = currentNode;
		}
		// once done, we need to make sure the last node doesn't point to a sibling
		currentNode->sibling = NULL;
		// now that all of the nodes right after the headNode (headNode->kid and the siblings of headNode->kid), we need to sort everything after those
		// this is the recursive step – this should cover everything below the current line
		for (int i = 0; i < size; i++)
		{
			sortSubtree(pointerArray[i]);
		}
	}
}

int main(int argc, char **argv)
{
	int c = getchar();

	if (c != '[')
	{
		printf("Wrong usage, need to start with a '[' character.\n");
		return 1;
	}

	int numNodes = 1;
	int numNodesOpen = 1;
	char prevChar = '[';

	Node *unreachableNode = malloc(sizeof(Node));
	unreachableNode->n = -1;
	unreachableNode->parent = NULL;
	unreachableNode->sibling = NULL;
	unreachableNode->size = 0;

	Node *headNode = malloc(sizeof(Node));
	headNode->n = 1;
	headNode->kid = NULL;
	headNode->sibling = NULL;
	headNode->size = 1;
	headNode->parent = unreachableNode;

	Node *currentNode = headNode;

	unreachableNode->kid = headNode;

	c = getchar();

	if (c == ']') // in the case of size 1, return stupid case
	{
		printf("[]");
		return 0;
	}

	int numChars = 1;

	while (c != EOF)
	{

		if (c != '[' && c != ']') // error checking for weird stuff
		{
			destroyTree(headNode);
			free(unreachableNode);
			return 1;
		}

		numChars++;

		if (c == '[') // we need to add a child
		{
			numNodes++;
			Node *tempNode = malloc(sizeof(Node));
			tempNode->n = numNodes;
			tempNode->size = 1;
			tempNode->kid = NULL;
			tempNode->sibling = NULL;
			tempNode->parent = NULL;

			addChild(currentNode, tempNode);
			if (currentNode != headNode)
			{
				headNode->size++;
			}
			currentNode = tempNode;

			numNodesOpen++;
			prevChar = '[';
		}

		else // if c == ']'
		{
			if (prevChar == '[') // then we need to close the most recently opened node from having a kid, which is the root node (no kids)
			{
				currentNode->kid = NULL;
				currentNode = currentNode->parent;
			}

			else if (prevChar == ']')
			{
				currentNode->sibling = NULL;
				currentNode = currentNode->parent;
			}

			numNodesOpen--;
			prevChar = ']';
		}

		if (numNodesOpen == 0)
		{
			break;
		}

		c = getchar();
	}

	if (numNodesOpen != 0)
	{
		printf("Error, you either have an incomplete or messed up tree.\n");
		return 1;
	}

	// printf("Size of tree: %ld\n", headNode->size);
	// printf("Attempting to sort tree...\n");
	sortSubtree(headNode);
	// printf("Done.\n");
	printSubtree(headNode);
	// printf("\n");
	destroyTree(headNode);
	free(unreachableNode);

    return 0;
}